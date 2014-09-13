#include <stdio.h>
#include <math.h>

typedef __uint8_t uint8;
typedef __int16_t int16;
typedef __int32_t int32;

const int size_of_header = 36;
const int sample_rate = 44100;
const int samples = sample_rate*4;
uint8 data[samples];

void synthesize();
void writewav();

int main()
{
  synthesize();
  writewav();
  return 0;
}

float sound(float t)
{
  float f = sin(sqrt(t)*M_PI*440)*exp(-t*2);
  return f;
}

void synthesize()
{
  for (int i = 0; i < samples; i++) {
    data[i] = sound(i/(float)sample_rate)*127+127;
  }
}

void write(FILE* f, int size, int32 arg)
{
  uint8 x;
  int16 y;
  int32 z;

  switch (size) {
  case 1:
    x = (uint8) arg;
    fwrite(&x, size, 1, f);
    break;
  case 2:
    y = (int16) arg;
    fwrite(&y, size, 1, f);
    break;
  case 4:
    z = (int32) arg;
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
