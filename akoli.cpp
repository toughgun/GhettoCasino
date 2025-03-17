// Phillip Lakes (Koli)
// CS 3350 Software Engineering
// Slot Machine
// 03/03/2025
// g++ main.cpp akoli.cpp -o slot -Wall -lX11 -lGLU -lm -lGL
// "There is only one good, knowledge, and one evil, ignorance." - Socrates
//
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "time.h"
#include "phil_funcs.h"
#include "image.h"
#include "reel.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <map>
#include <chrono>

#define TRUE  1
#define FALSE 0
// screen width and height
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT  768

std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

Surface *surface;

GLUquadricObj *quadratic;     // Storage object
GLuint cylinder_side_tex;
GLuint cylinder_spinner_tex;
GLuint reels_tex;

void calculate_framerate();
GLfloat get_rand( GLfloat max );
Surface* loadPNG(const char *fp);
bool initGLTexture(const char *name, GLuint *addr);
int loadGLTextures();
int initGL(GLvoid);
int resize(int width, int height);
void drawReels();
int draw(GLvoid);

void calculate_framerate() {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = current_time - last_time;
    last_time = current_time;
    float fps = 1.0f / elapsed.count();
    std::cout << "FPS: " << fps << std::endl;
}


Surface* loadPNG(const char *fp) {
	Image img = fp;
	surface = new Surface();
	surface->width = img.width;
	surface->height = img.height;
	surface->data = img.data;
	std::cout << "loadPNG(): " << fp << " (" << surface->width << "x" << surface->height << ")\n";
	return surface;
}

bool initGLTexture(const char *name, GLuint *addr) {
	Surface *surface;
	surface = loadPNG(name);
	glGenTextures(1, addr);
	glBindTexture(GL_TEXTURE_2D, *addr);
	// Stretch Property
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Texture Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->width, surface->height, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->data);
	return true;
}

int loadGLTextures() {
	initGLTexture("cyl_side_tex.png", &cylinder_side_tex);
	initGLTexture("cyl_spinner_tex.png", &cylinder_spinner_tex);
	initGLTexture("reels_tex.png", &reels_tex);
	
	// test
	//initGLTexture("reel_strip.png", &reel_strip);
	return 1;
}

int initGL(GLvoid) {
	if (loadGLTextures() == 1) {
		std::cout << "loadGLTextures() success\n";
	}
	//loadGLTextures();
	// background
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// depth
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth(1.5);
	// QoL stuff
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_TEXTURE_2D);
	// Quadratic Object
	quadratic = gluNewQuadric();
	gluQuadricDrawStyle(quadratic, GLU_FILL);
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	return 1;
}

int resize(int width, int height) {
	// width : height
	GLfloat ratio;
	if (height == 0) {
		height = 1;
	}
	ratio = GLfloat(width) / GLfloat(height);
	// Switch to projection matrix, set view
	glViewport(0, 0, (GLint)width, (GLint)height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	return 1;
}

void drawReels() {
	glEnable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glTranslatef(1.0f,-0.4f,-3.9);

	for (int x = 0; x < 3; ++x) {
		Reel *reel = reels[x];
		reels[x] = new Reel();
		reel->spin();
    }
	
	glColor4f( 128, 128, 128, 128 );
	// spinner rod
	glPushMatrix();
	    glRotatef( 90,0.0f,1.0f,0.0f);		            // Rotate On The Y Axis
 	    glTranslatef(0.0f,0.0f,-3.4f);	                // Center the cylinder 
	    glBindTexture( GL_TEXTURE_2D, cylinder_spinner_tex );
	    gluCylinder(quadratic,0.2f,0.2f,5.0f,9,3);      // Draw Our Cylinder 
	glPopMatrix();

	// right cylinder
	glPushMatrix();
	    glRotatef( reels[0]->degree,1.0f,0.0f,0.0f);	  // Rotate On The X Axis
	    glRotatef( 90,0.0f,1.0f,0.0f);		              // Rotate On The Y Axis
 	    glTranslatef(0.0f,0.0f,-0.20f);	                  // Center the cylinder 
	    glBindTexture( GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic,1.0f,1.0f,0.7f,9,3);        // Draw Our Cylinder 
	glPopMatrix();

	// center cylinder
	glPushMatrix();
	    glRotatef( reels[1]->degree, 1.0f,0.0f,0.0f);
	    glRotatef( 90,0.0f,1.0f,0.0f);
 	    glTranslatef( 0.0f, 0.0f,-1.38f ); 
	    glBindTexture( GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic, 1.0f, 1.0f, 0.7f, 9, 1);
	glPopMatrix();
	
	// left cylinder
	glPushMatrix();
	    glRotatef( reels[2]->degree,1.0f,0.0f,0.0f);
	    glRotatef( 90,0.0f,1.0f,0.0f);
	    glTranslatef( 0.0f,0.0f,-2.5f );
	    glBindTexture( GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic,1.0f,1.0f,0.6f,9,3);
	glPopMatrix();
	
}

int draw(GLvoid) {
	// Clear && Reset View
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	drawReels();

	return 1;
}
