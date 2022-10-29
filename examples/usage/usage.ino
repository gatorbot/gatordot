// ©2020 GatorbotLLC 
// DotStar Test
// My intent is to create my own SPI-based dotstar class
#include "gatordot.h"

// MAIN
GatorDot strip(5);

void setup() {
    Particle.variable("app", "GatorDot.usage()");

    strip.begin();
    PixelBlock* block = new PixelBlock(5);
    // red
    for (int i = 0; i<block->length; i++) {
        block->pixels[i].red = 255;
        block->pixels[i].grn = 0;
        block->pixels[i].blu = 0;
    }
    strip.fill(block);
    delay(5000);
    
    // greeen
    for (int i = 0; i<block->length; i++) {
        block->pixels[i].red = 0;
        block->pixels[i].grn = 255;
        block->pixels[i].blu = 0;
    }
    strip.fill(block);
    delay(5000);

    // blue
    for (int i = 0; i<block->length; i++) {
        block->pixels[i].red = 0;
        block->pixels[i].grn = 0;
        block->pixels[i].blu = 255;
    }
    strip.fill(block);
    delay(5000);

    // white
    for (int i = 0; i<block->length; i++) {
        block->pixels[i].red = 255;
        block->pixels[i].grn = 255;
        block->pixels[i].blu = 255;
    }
    strip.fill(block);
    delay(5000);
    strip.clear();

}

void loop() {

}