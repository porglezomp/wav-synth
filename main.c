#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "instruments.h"
#include "note.h"

#define SIZE_OF_HEADER 36
#define SAMPLE_RATE 44100
#define SECONDS 8
#define SAMPLES SAMPLE_RATE*SECONDS

uint8_t data[SAMPLES];
int num_notes = 1;
int next_note = 0;

extern note score[];
extern int score_len;

note *notes;

void synthesize();
void writewav();

int main()
{
  synthesize();
  writewav("test.wav");
  return 0;
}

void synthesize()
{
  int i;
  float t, note_t;
  for (i = 0; i < SAMPLES; i++) {
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
  write(f, 4, SIZE_OF_HEADER + SAMPLES);         /* chunk size       */
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
  fwrite(data, 1, SAMPLES, f);                   /* actual audio     */

  fclose(f);
}
