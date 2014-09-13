#include "instruments.h"
#include <math.h>

float instrument(float t) {
  return (sin(t*M_PI*440) + sin(t*M_PI*220)) * exp(-t) * 0.2;
}
