// Author: Christian Rodriguez
// Last modified: 3/11/2025
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <GL/gl.h>
#include <GL/glx.h>
#include "fonts.h"
#include "global.h"
#include "crodriguez4.h"

using namespace std;

// Dice Game Variables
int die1 = 0, die2 = 0, total = 0;
bool diceRevealed = false;
Choice playerChoice = NONE;
extern void drawBackground();
void render_dice() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground(); 
    // Bind the cup texture
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    // Calculate cup's center position (move left/right by g.cupPosX)
    float centerX = g.xres * 0.5f + g.cupPosX;
    float centerY = g.yres * 0.5f;
    float halfW = g.cupWidth  * 0.5f;
    float halfH = g.cupHeight * 0.5f;
    // Draw the cup as a textured quad in 2D
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(centerX - halfW, centerY - halfH);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(centerX + halfW, centerY - halfH);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(centerX + halfW, centerY + halfH);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(centerX - halfW, centerY + halfH);
    glEnd();
    glFlush();
}
void drawCup() {
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glColor3f(1.0f, 1.0f, 1.0f); 
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-g.cupWidth/2, -g.cupHeight/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f( g.cupWidth/2, -g.cupHeight/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f( g.cupWidth/2,  g.cupHeight/2);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-g.cupWidth/2,  g.cupHeight/2);
    glEnd();
    glDisable(GL_ALPHA_TEST);
}
void loadCupTexture() {
    Image cupImg("cup.png");
    glGenTextures(1, &g.cupTexture);
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, cupImg.width, cupImg.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, cupImg.data);
}
void draw_button(float x, float y, float width, float height, const char* label) {
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();
    Rect r;
    r.left = x + (width / 2) - 30;
    r.top  = y - (height / 2) + 10;
    r.right = 0; 
    r.bot = 0; 
    r.center = 1;  // Enable centering mode
    ggprint16(&r, 16, 0, "%s", label);
}
void roll_dice() {
    srand(time(0));
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    total = die1 + die2;
    diceRevealed = false;
    cout << "[INFO] Dice rolled. Awaiting user choice...\n";
}
void reveal_dice() {
    diceRevealed = true;
    cout << "[INFO] Revealing dice: " << die1 << " and " << die2 
         << " (Total: " << total << ")\n";
    if ((playerChoice == UNDER && total < 7) ||
        (playerChoice == OVER && total > 7) ||
        (playerChoice == EXACT && total == 7)) {
        cout << "[RESULT] You won!\n";
    } else {
        cout << "[RESULT] You lost. Try again!\n";
    }
}
/*Black jack section*/
void render_blackjack() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cout << "[INFO] Rendering Blackjack game.\n";
    glFlush();
}
/*slots section*/
void render_slots() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cout << "[INFO] Rendering Slot game.\n";
    glFlush();
}
/* Credits functionality */
static void show_child_credits_window() {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Child: Could not open display.\n");
        return;
    }
    int screen = DefaultScreen(dpy);
    Window root = DefaultRootWindow(dpy);
    int winWidth = 350;
    int winHeight = 250;

    Window win = XCreateSimpleWindow(dpy, root, 100, 100, winWidth, winHeight, 1,
                                     BlackPixel(dpy, screen), WhitePixel(dpy, screen));
    XStoreName(dpy, win, "Credits");
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(dpy, win);

    XFontStruct *font = XLoadQueryFont(dpy, "10x20");
    if (!font) {
        fprintf(stderr, "Failed to load font.\n");
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
        return;
    }
    GC gc = XCreateGC(dpy, win, 0, NULL);
    XSetFont(dpy, gc, font->fid);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    bool done = false;
    while (!done) {
        XEvent e;
        XNextEvent(dpy, &e);
        if (e.type == Expose) {
            XDrawString(dpy, win, gc, 20, 50, "Credits:", 8);
            XDrawString(dpy, win, gc, 20, 80, "  Christian Rodriguez", 21);
            XDrawString(dpy, win, gc, 20, 110, "  Philp Lakes", 13);
            XDrawString(dpy, win, gc, 20, 140, "  Haonan Chen", 13);
            XDrawString(dpy, win, gc, 20, 170, "  Ben", 5);
        }
        if (e.type == KeyPress || e.type == DestroyNotify) {
            done = true;
        }
    }
    XFreeFont(dpy, font);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}
void show_credits() {
    pid_t cpid;
    int status;
    cpid = fork();
    if (cpid == 0) {
        show_child_credits_window();
        exit(0);
    } else if (cpid > 0) {
        wait(&status);
        if (WIFEXITED(status)) {
            int es = WEXITSTATUS(status);
            printf("Child (credits) exited with status %d\n", es);
        } else {
            printf("Child did not exit normally.\n");
        }
    } else {
        perror("fork");
    }
}