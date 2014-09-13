#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "instruments.h"

const int size_of_header = 36;
const int sample_rate = 44100;
const int samples = sample_rate*4;
uint8_t data[samples];

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
  for (int i = 0; i < samples; i++) {
    data[i] = instrument(i/(float)sample_rate)*127+127;
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
  write(f, 4, size_of_header + samples);         /* chunk size       */
  fputs("WAVE", f);                              /* file format      */
  fputs("fmt ", f);                              /* format chunk     */
  write(f, 4, 16);                               /* size of subchunk */
  write(f, 2, 1);                                /* format (1 = PCM) */
  write(f, 2, 1);                                /* # of channels    */
  write(f, 4, sample_rate);                      /* sample rate      */
  write(f, 4, sample_rate);                      /* byte rate        */
  write(f, 2, 1);                                /* block align      */
  write(f, 2, 8);                                /* bits per sample  */
  fputs("data", f);                              /* data chunk       */
  /* body */
  fwrite(data, 1, samples, f);                   /* actual audio     */

  fclose(f);
}
