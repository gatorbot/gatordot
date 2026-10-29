/**
 * @file gatordot.h
 * @brief Particle-compatible library for APA102/DotStar LED strips using native SPI
 *
 * This library provides efficient control of APA102/DotStar LED strips via SPI,
 * optimized for memory-constrained Particle devices. Key features include:
 * - Native SPI hardware acceleration for precise timing
 * - Memory-efficient pattern reuse via PixelBlock
 * - Configurable SPI settings (clock speed, bit order, mode)
 *
 * Connection (default pins for Photon 2):
 * - DI (Data In)  -> MOSI (A5)
 * - CI (Clock In) -> SCK  (A3)
 * - +5V           -> 5V power
 * - GND           -> Ground
 *
 * APA102 protocol uses SPI MODE3 (CPOL=1, CPHA=1) with MSBFIRST.
 *
 * @copyright 2020–2023 Gatorbot LLC
 * @license MIT
 */

#ifndef GATORDOT_H
#define GATORDOT_H

#include "Particle.h"

/**
 * @struct jColor
 * @brief Represents a single LED color as RGB components
 *
 * Each color channel (red, green, blue) accepts values from 0-255.
 * Usage:
 * @code
 * jColor pixel;
 * pixel.red = 255;
 * pixel.grn = 128;
 * pixel.blu = 64;
 * @endcode
 */
struct jColor {
    uint8_t red;  ///< Red component (0-255)
    uint8_t grn;  ///< Green component (0-255)
    uint8_t blu;  ///< Blue component (0-255)
};

/**
 * @struct PixelBlock
 * @brief A reusable pattern block for LED strips
 *
 * PixelBlock stores a small sequence of colors that can be repeated
 * across a longer LED strip without duplicating memory. This is key
 * for memory-constrained devices—store 3 pixels and repeat them
 * across 100+ LEDs using only the memory for 3 pixels.
 *
 * Usage:
 * @code
 * PixelBlock pattern(4);  //Create a 4-pixel pattern
 * pattern.pixels[0] = {255, 0, 0};   //Red
 * pattern.pixels[1] = {0, 255, 0};   //Green
 * pattern.pixels[2] = {0, 0, 255};   //Blue
 * pattern.pixels[3] = {255, 255, 255}; //White
 * strip.fill(&pattern);  //Repeats pattern across all pixels
 * @endcode
 */
struct PixelBlock {
    unsigned int length;  ///< Number of pixels in this block
    jColor* pixels;       ///< Pointer to array of pixel colors (length bytes)

    /**
     * @brief Construct a PixelBlock with the given number of pixels
     * @param l Number of pixels to allocate space for
     */
    PixelBlock(unsigned int l);

    /**
     * @brief Destroy the PixelBlock and free allocated memory
     */
    ~PixelBlock();
};

/**
 * @struct GatorDot
 * @brief Main class for controlling APA102/DotStar LED strips via SPI
 *
 * GatorDot uses native SPI hardware for optimal performance and precise
 * timing required by the APA102 protocol. The default SPI settings
 * (8MHz, MSBFIRST, SPI_MODE3) match the APA102 datasheet specifications.
 *
 * Usage:
 * @code
 * #include "gatordot.h"
 *
 * GatorDot strip(150);  //150 pixels
 *
 * void setup() {
 *     strip.begin();
 *     strip.clear();
 * }
 *
 * void loop() {
 *     PixelBlock pattern(3);
 *     pattern.pixels[0] = {255, 165, 0};  //Orange
 *     pattern.pixels[1] = {255, 165, 0};
 *     pattern.pixels[2] = {75, 0, 130};   //Deep purple
 *     strip.fill(&pattern);
 *     delay(1000);
 * }
 * @endcode
 */
struct GatorDot {
    // Member variables

    /**
     * @brief Global brightness level (0-224, in 32-level steps)
     *
     * Note: APA102 has per-pixel brightness control. This is a global
     * multiplier applied during fill operations.
     */
    uint8_t brightness;

    /**
     * @brief Total number of pixels in the connected strip
     */
    unsigned int numpix;

    /**
     * @brief SPI configuration for communicating with the strip
     *
     * Default: 8MHz, MSBFIRST, SPI_MODE3 (as required by APA102)
     */
    SPISettings settings;

    // APA102 protocol constants

    /**
     * @brief Start frame marker (32 bits of zero)
     *
     * APA102 requires a start frame of at least 32 zero bits before
     * the pixel data. This value represents the raw uint32_t pattern.
     */
    uint32_t startFrame;

    /**
     * @brief End frame marker (32 bits of one)
     *
     * APA102 requires an end frame of at least 32 one bits after
     * pixel data to latch the output. This ensures all pixels update.
     */
    uint32_t endFrame;

    /**
     * @brief Brightness mask for APA102 header (0xE0 = top 3 bits)
     *
     * APA102 pixel headers use 5 bits of brightness (top 3 bits of
     * this mask) plus 2 bits of 1s (lower 2 bits). The 8-bit header
     * format is: 1 1 B4 B3 B2 B1 B0 1 where B4-B0 are brightness bits.
     */
    uint8_t bMask;

    // Public methods

    /**
     * @brief Construct a GatorDot instance for a strip of n pixels
     * @param n Number of pixels in the strip
     * @param settings SPI configuration (default: 8MHz, MSBFIRST, MODE3)
     */
    GatorDot(unsigned int n, SPISettings settings = SPISettings(8*MHZ, MSBFIRST, SPI_MODE3));

    /**
     * @brief Initialize SPI hardware and prepare the strip
     *
     * Call this once in setup() before any other GatorDot operations.
     * Sets up the SPI bus with the configured settings.
     */
    void begin();

    /**
     * @brief Fill the strip with a pattern from PixelBlock
     * @param pxls Pointer to the PixelBlock containing the color pattern
     * @param offset Starting position offset within the pattern (default: 0)
     *
     * This method repeats the pattern stored in PixelBlock across the
     * entire strip. The optional offset allows animation by shifting
     * the starting point in the pattern.
     *
     * Example for animation (marching effect):
     * @code
     * static unsigned int offset = 0;
     * strip.fill(&pattern, offset);
     * offset = (offset + 1) % pattern.length;
     * @endcode
     */
    void fill(PixelBlock* pxls, unsigned int offset = 0);

    /**
     * @brief Clear all pixels (turn them off)
     *
     * Sends zero brightness values to all pixels, effectively turning
     * off the entire strip.
     */
    void clear();
};

#endif // GATORDOT_H