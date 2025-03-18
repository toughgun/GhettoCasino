#ifndef GLOBAL_H
#define GLOBAL_H

#include "texture.h"
#include "button.h"

class Global {
public:
	int xres, yres;
	Texture tex;
	Global();
};
extern Global g;
#endif