// ©2020 GatorbotLLC 
// DotStar Usage Example
// Demonstrates basic usage of the GatorDot library
#include "gatordot.h"

// MAIN
GatorDot strip(5);

void setup() {
    Particle.variable("app", "GatorDot.usage()");

    strip.begin();
    PixelBlock block(5);
    // red
    for (int i = 0; i<block.length; i++) {
        block.pixels[i].red = 255;
        block.pixels[i].grn = 0;
        block.pixels[i].blu = 0;
    }
    strip.fill(&block);
    delay(5000);
    
    // green
    for (int i = 0; i<block.length; i++) {
        block.pixels[i].red = 0;
        block.pixels[i].grn = 255;
        block.pixels[i].blu = 0;
    }
    strip.fill(&block);
    delay(5000);

    // blue
    for (int i = 0; i<block.length; i++) {
        block.pixels[i].red = 0;
        block.pixels[i].grn = 0;
        block.pixels[i].blu = 255;
    }
    strip.fill(&block);
    delay(5000);

    // white
    for (int i = 0; i<block.length; i++) {
        block.pixels[i].red = 255;
        block.pixels[i].grn = 255;
        block.pixels[i].blu = 255;
    }
    strip.fill(&block);
    delay(5000);
    strip.clear();

}

void loop() {

}