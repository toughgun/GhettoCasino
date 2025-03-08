#ifndef PHIL_FUNCS_H
#define PHIL_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "time.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <map>
#include <chrono>

struct Surface {
    unsigned int width;
    unsigned int height;
    unsigned char* data;
};

struct Reel;
static Reel* reels[3];

void calculate_framerate();
GLfloat get_rand( GLfloat max );
Surface* loadBMP(const char *fp);
bool initGLTexture(const char *name, GLuint *addr);
int loadGLTextures();
int initGL(GLvoid);
int resize(int width, int height);
void drawReels();
int draw(GLvoid);

struct Reel {
	GLfloat speed;
	GLfloat degree;
	bool stopped;
	GLfloat begin;

    void start() {
		stopped=false;
		speed=get_rand( 2 );
		degree=get_rand( 360 );
		begin=0;
		begin=speed;
	}

	static void stop(){
		int r=0;
		for ( int x=0; x<3; ++x ){
			if ( reels[x]->speed < reels[r]->speed ){
				r=x;
			}
		}
		reels[r]->stopped = true;
	}

	static int winner(){
		int win = reels[0]->position();
		for ( int x=1; x<3; ++x ){
			if ( win != reels[x]->position() ){
				return -1;
			}
		}
		return win;
	}

	static int num_stopped(){
		int num = 0;
		for ( int x=0; x<3; ++x ){
			if ( reels[x]->stopped ){
				++num;
			}
		}
		return num;
	}
	
	static bool all_stopped(){
		return num_stopped() == 3;
	}

	Reel()
		{
			//start();
		}

	int position(){
		return (int)roundf((degree-30)/40);
	}

	bool at_stop_position(){
		return ( (GLint)roundf( degree+10 ) % 40 == 0 );
	}
};

#endif
