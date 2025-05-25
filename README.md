# RP2350 / Raspberry Pi Pico 2 π/4 DQPSK Modulator

Little experiment generating π/4 DQPSK baseband with Raspberry Pi RP2350.

This project is the subject of a blog post [here](https://foxdog.blog/blog/digital-modulation-rp2350).

No guarantees are made as to the correctness or suitability of the implementation for anything at all.

## Building

The normal Pico SDK build process should work:

```
mkdir build
cd build
cmake ..
make
```

This will produce a `uf2` that you can copy to Raspberry Pi Pico 2 board over USB.

