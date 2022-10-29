// ©2020 GatorbotLLC 
// DotStar Test
// My intent is to create my own SPI-based dotstar class
#include "Particle.h"

struct jColor {
    uint8_t red;
    uint8_t grn;
    uint8_t blu;
};


struct PixelBlock {
    unsigned int length;
    jColor* pixels;
    // Methods
    PixelBlock(unsigned int l);
    ~PixelBlock();
};

// GATORDOT
//
struct GatorDot {
    // Members
    uint8_t brightness;
    unsigned int numpix;
    SPISettings settings;
    
    // Constants
    uint32_t startFrame = 0;
    uint32_t endFrame = 0;
    uint8_t bMask = 0xE0;
    
    // Methods
    GatorDot(unsigned int n);
    void begin();
    //void fill(uint32_t clr);
    void fill(PixelBlock* pxls);
    void clear();
};
