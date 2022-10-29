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
GatorDot::GatorDot(unsigned int n = 0){
    brightness = 0xFF;
    numpix = n;
    settings = SPISettings(8*MHZ, MSBFIRST, SPI_MODE3);
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
    SPI.transfer(&endFrame, NULL, 4, NULL); // end frame
    SPI.endTransaction();
}

void GatorDot::clear(){
    uint8_t buf[] = {0xE0,0,0,0};
    SPI.beginTransaction(settings);
    SPI.transfer(&startFrame, NULL, 4, NULL);
    for (int i = 0; i<numpix; i++) SPI.transfer(buf, NULL, 4, NULL);
    SPI.transfer(&endFrame, NULL, 4, NULL);
    SPI.endTransaction();
}
