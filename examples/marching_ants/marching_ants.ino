// ©2020 GatorbotLLC 
// Marching Ants Example
// Demonstrates animation using offset parameter in fill()

#include "gatordot.h"

// MAIN - 10 LEDs, using 3-pixel pattern block
GatorDot strip(10);

// Global pattern block - allocated once to save RAM
PixelBlock pattern(3);

unsigned int frameCount = 0;

void setup() {
    Particle.variable("app", "GatorDot.marching_ants");
    
    strip.begin();
    
    // Initialize pattern: red, white, blue
    pattern.pixels[0].red = 255;
    pattern.pixels[0].grn = 0;
    pattern.pixels[0].blu = 0;
    
    pattern.pixels[1].red = 255;
    pattern.pixels[1].grn = 255;
    pattern.pixels[1].blu = 255;
    
    pattern.pixels[2].red = 0;
    pattern.pixels[2].grn = 0;
    pattern.pixels[2].blu = 255;
}

void loop() {
    // Fill strip with pattern, offset by frameCount to animate
    strip.fill(&pattern, frameCount);
    
    // Update frame count (2 frames per second = 500ms delay)
    frameCount = (frameCount + 1) % pattern.length;
    
    delay(500);
}
