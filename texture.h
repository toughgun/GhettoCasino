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
	//button Slots
	Image *bSlots;
	GLuint bSlotstex;
	//button Dice
	Image *bDice;
	GLuint bDicetex;
	//button Black Jack
	Image *bBlackjack;
	GLuint bBlackjacktex;
	//button exit
	Image *bExit;
	GLuint bExittex;
	
	float xc[2];
	float yc[2];
};
#endif