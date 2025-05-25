#include <complex.h>
#include <stdint.h>
#include <stdio.h>
#include "pico/stdlib.h"

// The number of symbols possible
#define NUMBER_OF_SYMBOLS 4

// Number of possible phase states
#define NUMBER_OF_PHASES 8

// The change in phase (in multiples of pi/4) for each bit pair
const int symbol_phase_diffs[NUMBER_OF_SYMBOLS] = {
    1,  // 00
    3,  // 01
    -1, // 10
    -3  // 11
};

// Mapping of the possible absolute phases (multiples of 1/4π) to their I/Q values
const complex float absolute_phases[NUMBER_OF_PHASES] = {
    1.0f + 0.0f * I, // 0 * pi/4
    0.7071067690849304 + 0.7071067690849304 * I , // 1 * pi/4
    0.0 + 1.0 * I, // 2 * pi/4
    -0.7071067690849304 + 0.7071067690849304 * I, // 3 * pi/4
    -1.0 + 0.0 * I, // 4 * pi/4
    -0.7071067690849304 - 0.7071067690849304 * I, // 5 * pi/4
    0.0 - 1.0 * I, // 6 * pi/4
    0.7071067690849304 - 0.7071067690849304 * I // 7 * pi/4
};

// Internal modulator state
static int last_phase = 0;

/**
 * Reset the modulator.
 */
void modulator_reset() {
    last_phase = 0;
}

/**
 * Modulate a symbol. 
 */
complex float modulate(unsigned int symbol) {

    if (symbol >= NUMBER_OF_SYMBOLS) {
        // Invalid symbol, return zero
        printf("invalid symbol provided to modulate() - %d", symbol);
        return 0.0f + 0.0f * I;
    }

    int next_symbol_phase_diff = symbol_phase_diffs[symbol];

    // Compute the difference
    int new_phase = last_phase + next_symbol_phase_diff;
    last_phase = ((new_phase % NUMBER_OF_PHASES) + NUMBER_OF_PHASES) % NUMBER_OF_PHASES;

    return absolute_phases[last_phase];
}

