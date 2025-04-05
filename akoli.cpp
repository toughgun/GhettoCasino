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
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "time.h"
#include "phil_funcs.h"
#include "image.h"
#include "reel.h"
#include "global.h"
#include "x11.h"
#include <vector>
#include <iostream>
#include <math.h>
#include <fstream>
#include <map>
#include <chrono>

std::chrono::time_point<std::chrono::high_resolution_clock> 
last_time = std::chrono::high_resolution_clock::now();

Surface *surface;

GLUquadricObj *quadratic;     // Storage object
GLuint cylinder_side_tex;
GLuint cylinder_spinner_tex;
GLuint reels_tex;

GLfloat L_Ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat L_Diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat L_Pos[] = { 0.0f, 0.0f, 2.0f, 1.0f };

void calculate_framerate();
void quit(int retcode);
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

void quit(int retcode) {
	gluDeleteQuadric(quadratic);
    glDeleteTextures(1, &cylinder_side_tex);
	glDeleteTextures(1, &cylinder_spinner_tex);
	glDeleteTextures(1, &reels_tex);
	exit(retcode);
}

Surface* loadPNG(const char *fp) {
    Image img(fp); // Properly initialize the Image object with the file path
    if (!img.data) { // Check if the image data is valid
        std::cerr << "Error: Failed to load image: " << fp << std::endl;
        return nullptr;
    }
    surface = new Surface();
    surface->width = img.width;
    surface->height = img.height;
	// Allocate and copy due to Segmentation Fault for large dimensions?
    surface->data = new unsigned char[img.width * img.height * 3];
    std::memcpy(surface->data, img.data, img.width * img.height * 3);
    std::cout << "loadPNG(): " << fp << " (" << surface->width << "x" << surface->height << ")\n";
	if ( !surface ) {
	    fprintf( stderr,  "No Surface! \n");
	    quit(1);
	}
    return surface;
}

// reel.cpp
static int gluttony = -1;
struct Reel;
Reel* reels[3];

GLfloat get_rand( GLfloat max ){
	return ( 1 + (float) ( max * (rand() / (RAND_MAX + 1.0))) );
}

Reel::Reel() {
    //start();
}

void Reel::start() {
    stopped = false;
    speed = get_rand(2);
    degree = get_rand(360);
    begin = 0;
    begin = speed;
}

void Reel::stop() {
    int r = 0;
    for (int x = 0; x < 3; ++x) {
        if (reels[x]->speed < reels[r]->speed) {
            r = x;
        }
    }
    reels[r]->stopped = true;
}

int Reel::winner() {
    int win = reels[0]->position();
    for (int x = 1; x < 3; ++x) {
        if (win != reels[x]->position()) {
            return -1;
        }
    }
    return win;
}

int Reel::num_stopped() {
    int num = 0;
    for (int x = 0; x < 3; ++x) {
        if (reels[x]->stopped) {
            ++num;
        }
    }
    return num;
}

bool Reel::all_stopped() {
    return num_stopped() == 3;
}

int Reel::position() {
    return static_cast<int>(roundf((degree - 30) / 40));
}

bool Reel::at_stop_position() {
    return (static_cast<GLint>(roundf(degree + 10)) % 40 == 0);
}

int Reel::stop_position(Reel *reel) {
    int position = reel->position();
    if (gluttony != -1) {
        return gluttony;
    }
    std::cout << "Results: " << Reel::num_stopped() << std::endl;
    switch (Reel::num_stopped()) {
        case 0:
            return position;
        case 1:
            for (int x = 0; x < 3; ++x) {
                if (reels[x]->stopped) {
                    return reels[x]->position();
                }
            }
            return position;
        default:
            int reel_stopped;
            for (int x = 0; x < 3; ++x) {
                if (reels[x]->stopped) {
                    reel_stopped = reels[x]->position();
                }
            }
            std::cout << "Second Results: " << reel_stopped << std::endl;

            int w = static_cast<int>(get_rand(3));
            std::cout << "Final Reel FTW?: " << (w == 1) << std::endl;
            if (w == 1) {
                return gluttony = reel_stopped;
            } else {
                if (position == reel_stopped) {
                    if (++position > 9) {
                        position = 0;
                    }
                }
                return gluttony = position;
            }
    }
}

