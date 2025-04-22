//
// program: background.cpp --> main.cpp
// author:  Gordon Griesel
// date:    2017 - 2018
//
// The position of the background QUAD does not change.
// Just the texture coordinates change.
// In this example, only the x coordinates change.
//
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <AL/al.h>
#include "blackjack.h"
#include "bolayvar.h"
#include "button.h"
#include "crodriguez4.h"
#include "fonts.h"
#include "global.h"
#include "hchen_functions.h"
#include "image.h"
#include "phil_funcs.h"
#include <iostream>

using namespace std;

// macros
#define MakeVector(x, y, z, v) (v)[0] = (x), (v)[1] = (y), (v)[2] = (z)

Global g;

Image img[6] = {"menu_bg.png",   "menu_button.png",
                "logo.png",      "menu_bg_devscreen.png",
                "slot_face.png", "blackjacktable.png"};

class X11_wrapper {
  private:
    Display*   dpy;
    Window     win;
    GLXContext glc;

  public:
    X11_wrapper()
    {
        GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
        // GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, None };
        setup_screen_res(1280, 720);
        dpy = XOpenDisplay(NULL);
        if (dpy == NULL) {
            printf("\n\tcannot connect to X server\n\n");
            exit(EXIT_FAILURE);
        }
        Window       root = DefaultRootWindow(dpy);
        XVisualInfo* vi   = glXChooseVisual(dpy, 0, att);
        if (vi == NULL) {
            printf("\n\tno appropriate visual found\n\n");
            exit(EXIT_FAILURE);
        }
        Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
        XSetWindowAttributes swa;
        swa.colormap   = cmap;
        swa.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                         PointerMotionMask | ButtonPressMask |
                         ButtonReleaseMask | StructureNotifyMask |
                         SubstructureNotifyMask;
        win = XCreateWindow(dpy, root, 0, 0, g.xres, g.yres, 0, vi->depth,
                            InputOutput, vi->visual, CWColormap | CWEventMask,
                            &swa);
        set_title();
        glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
        glXMakeCurrent(dpy, win, glc);
    }
    void cleanupXWindows()
    {
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
    }
    void setup_screen_res(const int w, const int h)
    {
        g.xres = w;
        g.yres = h;
    }
    void reshape_window(int width, int height)
    {
        // window has been resized.
        setup_screen_res(width, height);
        glViewport(0, 0, (GLint)width, (GLint)height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glOrtho(0, g.xres, 0, g.yres, -1, 1);
        set_title();
    }
    void set_title()
    {
        // Set the window title bar.
        XMapWindow(dpy, win);
        XStoreName(dpy, win, "Ghetto Casino");
    }
    bool   getXPending() { return XPending(dpy); }
    XEvent getXNextEvent()
    {
        XEvent e;
        XNextEvent(dpy, &e);
        return e;
    }
    void swapBuffers() { glXSwapBuffers(dpy, win); }
    void check_resize(XEvent* e)
    {
        // The ConfigureNotify is sent by the
        // server if the window is resized.
        if (e->type != ConfigureNotify)
            return;
        XConfigureEvent xce = e->xconfigure;
        if (xce.width != g.xres || xce.height != g.yres) {
            // Window size did change.
            reshape_window(xce.width, xce.height);
        }
    }
} x11;

void init_opengl(void);
void check_mouse(XEvent* e);
int  check_keys(XEvent* e);
void physics(void);
void render(void);
// global variable
int gameState     = 0;
int mouseposition = 0;
int kill          = 0;
//===========================================================================
//===========================================================================
int main()
{
    printf("```Welcome to the Ghetto```\n");
    //===================================================
    init_opengl();
    while (gameState != 1) {
        while (x11.getXPending()) {
            XEvent e = x11.getXNextEvent();
            x11.check_resize(&e);
            check_mouse(&e);
            kill = check_keys(&e);
        }
        physics();
        render();
        x11.swapBuffers();
        gameState = kill;
    }
    return 0;
}

unsigned char* buildAlphaData(Image* img)
{
    // Add 4th component to an RGB stream...
    // RGBA
    // When you do this, OpenGL is able to use the A component to determine
    // transparency information.
    // It is used in this application to erase parts of a texture-map from view.
    int            i;
    int            a, b, c;
    unsigned char *newdata, *ptr;
    unsigned char* data = (unsigned char*)img->data;
    newdata             = (unsigned char*)malloc(img->width * img->height * 4);
    ptr                 = newdata;
    for (i = 0; i < img->width * img->height * 3; i += 3) {
        a          = *(data + 0);
        b          = *(data + 1);
        c          = *(data + 2);
        *(ptr + 0) = a;
        *(ptr + 1) = b;
        *(ptr + 2) = c;
        //-----------------------------------------------
        // get largest color component...
        //*(ptr+3) = (unsigned char)((
        //		(int)*(ptr+0) +
        //		(int)*(ptr+1) +
        //		(int)*(ptr+2)) / 3);
        // d = a;
        // if (b >= a && b >= c) d = b;
        // if (c >= a && c >= b) d = c;
        //*(ptr+3) = d;
        //-----------------------------------------------
        // this code optimizes the commented code above.
        // code contributed by student: Chris Smith
        // Using bitwise OR makes black (0,0,0) yield 0 in alpha.
        *(ptr + 3) = (a | b | c);
        //-----------------------------------------------
        ptr += 4;
        data += 3;
    }
    return newdata;
}

void init_opengl(void)
{
    glViewport(0, 0, g.xres, g.yres);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);
    
