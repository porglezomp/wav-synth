#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "instruments.h"

#define SIZE_OF_HEADER 36
#define SAMPLE_RATE 44100
#define SECONDS 4
#define SAMPLES SAMPLE_RATE*SECONDS
uint8_t data[SAMPLES];

void synthesize();
void writewav();

int main()
{
  synthesize();
  writewav();
  return 0;
}

void synthesize()
{
  for (int i = 0; i < SAMPLES; i++) {
    data[i] = instrument(i/(float)SAMPLE_RATE)*127+127;
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

void writewav() 
{
  FILE* f = fopen("test.wav", "w");

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
