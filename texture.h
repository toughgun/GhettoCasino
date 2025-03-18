#ifndef TEXTURE_H
#define TEXTURE_H

#include "image.h"
class Texture {
public:
	//button
	Image *backImage;
	GLuint buttontex;
	//menu background
	Image *buttonImage;
	GLuint backTexture;
	//hover over button
	Image *buttonImageHover;
	GLuint buttontexHover;
	
	float xc[2];
	float yc[2];
};
#endif