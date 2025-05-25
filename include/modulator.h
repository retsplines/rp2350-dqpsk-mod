#ifndef _MODULATOR_H
#define _MODULATOR_H

#include <stdint.h>
#include <complex.h>

void modulator_reset();
complex float modulate(uint symbol);

#endif // _MODULATOR_H