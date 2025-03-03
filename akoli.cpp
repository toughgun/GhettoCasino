// Phillip Lakes (Koli)
// CS 3350 Software Engineering
//
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

using namespace std;

#define TRUE  1
#define FALSE 0
// screen width and height
#define SCREEN_WIDTH  1024
#define SCREEN_HEIGHT  768

std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();

GLUquadricObj *quadratic;     // Storage object
GLuint cylinder_side_tex;
GLuint cylinder_spinner_tex;
GLuint reels_tex;

struct Reel;
static Reel* reels[3];

struct Reel {
    GLfloat speed;
	GLfloat degree;
	bool stopped;
	GLfloat begin;
    Reel() {

    }
};

void calculate_framerate() {
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = current_time - last_time;
    last_time = current_time;
    float fps = 1.0f / elapsed.count();
    cout << "FPS: " << fps << endl;
}

void drawReels() {
	glEnable( GL_LIGHTING );
	glDisable( GL_BLEND );
	glTranslatef(1.0f,-0.4f,-3.9);

	for (int x=0;x<3;++x){
	    //Reel *reel = reels[x];
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

// int main(int argc, char *argv[] ) {

//   return 0;
// }
