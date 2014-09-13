#include "loadscore.h"
#include "note.h"
#include <stdlib.h>
#include <stdio.h>

note* loadscore(const char *name, int *length, float *duration) {
  note* notes;
  int i;
  float hz, start, len;
  FILE *f;
  
  f= fopen(name, "r");
  
  fscanf(f, "%i notes\n", length);
  //  double tmp;
  fscanf(f, "%f seconds\n", duration);
  //  *duration = ((float)tmp;
  printf("%i notes, %f seconds long\n", *length, *duration);
  if (length == 0) {
    fclose(f);
    return NULL;
  }

  notes = malloc(sizeof(note)*(*length));

  for (i = 0; i < *length; i++) {
    fscanf(f, "%f %f %f", &hz, &start, &len);
    note n = {hz, start, len, NULL, NULL};
    notes[i] = n;
  }

  fclose(f);
  return notes;
}
