#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "instruments.h"
#include "loadscore.h"
#include "note.h"

#define SIZE_OF_HEADER 36
#define SAMPLE_RATE 44100

uint8_t* data;
int samples = 0;
int next_note = 0;
float duration;

note* score;
int score_len;

note *notes;
char *inputname  = "score.cscore";
char *outputname = "out.wav";

void synthesize();
void writewav();
void badargs();

char *exename;

void badargs() {
   printf("usage: %s <score filename> <output filename>\n"
	   "       %s <score filename> (assumes 'out.wav')\n"
	   "       %s (assumes 'score.cscore') (assumes 'out.wav')\n",
	  exename, exename, exename);
   exit(1);
}

int main(int argc, char *argv[])
{
  exename = argv[0];
  if (argc >= 3) {
    badargs();
  }
  if (argc > 1) {
    inputname = argv[1];
  }
  if (argc > 2) {
    outputname = argv[2];
  }
  score = loadscore(inputname, &score_len, &duration);
  if (score == NULL) return 1;
  samples = duration*SAMPLE_RATE;
  data = malloc(samples);
  synthesize();
  writewav(outputname);
  free(data);
  free(score);
  return 0;
}

void synthesize()
{
  int i;
  float t, note_t;
  for (i = 0; i < samples; i++) {
    t = i/(float)SAMPLE_RATE;
    /* when a note beings, add it to the list of playing notes */
    while (next_note < score_len && score[next_note].begin < t) {
      /* push it to the front of a linked list */
      score[next_note].next = notes;
      notes = &score[next_note];
      next_note++;
    }

    note *n = notes;
    float accum = 0.0;
    while (1) {
      if (n == NULL) break;
      note_t = t - n->begin;
      accum += instrument(note_t, n->hz);

      /* once a note has ended, unlink it from the list */
      if (note_t > n->duration) {
	if (n->previous)
	  n->previous->next = n->next;
	if (n->next)
	  n->next->previous = n->previous;
	if (n == notes)
	  notes = n->next;
	n->next = NULL; n->previous = NULL;
      }

      n = n->next;
    }

    data[i] = accum*127 + 127;
  }
}

void write(FILE* f, int size, int32_t arg)
{
  uint8_t x;
  int16_t y;
  int32_t z;

  switch (size) {
  case 1:
    x = (uint8_t) arg;
    fwrite(&x, size, 1, f);
    break;
  case 2:
    y = (int16_t) arg;
    fwrite(&y, size, 1, f);
    break;
  case 4:
    z = (int32_t) arg;
    fwrite(&z, size, 1, f);
    break;
  }
}

void writewav(const char* filename) 
{
  FILE* f = fopen(filename, "w");

  /* header*/
  fputs("RIFF", f);                              /* main chunk       */
  write(f, 4, SIZE_OF_HEADER + samples);         /* chunk size       */
  fputs("WAVE", f);                              /* file format      */
  fputs("fmt ", f);                              /* format chunk     */
  write(f, 4, 16);                               /* size of subchunk */
  write(f, 2, 1);                                /* format (1 = PCM) */
  write(f, 2, 1);                                /* # of channels    */
  write(f, 4, SAMPLE_RATE);                      /* sample rate      */
  write(f, 4, SAMPLE_RATE);                      /* byte rate        */
  write(f, 2, 1);                                /* block align      */
  write(f, 2, 8);                                /* bits per sample  */
  fputs("data", f);                              /* data chunk       */
  /* body */
  fwrite(data, 1, samples, f);                   /* actual audio     */

  fclose(f);
}
