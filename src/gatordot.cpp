/**
 * @file gatordot.cpp
 * @brief Implementation of GatorDot APA102/DotStar LED strip library
 *
 * This file contains the implementation details for the Particle-compatible
 * GatorDot library. The library uses native SPI hardware to communicate with
 * APA102/DotStar LED strips, which require precise timing for data transmission.
 *
 * @copyright 2020–2023 Gatorbot LLC
 * @license MIT
 */

#include "gatordot.h"

/**
 * @brief Construct a PixelBlock with the specified number of pixels
 * @param l Number of pixels to allocate space for
 *
 * Allocates memory for the pixel array. The caller is responsible for
 * filling the pixel array with color values after construction.
 */
PixelBlock::PixelBlock(unsigned int l)
    : length(l), pixels(new jColor[length]) {
}

/**
 * @brief Destroy the PixelBlock and free allocated memory
 *
 * Deallocates the pixel array and resets the length to zero.
 */
PixelBlock::~PixelBlock() {
    delete[] pixels;
    length = 0;
}

/**
 * @brief Construct a GatorDot instance for a strip of n pixels
 * @param n Number of pixels in the strip
 * @param s SPI configuration for communication with the strip
 *
 * Initializes the brightness to maximum (0xFF) and stores the
 * strip length and SPI settings. SPI hardware is not started
 * until begin() is called.
 */
GatorDot::GatorDot(unsigned int n, SPISettings s)
    : brightness(0xFF), numpix(n), settings(s),
      startFrame(0x00000000), endFrame(0xFFFFFFFF), bMask(0xE0) {
}

/**
 * @brief Initialize SPI hardware for communication with the LED strip
 *
 * Calls SPI.begin() to set up the SPI bus with the default pins.
 * The actual SPI bus configuration (clock speed, bit order, mode)
 * is specified when each transfer is performed via SPI.beginTransaction().
 */
void GatorDot::begin() {
    SPI.begin();
}

/**
 * @brief Fill the strip with colors from a PixelBlock pattern
 * @param pxls Pointer to the PixelBlock containing the color pattern
 * @param offset Starting position offset within the pattern (default: 0)
 *
 * This method implements the APA102 protocol:
 * 1. Send start frame (32 zero bits)
 * 2. Send each pixel's 32-bit header + RGB data
 * 3. Send end frame (enough zero bytes to latch all LEDs)
 *
 * The pattern from PixelBlock is repeated across the strip, with the
 * offset parameter allowing animation by shifting the starting point.
 *
 * APA102 pixel format (32 bits per pixel):
 * - Header: 1 1 B4 B3 B2 B1 B0 1 (5-bit brightness + fixed bits)
 * - Data: Blue (8 bits), Green (8 bits), Red (8 bits)
 *
 * @note The brightness value from the brightness member variable is
 * applied to all pixels during transfer.
 */
void GatorDot::fill(PixelBlock* pxls, unsigned int offset) {
    SPI.beginTransaction(settings);
    
    // Start frame: APA102 requires 32 zero bits before pixel data
    SPI.transfer(&startFrame, NULL, 4, NULL);
    
    // Send each pixel in the strip
    for (int i = 0; i < numpix; ++i) {
        // Get the color from the pattern (with wrapping for offset)
        const jColor& color = pxls->pixels[(i + offset) % pxls->length];
        
        // APA102 pixel format: 11BBBBB1 followed by B, G, R (each 8 bits)
        // The header includes 5 bits of brightness (top 5 bits of bMask + brightness)
        uint8_t header = brightness | bMask;  // 1 1 B4 B3 B2 B1 B0 1
        SPI.transfer(header);
        SPI.transfer(color.blu);
        SPI.transfer(color.grn);
        SPI.transfer(color.red);
    }
    
    // End frame: APA102 spec requires n/8 + 1 zero bytes after pixel data
    // to ensure all LEDs latch and update
    int endFrameBytes = (numpix / 8) + 1;
    for (int i = 0; i < endFrameBytes; i++) {
        SPI.transfer(0x00);
    }
    
    SPI.endTransaction();
}

/**
 * @brief Clear all pixels (turn them off)
 *
 * Sends zero RGB values with minimum brightness header to all pixels,
 * effectively turning off the entire strip. The APA102 protocol is
 * followed with start frame, pixel data, and end frame.
 *
 * This is equivalent to calling fill() with a pattern of black pixels.
 */
void GatorDot::clear() {
    // Zero RGB with minimum brightness (0xE0 = 11100000 in APA102 header)
    uint8_t buf[] = {0xE0, 0, 0, 0};
    
    SPI.beginTransaction(settings);
    
    // Start frame
    SPI.transfer(&startFrame, NULL, 4, NULL);
    
    // Send zero pixels for all LEDs
    for (int i = 0; i < numpix; i++) {
        SPI.transfer(buf, NULL, 4, NULL);
    }
    
    // End frame
    int endFrameBytes = (numpix / 8) + 1;
    for (int i = 0; i < endFrameBytes; i++) {
        SPI.transfer(0x00);
    }
    
    SPI.endTransaction();
}
