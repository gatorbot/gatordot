// ©2020 GatorbotLLC 
// DotStar Test
// My intent is to create my own SPI-based dotstar class
#include "gatordot.h"

// PIXELBLOCK
//
PixelBlock::PixelBlock(unsigned int l){
    // create a new block of pixels
    length = l;
    pixels = new jColor[length];
}
PixelBlock::~PixelBlock(){
    delete[] pixels;
    length = 0;
}

// GATORDOT
//
GatorDot::GatorDot(unsigned int n, SPISettings s) 
    : brightness(0xFF), numpix(n), settings(s) {
}

void GatorDot::begin(){
    // Set up SPI
    SPI.begin();
}

void GatorDot::fill(PixelBlock* pxls){
    SPI.beginTransaction(settings);
    SPI.transfer(&startFrame, NULL, 4, NULL); // start frame
    for (int i = 0; i < numpix; ++i){
        // send a pixel repeating pixels in block for numpix
        SPI.transfer(brightness | bMask);
        SPI.transfer(pxls->pixels[i%pxls->length].blu);
        SPI.transfer(pxls->pixels[i%pxls->length].grn);
        SPI.transfer(pxls->pixels[i%pxls->length].red);
    }
    // Send enough zero bytes to ensure all LEDs latch (APA102 spec: n/8 + 1 zeros)
    int endFrameBytes = (numpix / 8) + 1;
    for (int i = 0; i < endFrameBytes; i++) {
        SPI.transfer(0x00);
    }
    SPI.endTransaction();
}

void GatorDot::clear(){
    uint8_t buf[] = {0xE0,0,0,0};
    SPI.beginTransaction(settings);
    SPI.transfer(&startFrame, NULL, 4, NULL);
    for (int i = 0; i<numpix; i++) SPI.transfer(buf, NULL, 4, NULL);
    // Send enough zero bytes to ensure all LEDs latch
    int endFrameBytes = (numpix / 8) + 1;
    for (int i = 0; i < endFrameBytes; i++) {
        SPI.transfer(0x00);
    }
    SPI.endTransaction();
}
