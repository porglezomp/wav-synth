#ifndef _NOTE_H_
#define _NOTE_H_

typedef struct note_t note;
struct note_t {
  float hz, begin, duration;
  note *previous, *next;
};

#endif
