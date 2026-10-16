# gatordot

A Particle-compatible library for DotStar (APA102) LED strips using native SPI. Optimized for memory-constrained devices with pattern reuse via `PixelBlock`.

[![Particle](https://img.shields.io/badge/Particle-Compatible-CE0076?logo=particle&logoColor=white)](https://particle.io)
[![License](https://img.shields.io/badge/License-MIT-007BFF)](LICENSE)

## Features

- **Memory-efficient pattern reuse**: Use a small `PixelBlock` (e.g., 3 pixels) and repeat it across a long strip (e.g., 50+ pixels) without duplicating data
- **Native SPI performance**: Direct SPI transfers for optimal DotStar protocol timing
- **Flexible animation support**: Optional offset parameter in `fill()` for marching patterns and animations
- **Configurable SPI settings**: Customizable SPI clock speed, bit order, and mode

## Installation

Add the library to your Particle project:

```bash
particle library add gatordot
```

Or include the source files directly in your project's `src/` folder.

## Quick Start

```cpp
#include "gatordot.h"

// Create a GatorDot instance for 10 LEDs
GatorDot strip(10);

void setup() {
    strip.begin();
    
    // Fill with a 3-pixel pattern (red, white, blue) repeated across strip
    PixelBlock pattern(3);
    pattern.pixels[0].red = 255; pattern.pixels[0].grn = 0; pattern.pixels[0].blu = 0;
    pattern.pixels[1].red = 255; pattern.pixels[1].grn = 255; pattern.pixels[1].blu = 255;
    pattern.pixels[2].red = 0; pattern.pixels[2].grn = 0; pattern.pixels[2].blu = 255;
    
    strip.fill(&pattern);  // No offset - starts at beginning
}

void loop() {
    // Animation example: march the pattern
    static unsigned int offset = 0;
    strip.fill(&pattern, offset);
    offset = (offset + 1) % pattern.length;
    delay(250);
}
```

## Examples

See the [examples](examples) folder:
- **usage**: Basic red/green/blue/white color test sequence
- **marching_ants**: 10-LED strip with red/white/blue pattern animating at 2 FPS

## API

### GatorDot

```cpp
GatorDot(unsigned int n, SPISettings settings = SPISettings(8*MHZ, MSBFIRST, SPI_MODE3))
```
Create a GatorDot instance for `n` pixels. Default SPI uses 8MHz, MSBFIRST, SPI_MODE3 (APA102 spec).

```cpp
void begin()
```
Initialize SPI hardware.

```cpp
void fill(PixelBlock* pxls, unsigned int offset = 0)
```
Fill the strip with a pattern from `PixelBlock`, optionally starting at `offset` positions into the pattern.

```cpp
void clear()
```
Clear all pixels (send zeros to all LEDs).

### PixelBlock

```cpp
PixelBlock(unsigned int l)
```
Create a pixel block of length `l`. Memory is allocated for `l` `jColor` structs.

```cpp
~PixelBlock()
```
Destructor releases allocated memory.

### jColor

Struct with `red`, `grn`, `blu` members (0-255 each).

## Hardware

Connect your APA102/DotStar strip to your Particle device:

| Strip Wire | Color | Connection |
|------------|-------|------------|
| Data (DI)  | Blue  | MOSI (A5 on Photon) |
| Clock (CI) | Red   | SCK (A3 on Photon) |
| +5V        | Red   | 5V power |
| GND        | Black | Ground |

**Note**: The APA102 uses SPI MODE3 (CPOL=1, CPHA=1) with MSBFIRST. The library configures this by default.

## Memory Optimization

The `PixelBlock` design allows you to store a small pattern and repeat it:

```cpp
// Instead of 100 pixel colors (300 bytes RAM):
// jColor pixels[100];

// Use a 5-pixel pattern (15 bytes RAM):
PixelBlock pattern(5);
// ... set 5 colors ...
strip.fill(&pattern);  // Repeats pattern 20x
```

## Contributing

1. Fork and clone the repository
2. Create a feature branch (`git checkout -b feature/amazing-thing`)
3. Make your changes and commit (`git commit -m 'Add amazing thing'`)
4. Push to your branch (`git push origin feature/amazing-thing`)
5. Open a Pull Request

## License

MIT License - see [LICENSE](LICENSE) for details.

---

Copyright 2020–2023 Gatorbot LLC
