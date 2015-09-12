#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "instruments.h"
#include "wavwriter.h"
#include "loadscore.h"
#include "note.h"

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
  writewav(data, samples, SAMPLE_RATE, outputname);
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
    while (next_note < score_len && score[next_note].begin <= t) {
      /* push it to the front of a linked list */
      score[next_note].next = notes;
      notes = &score[next_note];
      if (notes->next) {
        notes->next->previous = notes;
      }
      next_note++;
    }

    note *n = notes;
    float accum = 0.0;
    while (1) {
      if (n == NULL) break;
      note_t = t - n->begin;
      accum += instrument(note_t, n->hz);
      note *next = n->next;

      /* once a note has ended, unlink it from the list */
      if (note_t > n->duration) {
        if (n->previous)
          n->previous->next = n->next;
        if (n->next)
          n->next->previous = n->previous;
        if (notes == n)
          notes = n->next;

        /* set the current links to NULL */
        n->next = NULL; n->previous = NULL;
      }

      n = next;
    }

    data[i] = accum*127 + 127;
  }
}
