//
//program: background.cpp --> main.cpp
//author:  Gordon Griesel
//date:    2017 - 2018
//
//The position of the background QUAD does not change.
//Just the texture coordinates change.
//In this example, only the x coordinates change.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
#include "crodriguez4.h"
#include "image.h"
#include "global.h"
#include "phil_funcs.h"

#include <iostream>
using namespace std;

/*class Image {
public:
	int width, height;
	unsigned char *data;
	~Image() { delete [] data; }
	Image(const char *fname) {
		if (fname[0] == '\0')
			return;
		char name[40];
		strcpy(name, fname);
		int slen = strlen(name);
		name[slen-4] = '\0';
		char ppmname[80];
		sprintf(ppmname,"%s.ppm", name);
		char ts[100];
		sprintf(ts, "convert %s %s", fname, ppmname);
		system(ts);
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
			printf("ERROR opening image: %s\n", ppmname);
			exit(0);
		}
		unlink(ppmname);
	}
};*/

Image img[1] = {"menu_bg.png"};

Global g;

// class Texture {
// public:
// 	Image *backImage;
// 	GLuint backTexture;
// 	float xc[2];
// 	float yc[2];

// };

// class Global {
// public:
// 	int xres, yres;
// 	Texture tex;
// 	Global() {
// 		xres=1280, yres=720;
// 	}
// } g;

class X11_wrapper {
private:
	Display *dpy;
	Window win;
	GLXContext glc;
public:
	X11_wrapper() {
		GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
		//GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
		setup_screen_res(1280, 720);
		dpy = XOpenDisplay(NULL);
		if(dpy == NULL) {
			printf("\n\tcannot connect to X server\n\n");
			exit(EXIT_FAILURE);
		}
		Window root = DefaultRootWindow(dpy);
		XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
		if(vi == NULL) {
			printf("\n\tno appropriate visual found\n\n");
			exit(EXIT_FAILURE);
		} 
		Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
		XSetWindowAttributes swa;
		swa.colormap = cmap;
		swa.event_mask =
			ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask |
			ButtonPressMask | ButtonReleaseMask |
			StructureNotifyMask | SubstructureNotifyMask;
		win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0,
								vi->depth, InputOutput, vi->visual,
								CWColormap | CWEventMask, &swa);
		set_title();
		glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
		glXMakeCurrent(dpy, win, glc);
	}
	void cleanupXWindows() {
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	void setup_screen_res(const int w, const int h) {
		g.xres = w;
		g.yres = h;
	}
	void reshape_window(int width, int height) {
		//window has been resized.
		setup_screen_res(width, height);
		glViewport(0, 0, (GLint)width, (GLint)height);
		glMatrixMode(GL_PROJECTION); glLoadIdentity();
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
		glOrtho(0, g.xres, 0, g.yres, -1, 1);
		set_title();
	}
	void set_title() {
		//Set the window title bar.
		XMapWindow(dpy, win);
		XStoreName(dpy, win, "Ghetto Casino");
	}
	bool getXPending() {
		return XPending(dpy);
	}
	XEvent getXNextEvent() {
		XEvent e;
		XNextEvent(dpy, &e);
		return e;
	}
	void swapBuffers() {
		glXSwapBuffers(dpy, win);
	}
	void check_resize(XEvent *e) {
		//The ConfigureNotify is sent by the
		//server if the window is resized.
		if (e->type != ConfigureNotify)
			return;
		XConfigureEvent xce = e->xconfigure;
		if (xce.width != g.xres || xce.height != g.yres) {
			//Window size did change.
			reshape_window(xce.width, xce.height);
		}
	}
} x11;

void init_opengl(void);
void check_mouse(XEvent *e);
int check_keys(XEvent *e);
void physics(void);
void render(void);

//global variable
int gameState=0;

//===========================================================================
//===========================================================================
int main() {
    printf("```Welcome to the Ghetto```\n");
//===================================================
init_opengl();
while (gameState != 1) {
	while (x11.getXPending()) {
		XEvent e = x11.getXNextEvent();
		x11.check_resize(&e);
		check_mouse(&e);
		check_keys(&e);
	}
	physics();
	render();
	x11.swapBuffers();
	}
	return 0;
}

