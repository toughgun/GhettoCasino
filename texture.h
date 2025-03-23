#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.h"
class Texture {
public:
	//button
	Image *buttonImage;
	GLuint buttontex;
	//menu background
	Image *backImage;
	GLuint backTexture;
	//menu logo
	Image *menuLogo;
	GLuint menulogotex;
	float xc[2];
	float yc[2];
};
#endif