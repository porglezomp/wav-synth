#include "instruments.h"
#include <math.h>

float frac(float x) {
  return x-floor(x);
}

float square(float t, float hz) {
  float x = t*hz;
  return 2*floor(x) - floor(2*x) + 1;
}

float instrument(float t, float hz) {
  return square(t, hz) * 0.2;
  //  return sin(t*M_PI*2*hz) * exp(-t) * 0.2;//(sin(t*M_PI*440) + sin(t*M_PI*220)*0.25) * exp(-t) * 0.2;
}