void init_opengl(void)
{
	glViewport(0, 0, g.xres, g.yres);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    // Load the cup texture.
    loadCupTexture();

	g.tex.backImage = &img[0];
	//create menu background
	glGenTextures(1, &g.tex.backTexture);
	int w = ((Image*)g.tex.backImage)->width;
	int h = ((Image*)g.tex.backImage)->height;
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0,
			GL_RGB, GL_UNSIGNED_BYTE, ((Image*)g.tex.backImage)->data);
	g.tex.xc[0] = 0.0f;
	g.tex.xc[1] = 1.0f;
	g.tex.yc[0] = 0.0f;
	g.tex.yc[1] = 1.0f;	
	
}

unsigned char *buildAlphaData(Image *img)
{
	int i, a, b, c;
	unsigned char *newdata, *ptr;
	unsigned char *data = (unsigned char *)img->data;
	newdata = (unsigned char *)malloc(img->width * img->height * 4);
	ptr = newdata;
	for (i = 0; i < img->width * img->height * 3; i += 4) {
		a = *(data+0);
		b = *(data+1);
		c = *(data+2);
		*(ptr+0) = a;
		*(ptr+1) = b;
		*(ptr+2) = c;

		*(ptr+3) = (a|b|c);
		ptr += 4;
		data += 3;
	}
	return newdata;
}
	
void check_mouse(XEvent *e)
{
	//Did the mouse move?
	//Was a mouse button clicked?
	static int savex = 0;
	static int savey = 0;
	//
	if (e->type == ButtonRelease) {
		return;
	}
	if (e->type == ButtonPress) {
		if (e->xbutton.button==1) {
			//Left button is down
		}
		if (e->xbutton.button==3) {
			//Right button is down
		}
	}
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
		//Mouse moved
		savex = e->xbutton.x;
		savey = e->xbutton.y;
	}
}
// Inside of crodriguez.cpp
// extern void show_credits();

int check_keys(XEvent *e) {
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);

        if (key == XK_Escape) {
            return gameState = 1; // exit
        }
        if (gameState == 3) {
            if (key == XK_space) {
                roll_dice();
            }
            if (key == XK_u || key == XK_U) {
                playerChoice = UNDER;
                cout << "[INPUT] Player chose UNDER.\n";
            }
            if (key == XK_o || key == XK_O) {
                playerChoice = OVER;
                cout << "[INPUT] Player chose OVER.\n";
            }
            if (key == XK_e || key == XK_E) {
                playerChoice = EXACT;
                cout << "[INPUT] Player chose EXACT.\n";
            }
            if (key == XK_r || key == XK_R) {
				if (playerChoice != NONE)
                	reveal_dice();
				else
					printf("Make a choice first\n");
            }
        }
        if (key == XK_c) {
            show_credits();
        }
        if (key == XK_m) {
            cout << "gameState = 0\n";
            return gameState = 0;
        }
        if (key == XK_s) {
            cout << "gameState = 2\n";
            return gameState = 2;
        }
        if (key == XK_d) {
            cout << "gameState = 3\n";
            return gameState = 3;
        }
        if (key == XK_b) {
            cout << "gameState = 4\n";
            return gameState = 4;
        }
    }
    return 0;
}

void physics()
{
	if (gameState == 3) {
        g.cupPosX += g.cupVelX;
        if (g.cupPosX > g.cupRange || g.cupPosX < -g.cupRange) {
            g.cupVelX = -g.cupVelX;
        }
    }
}

void drawBackground() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[1]); glVertex2i(0,      0);
        glTexCoord2f(g.tex.xc[0], g.tex.yc[0]); glVertex2i(0,      g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
        glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
    glEnd();
}

void drawMenuOptions()
{




}

void render() {
    if (gameState == 0) {
        drawBackground();
        drawMenuOptions();
    } else if (gameState == 2) {
        cout << "[STATE] Slot selected.\n";
        render_slots();
    } else if (gameState == 3) {      
		drawBackground();
        render_dice();
    } else if (gameState == 4) {
        cout << "[STATE] Blackjack selected.\n";
        render_blackjack();
    }
}