    // Load the cup texture.
    loadCupTexture();
    // load Dice texture
	// loadDiceTextures();

    // init the position of the menu buttons and logo
    // buttons spaced out by 87 pixels
    MakeVector(g.xres / 2, 575, 0, menulogo.pos);              // logo
    MakeVector(g.xres / 2, g.yres / 2, 0, dev.pos);            // slots
    MakeVector(g.xres / 2, 430, 0, bslot.pos);                 // slots
    MakeVector(g.xres / 2, 430 - 87, 0, bdice.pos);            // dice
    MakeVector(g.xres / 2, 430 - (87 * 2), 0, bblackjack.pos); // blackjack
    MakeVector(g.xres / 2, 430 - (87 * 3), 0, bexit.pos);      // exit
    //
    g.tex.backImage   = &img[0];
    g.tex.buttonImage = &img[1];
    g.tex.menuLogo    = &img[2];
    g.tex.devImage    = &img[3];
    g.tex.slotImage   = &img[4];
    g.tex.bjImage     = &img[5];
    //
    // create menu logo
    glGenTextures(1, &g.tex.menulogotex);
    int w = g.tex.menuLogo->width;
    int h = g.tex.menuLogo->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.menulogotex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char* lgo = buildAlphaData(&img[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 lgo);
    free(lgo);
    //
    // create menu background
    glGenTextures(1, &g.tex.backTexture);
    w = g.tex.backImage->width;
    h = g.tex.backImage->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 g.tex.backImage->data);
    g.tex.xc[0] = 1.0;
    g.tex.yc[1] = 1.0;
    //
    // create menu button
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    w = g.tex.buttonImage->width;
    h = g.tex.buttonImage->height;
    unsigned char* but = buildAlphaData(&img[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 but);
    free(but);
    //
    // create dev screen
    glGenTextures(1, &g.tex.devtex);
    w = g.tex.devImage->width;
    h = g.tex.devImage->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.devtex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char* devbut = buildAlphaData(&img[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 devbut);
    free(devbut);
    //
    // create slot face
    glGenTextures(1, &g.tex.slottex);
    w = g.tex.slotImage->width;
    h = g.tex.slotImage->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.slottex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char* slotface = buildAlphaData(&img[4]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 slotface);
    //
    // create BJ Table
    glGenTextures(1, &g.tex.bjtex);
    w = g.tex.bjImage->width;
    h = g.tex.bjImage->height;
    glBindTexture(GL_TEXTURE_2D, g.tex.bjtex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 g.tex.bjImage->data);
    g.tex.xc[0] = 1.0;
    g.tex.yc[1] = 1.0;
    //
    initialize_fonts();
}

void check_mouse(XEvent* e)
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
		if (gameState == 3) {
            if (bettingUIActive) {
                processBettingInput(e->xbutton.x, e->xbutton.y, e->xbutton.button);
                return;
            }
            if (choiceUIActive) {
                handleChoiceInput(e->xbutton.x, e->xbutton.y);
                return;
            }
            if (revealUIActive) {
                handleRevealClick(e->xbutton.x, e->xbutton.y);
                return;
            }
            if (resultUIActive) {
				handleResultInput(e->xbutton.x, e->xbutton.y);
				return;
			}
        }
		if (e->xbutton.button == 1) {
			//Left button is down
			if (gameState == 0) {
				disInfo = infoButton(savex, savey);
				gameState = click(savex, savey, kill);
			}
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
	if (gameState == 0) {
		//check mouse hover state
		mouseposition = checkhover(savex,savey,mouseposition); 
	}
}

int check_keys(XEvent* e)
{
    if (e->type == KeyPress) {
        int key = XLookupKeysym(&e->xkey, 0);

        if (key == XK_Escape) {
            gameState = check_esc(gameState);
            quit(0);
        }
        ///////temporary add-in by Phil///////////
        if (gameState == 2) {
            if (key == XK_space) {
                for (int i = 0; i < 3; ++i) {
                    reels[i]->start();
                }
            }
            
        }
        ////////////////////////////////////////////
        if (gameState == 3) {
			handleDiceKeys(key);
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
			bettingUIActive = true; // Show betting UI first
			return gameState = 3;
		}
        if (key == XK_b) {
            cout << "gameState = 4\n";
            return gameState = 4;
        }
        if (key == XK_Shift_L) {
            // Out like a light, ayy,
            exit(666);
        }
        if (key == XK_space && introplay == 1) {
            introplay  = 0;
            introstart = false;
        }
    }
    return gameState;
}

void physics()
{
    switch (gameState) {
    case 2: // black jack
        break;
    }
}

void render()
{
    if (introplay == 1) {
        intro_render();
    } else {
        switch (gameState) {
			case 0:
				glClear(GL_COLOR_BUFFER_BIT);
				drawBackground();
				drawDevscreen();
				drawMenuLogo();
				drawMenuOptions(mouseposition);
				drawButtonTxt();
				gameInfo();
				if (disInfo == true) {
					glClear(GL_COLOR_BUFFER_BIT);
					drawBackground();
					gameInfo();
					displayInfo();
				}
				break;

			case 2:
				// render_slots();
				cout << "[INFO] Rendering Slot game.\n";
				//////From Phil's main///////////////////////////
				//drawSlotFace();
				srand(time(0));
				for (int x = 0; x < 3; ++x) {
					reels[x] = new Reel();
				}

				if (initGL() == 1) {
					printf("Successfully initialized OpenGL.\n");
				}

				resize(g.xres, g.yres);

				while (g.exec) {
                    while (x11.getXPending()) {
                        XEvent e = x11.getXNextEvent();
                        check_keys(&e);  // Process key events
                    }
                    
					draw();
					x11.swapBuffers();
				}

				break;
				/////////////////////////////////////////////////

                case 3:
				if (bettingUIActive)
					renderBettingUI();
				else if (choiceUIActive) {
					render_dice();
					renderChoiceUI();
				}
				else if (revealUIActive)
					renderRevealButton();
				else if (resultUIActive)
					renderResultUI();
				else
					render_dice();

				if (resultState != ResultState::IDLE)
					renderResult();
			    
                break;

			case 4:
				drawBJBackground();
				initShoe();
				// beginBJPlay();

				break;
        }
    }
}
