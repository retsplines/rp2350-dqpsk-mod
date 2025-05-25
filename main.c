#include <stdio.h>
#include <complex.h>
#include "pico/stdlib.h"
#include "pico/rand.h"    
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"
#include "i2s.pio.h"
#include "include/modulator.h"
#include "include/filter.h"

// The data pin & base clock pin for the I2S output
#define DATA_PIN 5
#define CLOCK_BASE_PIN 2

// The upsampling rate (number of samples per symbol)
#define UPSAMPLE_RATE 8

// Should we just output a circular test pattern?
#define CIRCLE_TEST 0

// The PIO instance to use
PIO pio = pio0;
int sm = 0;

/**
 * Modulate a symbol (value 0 to 3) and push the samples into the RRC filter.
 * As values emerge from the filter, add them to the PIO TX FIFO for output.
 */
void put_symbol(uint symbol) {

    complex float phase = modulate(symbol);

    // Push the sample into the filter
    for (int samp_index = 0; samp_index < UPSAMPLE_RATE; samp_index ++) {

        complex float filtered_samp = filter(samp_index == 0 ? phase : 0.0f + 0.0f * I);
        
        // Convert the filtered sample to two 16-bit floats
        int16_t real_part = (int16_t)(creal(filtered_samp) * 32767.0f);
        int16_t imag_part = (int16_t)(cimag(filtered_samp) * 32767.0f);

        // Push the filter output into the TX FIFO for transmission
        pio_sm_put_blocking(pio, sm, (real_part & 0xffff) << 16 | (imag_part & 0xffff));
    }
}

int main() {

    // Set system clock
    set_sys_clock_hz(200 * MHZ, true);
    setup_default_uart();
    uint offset;

    // Add the program
    offset = pio_add_program(pio, &i2s_program);

    // Initialise the program
    i2s_program_init(pio, sm, offset, DATA_PIN, CLOCK_BASE_PIN);

    // Set the clock divider
    // Running at 200MHz, two cycles per bit = 100MHz effective base clock
    // 144kHz word clock (144kHz sample rate)
    // 32 bits per sample = 144,000 * 32 = 4,608,000 Hz BCLK
    // 100MHz/4,608,000Hz = 21.7013888889
    pio_sm_set_clkdiv(pio, sm, 21.7013888889);

    // Enable
    pio_sm_set_enabled(pio, sm, true);

    // Constantly push data to the FIFO, randomising the symbol each time
    while (true) {

        // Randomly select a symbol & output it
        #if CIRCLE_TEST
        static uint symbol = 0;
        #else
        uint symbol = get_rand_32() % 4;
        #endif
        put_symbol(symbol);   
    }
}
