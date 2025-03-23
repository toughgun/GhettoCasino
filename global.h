#ifndef GLOBAL_H
#define GLOBAL_H
#include <GL/gl.h>
#include "image.h"

struct Texture {
    //menu background
    Image* backImage;
    GLuint backTexture;
    //menu logo
	Image* menuLogo;
	GLuint menulogotex;
	//button
	Image* buttonImage;
	GLuint buttontex;
    //
    float xc[2];
    float yc[2];
};

class Global {
public:
    int xres, yres;
    Texture tex;
    GLuint cupTexture;
    GLuint silhouetteTexture;  // Added here
    float cupWidth;
    float cupHeight;
    float cupPosX;
    float cupVelX;
    float cupRange;
    float w;
    float h;

    Global() {
        xres = 1280;
        yres = 720;
        cupTexture = 0;
        silhouetteTexture = 0; // Initialized
        cupWidth = 50.0f;
        cupHeight = 50.0f;
        cupPosX = 0.0f;
        cupVelX = 2.0f;
        cupRange = 50.0f;
    }
};

extern Global g;
#endif // GLOBAL_H
