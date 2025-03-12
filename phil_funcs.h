// Phillip Lakes (Koli)
// CS 3350 Software Engineering
// Image class by Gordon Griesel

#ifndef PHIL_FUNCS_H
#define PHIL_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "time.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <map>
#include <chrono>

class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		//printf("fname **%s**\n", fname);
		int ppmFlag = 0;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		char ppmname[80];
		if (strncmp(name+(slen-4), ".ppm", 4) == 0)
			ppmFlag = 1;
		if (ppmFlag) {
			strcpy(ppmname, name);
		} else {
			name[slen-4] = '\0';
			//printf("name **%s**\n", name);
			sprintf(ppmname,"%s.ppm", name);
			//printf("ppmname **%s**\n", ppmname);
			char ts[100];
			//system("convert eball.jpg eball.ppm");
			sprintf(ts, "convert %s %s", fname, ppmname);
			system(ts);
		}
		//sprintf(ts, "%s", name);
		FILE *fpi = fopen(ppmname, "r");
		if (fpi) {
			char line[200];
			fgets(line, 200, fpi);
			fgets(line, 200, fpi);
			//skip comments and blank lines
			while (line[0] == '#' || strlen(line) < 2)
				fgets(line, 200, fpi);
			sscanf(line, "%i %i", &width, &height);
			fgets(line, 200, fpi);
			//get pixel data
			int n = width * height * 3;			
			data = new unsigned char[n];			
			for (int i=0; i<n; i++)
				data[i] = fgetc(fpi);
			fclose(fpi);
		} else {
			printf("ERROR opening image: %s\n",ppmname);
			exit(0);
		}
		if (!ppmFlag)
			unlink(ppmname);
	}
};

/*Image img[4] = {
"./cyl_side_tex.png",
"./cyl_spinner_tex.png",
"./reels_tex.png"};
*/

// Attempt to instantiate Surface
struct Surface {
    unsigned int width;
    unsigned int height;
    unsigned char* data;
};

struct Reel;
static Reel* reels[3];
static int gluttony = -1;

//void calculate_framerate();
GLfloat get_rand( GLfloat max );
//Surface* loadPNG(const char *fp);
//bool initGLTexture(const char *name, GLuint *addr);
//int loadGLTextures();
int initGL(GLvoid);
//int resize(int width, int height);
//void drawReels();
int draw(GLvoid);

struct Reel {
	GLfloat speed;
	GLfloat degree;
	bool stopped;
	GLfloat begin;

    void start() {
		stopped=false;
		speed=get_rand(2);
		degree=get_rand(360);
		begin=0;
		begin=speed;
	}

	static void stop() {
		int r=0;
		for ( int x=0; x<3; ++x ){
			if ( reels[x]->speed < reels[r]->speed ){
				r=x;
			}
		}
		reels[r]->stopped = true;
	}

	static int winner() {
		int win = reels[0]->position();
		for ( int x=1; x<3; ++x ){
			if ( win != reels[x]->position() ){
				return -1;
			}
		}
		return win;
	}

	static int num_stopped() {
		int num = 0;
		for ( int x=0; x<3; ++x ){
			if ( reels[x]->stopped ){
				++num;
			}
		}
		return num;
	}
	
	static bool all_stopped() {
		return num_stopped() == 3;
	}

	int position() {
		return (int)roundf((degree-30)/40);
	}

	bool at_stop_position() {
		return ( (GLint)roundf( degree+10 ) % 40 == 0 );
	}

	static int stop_position(Reel *reel) {
		int position = reel->position();
		if (gluttony != -1) {
			return gluttony;
		}
		std::cout << "Results: " << Reel::num_stopped() << std::endl;
		switch (Reel::num_stopped()) {
		case 0:
			return position;
			break;
		case 1:
			for (int x=0; x<3; ++x) {
				if (reels[x]->stopped) {
					return reels[x]->position();

				}
			}
			return position;
			break;
		default:
			int reel_stopped;
			for (int x=0; x<3; ++x) {
				if (reels[x]->stopped) {
					reel_stopped = reels[x]->position();
				}
			}
			std::cout << "Second Results: " << reel_stopped << std::endl;

			// Does the house always win?

			int w = (int)get_rand(3);
			std::cout << "Final Reel FTW?: " << (w==1) << std::endl;
			if (w==1) {
				return gluttony = reel_stopped;
			} else {
				// w pos
				if (position == reel_stopped) {
					if (++position > 9) {
						position = 0;
					}
				}
				return gluttony=position;

			}
			
		}

	}

	void spin() {
	// 'this' is individual reel
		if(this->stopped) {
			return;
		}
		if (speed < 0.2) {
			if (this->at_stop_position()) {
				if (Reel::stop_position(this) == this->position()) {
					stopped = true;
					std::cout << "Reel halts on: " << this->position() << std::endl;
					if (Reel::all_stopped()) {
						int winner = Reel::winner();
						std::cout << "Winner: 	" << winner << std::endl;
					}

				}
			}
		} else {
			speed -= 0.001;

		}
		degree += speed;
		if (degree > 360) {
			degree -= 360;
		}
	}

	Reel() {
			// start();
	}

};

#endif