void Reel::spin() {
    if (this->stopped) {
        return;
    }
    if (speed < 0.2) {
        if (this->at_stop_position()) {
            if (Reel::stop_position(this) == this->position()) {
                stopped = true;
                std::cout << "Reel halts on: " << this->position() << std::endl;
                if (Reel::all_stopped()) {
                    int winner = Reel::winner();
                    std::cout << "Winner: " << winner << std::endl;
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


///

bool initGLTexture(const char *name, GLuint *addr) {
	//Surface *surface;
	surface = loadPNG(name);
	glGenTextures(1, addr);
	glBindTexture(GL_TEXTURE_2D, *addr);
	// Stretch Property
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Texture Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, 3, surface->width, surface->height, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->data);
	return true;
}

int loadGLTextures() {
	if (initGLTexture("cyl_side_tex.png", &cylinder_side_tex));
	initGLTexture("cyl_spinner_tex.png", &cylinder_spinner_tex);
	initGLTexture("reels_tex.png", &reels_tex);

	return 1;
}



int initGL(GLvoid) {
	if (loadGLTextures() == 1) {
		std::cout << "loadGLTextures() success\n";
	}
	//loadGLTextures();
	// background
	glShadeModel(GL_SMOOTH);
	glClearColor(0.1f, 0.25f, 0.25f, 1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	// Let there be light
    glLightfv( GL_LIGHT1, GL_AMBIENT, L_Ambient );

    glLightfv( GL_LIGHT1, GL_DIFFUSE, L_Diffuse );

    glLightfv( GL_LIGHT1, GL_POSITION, L_Pos );

    glEnable( GL_LIGHT1 );
	///

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
	glEnable( GL_BLEND );
	glTranslatef(1.0f,-0.4f,-3.9);
	
	for (int x = 0; x < 3; ++x) {
		Reel *reel = reels[x];
		//reels[x] = new Reel();
		reel->spin();
		//std::cout << x << " " << reels[x] << std::endl;
	}

	//glColor4f( 128, 128, 128, 128 );
	glColor4f( 0.5f, 0.5f, 0.5f, 0.5f );
	// spinner rod
	glPushMatrix();
	    glRotatef(90,0.0f,1.0f,0.0f);		            // Rotate On The Y Axis
 	    glTranslatef(0.0f,0.0f,-3.4f);	                // Center the cylinder 
	    glBindTexture( GL_TEXTURE_2D, cylinder_spinner_tex );
	    gluCylinder(quadratic,0.2f,0.2f,5.0f,9,3);      // Draw Our Cylinder 
	glPopMatrix();

	// right cylinder mask
	glPushMatrix();
	    glRotatef(reels[0]->degree,1.0f,0.0f,0.0f);
	    glRotatef( 90,0.0f,1.0f,0.0f);
 	    glTranslatef(0.0f,0.0f,-0.20f);
	    glBindTexture( GL_TEXTURE_2D, cylinder_side_tex );
	    gluDisk(quadratic,0.2f,1.0f,9,3);             // Draw A Disc
	glPopMatrix();

	// right cylinder
	glPushMatrix();
	    glRotatef(reels[0]->degree,1.0f,0.0f,0.0f);	  // Rotate On The X Axis
	    glRotatef(90,0.0f,1.0f,0.0f);		              // Rotate On The Y Axis
 	    glTranslatef(0.0f,0.0f,-0.20f);	                  // Center the cylinder 
	    glBindTexture( GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic,1.0f,1.0f,0.7f,9,3);        // Draw Our Cylinder 
	glPopMatrix();

	// center cylinder
	glPushMatrix();
	    glRotatef(reels[1]->degree, 1.0f,0.0f,0.0f);
	    glRotatef(90,0.0f,1.0f,0.0f);
 	    glTranslatef(0.0f, 0.0f,-1.38f ); 
	    glBindTexture(GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic, 1.0f, 1.0f, 0.7f, 9, 1);
	glPopMatrix();

	glPushMatrix();
	    glRotatef(reels[2]->degree,1.0f,0.0f,0.0f);
	    glRotatef( 90,0.0f,1.0f,0.0f);
	    glTranslatef( 0.0f,0.0f,-1.9f );
	    glBindTexture( GL_TEXTURE_2D, cylinder_side_tex );
	    gluDisk(quadratic,0.2f,1.0f,9,3);             // Draw A Disc
	glPopMatrix();
	
	// left cylinder
	glPushMatrix();
	    glRotatef(reels[2]->degree,1.0f,0.0f,0.0f);
	    glRotatef(90,0.0f,1.0f,0.0f);
	    glTranslatef(0.0f,0.0f,-2.5f );
	    glBindTexture(GL_TEXTURE_2D, reels_tex );
	    gluCylinder(quadratic,1.0f,1.0f,0.6f,9,3);
	glPopMatrix();
	
}

X11_wrapper::~X11_wrapper() {
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper() {
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	int w = g.xres, h = g.yres;
	dpy = XOpenDisplay(NULL);
	if (dpy == NULL) {
		std::cout << "\n\tcannot connect to X server\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Window root = DefaultRootWindow(dpy);
	XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
	if (vi == NULL) {
		std::cout << "\n\tno appropriate visual found\n" << std::endl;
		exit(EXIT_FAILURE);
	}
	Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
	XSetWindowAttributes swa;
	swa.colormap = cmap;
	swa.event_mask =
		ExposureMask | KeyPressMask | KeyReleaseMask |
		ButtonPress | ButtonReleaseMask |
		PointerMotionMask |
		StructureNotifyMask | SubstructureNotifyMask;
	win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
		InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	set_title();
	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::check_mouse(XEvent *e) {
	static int savex = 0;
	static int savey = 0;

	//Weed out non-mouse events
	if (e->type != ButtonRelease &&
		e->type != ButtonPress &&
		e->type != MotionNotify) {
		//This is not a mouse event that we care about.
		return;
	}
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button was pressed.
			//int y = g.yres - e->xbutton.y;
			return;
		}
		if (e->xbutton.button==3) {
			//Right button was pressed.
			return;
		}
	}
	if (e->type == MotionNotify) {
		//The mouse moved!
		if (savex != e->xbutton.x || savey != e->xbutton.y) {
			savex = e->xbutton.x;
			savey = e->xbutton.y;
			//Code placed here will execute whenever the mouse moves.


		}
	}
}


int X11_wrapper::check_keys(XEvent *e) {
	if (e->type != KeyPress && e->type != KeyRelease)
		return 0;
	int key = XLookupKeysym(&e->xkey, 0);
	if (e->type == KeyPress) {
		switch (key) {
			case XK_Escape:
				g.exec = false;
				break;
			case XK_space:
				for (int i = 0; i < 3; ++i) {
					reels[i]->start();
				}
				break;
			default:
				break;
		}
	}
	return 0;
}


void X11_wrapper::set_title() {
	//Set the window title bar.
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "Ghetto Casino - Slot Machine");
}

bool X11_wrapper::getXPending() {
	//See if there are pending events.
	return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent() {
	//Get a pending event.
	XEvent e;
	XNextEvent(dpy, &e);
	return e;
}

void X11_wrapper::swapBuffers() {
	glXSwapBuffers(dpy, win);
}
///

int draw(GLvoid) {
	// Clear && Reset View
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	/*
	// Set up the camera
	gluLookAt(0.0f, 0.0f, 5.0f,  // Camera position
	          0.0f, 0.0f, 0.0f,  // Look-at point
	          0.0f, 1.0f, 0.0f); // Up vector
	*/

	// Draw the reels
	drawReels();
	GLenum err = glGetError();
		if (err != GL_NO_ERROR) {
    		std::cerr << "OpenGL Error: " << gluErrorString(err) << std::endl;
		}
	/*
	if (Reel::all_stopped() && Reel::winner() != -1) {
		std::cout << "```Behold the Ghetto Kingpin!```" << std::endl;
	}
	*/
	
	return 1;
}
