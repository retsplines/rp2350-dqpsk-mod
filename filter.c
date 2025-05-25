#include "include/filter.h"
#include "complex.h"

// Span of the filter is 6 symbols, 8 samples each, plus one for symmetry = 49 taps
#define NUMBER_OF_FILTER_TAPS 49

static float filter_coef[NUMBER_OF_FILTER_TAPS] = {
 -0.0028965468899174998,
-0.0032358975382416567,
-0.0025791896973552855,
-0.0008537298483591387,
0.0017294783774642456,
0.004671405683775374,
0.007252551717854649,
0.008667905271797399,
0.008208427832625676,
0.005454626197276931,
0.0004404642246052356,
-0.006252949375184442,
-0.013504223085090056,
-0.019771553228367652,
-0.02331577635843313,
-0.022494485078891235,
-0.016077514982750497,
-0.003525279521105954,
0.014826237298844783,
0.03771630873989669,
0.06306564393508352,
0.08822941266619351,
0.11036085564536234,
0.12682724803053408,
0.1356083065374232,
0.1356083065374232,
0.12682724803053408,
0.11036085564536234,
0.08822941266619351,
0.06306564393508352,
0.03771630873989669,
0.014826237298844783,
-0.003525279521105954,
-0.016077514982750497,
-0.022494485078891235,
-0.02331577635843313,
-0.019771553228367652,
-0.013504223085090056,
-0.006252949375184442,
0.0004404642246052356,
0.005454626197276931,
0.008208427832625676,
0.008667905271797399,
0.007252551717854649,
0.004671405683775374,
0.0017294783774642456,
-0.0008537298483591387,
-0.0025791896973552855,
-0.0032358975382416567,
};

// A circular buffer so we don't have to shift the state
static complex float filter_state[NUMBER_OF_FILTER_TAPS] = {0 + 0 * I};

// The cell of filter_state that was most recently written to
static size_t newest_sample_index = 0;

/**
 * Reset the filter state.
 */
void filter_reset() {
    for (int i = 0; i < NUMBER_OF_FILTER_TAPS; i++) {
        filter_state[i] = 0.0f + 0.0f * I;
    }
}

/**
 * Filter a sample. 
 */
complex float filter(complex float input) {
    
    filter_state[newest_sample_index] = input;
    
    // Compute the output (perform the convolution), reading each sample, starting with the most recent
    complex float output = 0.0f + 0.0f * I;
    size_t read_index = newest_sample_index;

    for (int i = 0; i < NUMBER_OF_FILTER_TAPS; i++) {

        output += filter_coef[i] * filter_state[read_index];

        // Wrap the read index
        if (read_index == NUMBER_OF_FILTER_TAPS - 1) {
            read_index = 0;
        } else {
            read_index ++;
        }

    }

    // Update the newest sample index, wrapping at the start
    if (newest_sample_index == 0) {
        newest_sample_index = NUMBER_OF_FILTER_TAPS - 1;
    } else {
        newest_sample_index --;
    }

    return output;
}
