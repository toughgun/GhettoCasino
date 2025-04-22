#ifndef GLOBAL_H
#define GLOBAL_H
#include <GL/gl.h>
#include "image.h"

struct Texture {
    // menu background
    Image* backImage;
    GLuint backTexture;
    // menu logo
    Image* menuLogo;
    GLuint menulogotex;
    float logo_xc[2];
    float logo_yc[2];
    // button
    Image* buttonImage;
    GLuint buttontex;
    // devmode
    Image* devImage;
    GLuint devtex;
    //Dice
    GLuint diceTex[6] = {0};
    Image* diceImage;
    // slot face
    Image* slotImage;
    GLuint slottex;
    float xc[2];
    float yc[2];
};

class Global {
public:
    int xres, yres;
    Texture tex;
    GLuint silhouetteTexture;
    int devmode;
    // Dice variables
    GLuint cupTexture;
    float cupWidth;
    float cupHeight;
    float cupPosX;
    float cupVelX;
    float cupRange;
    int vsync;
    int winstreak;
    //
    float w;
    float h;
    bool exec;
    // Betting variables
    int currency;      // Starting currency (chips)
    int currentBet;    // Current bet amount

    Global() {
        devmode = 0;
        xres = 1280;
        yres = 720;
        cupTexture = 0;
        silhouetteTexture = 0; // Initialized
        //Dice
        cupWidth = 50.0f;
        cupHeight = 50.0f;
        cupPosX = 0.0f;
        cupVelX = 2.0f;
        cupRange = 50.0f;
        winstreak = 0;
        vsync = 0;
        //
        exec = true;
        currency = 1000;     // Start with 1000 chips
        currentBet = 0;      // Initial bet is 0
    }
};

extern Global g;
#endif // GLOBAL_H
