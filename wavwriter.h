#ifndef _WAV_SYNTH_WAVWRITER_H_INCLUDED
#define _WAV_SYNTH_WAVWRITER_H_INCLUDED

#include <stdint.h>
void writewav(uint8_t *data, int samples, int sample_rate, const char* filename);

#endif
