#include "instruments.h"
#include <math.h>

float instrument(float t, float hz) {
  return sin(t*M_PI*hz) * exp(-t) * 0.2;//(sin(t*M_PI*440) + sin(t*M_PI*220)*0.25) * exp(-t) * 0.2;
}
