/*
 * crodriguez4.cpp
 * Author: Christian Rodriguez
 * Last Modified: 3/11/2025 (Updated with betting functionality)
*/
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
#include "x11.h"
#include "fonts.h"
#include "crodriguez4.h"
using namespace std;

int die1 = 0, die2 = 0, total = 0;
bool diceRevealed = false;
Choice playerChoice = NONE;

extern void drawBackground();

void render_dice()
{
    /* Set up projection and view matrices */
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
    // renderBettingUI();
}
void drawCup()
{
    /* Enable alpha testing and draw the cup quad */
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(-g.cupWidth / 2, -g.cupHeight / 2);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(g.cupWidth / 2, -g.cupHeight / 2);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(g.cupWidth / 2, g.cupHeight / 2);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(-g.cupWidth / 2, g.cupHeight / 2);
    glEnd();
    glDisable(GL_ALPHA_TEST);
}
void loadCupTexture()
{
    /* Load the cup image and generate an OpenGL texture */
    Image cupImg("cup.png");
    glGenTextures(1, &g.cupTexture);
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, cupImg.width, cupImg.height, 0,
                 GL_RGB, GL_UNSIGNED_BYTE, cupImg.data);
}
void roll_dice()
{
    /* Generate random dice values and compute the total */
    srand(time(0));
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    total = die1 + die2;
    diceRevealed = false;
    cout << "[INFO] Dice rolled. Awaiting user choice...\n";
}
void cupPhysics()
{
    /* Update cup position and reverse direction if at range limits */
    g.cupPosX += g.cupVelX;
    if (g.cupPosX > g.cupRange || g.cupPosX < -g.cupRange)
        g.cupVelX = -g.cupVelX;
}
void Start_Dice()
{
    /* 
     * Run the dice game loop for 7 seconds.
     * Process pending X11 events and update cup physics.
     */
    time_t start = time(NULL);
    while (difftime(time(NULL), start) < 7) {
        /* Process X11 events in the loop */
        while (XPending(x11.getDisplay())) {
            XEvent event;
            XNextEvent(x11.getDisplay(), &event);
            if (event.type == KeyPress) {
                KeySym key = XLookupKeysym(&event.xkey, 0);
                if (key == XK_v) {
                    // Toggle vsync flag in your global state 
                    g.vsync = !g.vsync;
                    // Get function pointer for vsync control
                    static PFNGLXSWAPINTERVALEXTPROC 
                        glXSwapIntervalEXT = NULL;
                    if (!glXSwapIntervalEXT) {
                        glXSwapIntervalEXT =
                          (PFNGLXSWAPINTERVALEXTPROC)
                          glXGetProcAddressARB(
                          (const GLubyte *)"glXSwapIntervalEXT");
                    }
                    // Get the current drawable
                    GLXDrawable drawable = glXGetCurrentDrawable();
                    if (g.vsync) {
                        glXSwapIntervalEXT(x11.getDisplay(), drawable, 1);
                        cout << "Vsync enabled\n";
                    } else {
                        glXSwapIntervalEXT(x11.getDisplay(), drawable, 0);
                        cout << "Vsync disabled\n";
                    }
                }
            }
        }
        cupPhysics();
        usleep(300000);
    }
    /* Reset cup motion and update display */
    g.cupVelX = 0;
    g.cupPosX = 0;
    render_dice();
    roll_dice();
}
void reveal_dice()
{
    /* Reveal dice and display outcome based on player's choice */
    diceRevealed = true;
    cout << "[INFO] Revealing dice: " << die1 << " and " << die2 
         << " (Total: " << total << ")\n";
    if ((playerChoice == UNDER && total < 7) ||
        (playerChoice == OVER && total > 7) ||
        (playerChoice == EXACT && total == 7))
        cout << "[RESULT] You won!\n";
    else
        cout << "[RESULT] You lost. Try again!\n";
}
void draw_button(float x, float y, float width, float height,
                 const char *label)
{
    /* Draw a button quad with text label centered on it */
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
    r.center = 1;
    ggprint16(&r, 16, 0, "%s", label);
}
static void show_child_credits_window()
{
    /* Create and display a child window to show credits */
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Child: Could not open display.\n");
        return;
    }
    int screen = DefaultScreen(dpy);
    Window root = DefaultRootWindow(dpy);
    int winWidth = 350, winHeight = 250;
    Window win = XCreateSimpleWindow(dpy, root, 100, 100, winWidth,
                                     winHeight, 1,
                                     BlackPixel(dpy, screen),
                                     WhitePixel(dpy, screen));
    XStoreName(dpy, win, "Credits");
    XSelectInput(dpy, win,
      ExposureMask | KeyPressMask | StructureNotifyMask);
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
        /* Process events for window exposure and key presses */
        XEvent e;
        XNextEvent(dpy, &e);
        if (e.type == Expose) {
            XDrawString(dpy, win, gc, 20, 50, "Credits:", 8);
            XDrawString(dpy, win, gc, 20, 80,
                        " Christian Rodriguez", 20);
            XDrawString(dpy, win, gc, 20, 110,
                        " Philp Lakes", 12);
            XDrawString(dpy, win, gc, 20, 140,
                        " Haonan Chen", 12);
            XDrawString(dpy, win, gc, 20, 170, " Ben", 4);
        }
        if (e.type == KeyPress || e.type == DestroyNotify)
            done = true;
    }
    XFreeFont(dpy, font);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}
void show_credits()
{
    /* Fork a process to show the credits window */
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
bool isInsideRect(int mouseX, int mouseY, int rectX, int rectY,
                  int rectW, int rectH)
{
    /* Return true if the point is inside the specified rectangle */
    return (mouseX >= rectX && mouseX <= rectX + rectW &&
            mouseY >= rectY && mouseY <= rectY + rectH);
}
void renderBettingUI()
{
    /* Render the betting UI with a box, currency info, and bet buttons */
    int margin = 10, boxWidth = 200, boxHeight = 50;
    glColor3f(0.8f, 0.8f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2i(margin, g.yres - margin);
        glVertex2i(margin + boxWidth, g.yres - margin);
        glVertex2i(margin + boxWidth, g.yres - margin - boxHeight);
        glVertex2i(margin, g.yres - margin - boxHeight);
    glEnd();
    char info[100];
    snprintf(info, sizeof(info), "Currency: %d  Bet: %d", g.currency,
             g.currentBet);
    glColor3f(0.0f, 0.0f, 0.0f);
    Rect r;
    r.left = margin + 5;
    r.top = g.yres - margin - 10;
    r.center = 0;
    ggprint16(&r, 16, 0, info);
    /* Draw bet adjustment buttons */
    int buttonW = 50, buttonH = 30, spacing = 5;
    int betValues[6] = {1, 5, 10, 25, 50, 100};
    int startX = margin;
    int startY = g.yres - margin - boxHeight - spacing - buttonH;
    int i;
    for (i = 0; i < 6; i++) {
        char label[10];
        snprintf(label, sizeof(label), "%d", betValues[i]);
        draw_button(startX + i * (buttonW + spacing), startY,
                    buttonW, buttonH, label);
    }
    /* Draw the decrease bet buttons below the increase buttons */
    startY = g.yres - margin - boxWidth - spacing - 2 * buttonH -
             spacing;
    for (i = 0; i < 6; i++) {
        char label[10];
        snprintf(label, sizeof(label), "-%d", betValues[i]);
        draw_button(startX + i * (buttonW + spacing), startY,
                    buttonW, buttonH, label);
    }
}
void processBettingInput(int mouseX, int mouseY, int button)
{
    /* Process mouse clicks for adjusting the bet.
       Left-click increases; right-click decreases. */
    mouseY = g.yres - mouseY;
    int margin = 10, boxHeight = 50, spacing = 5;
    int buttonW = 50, buttonH = 30;
    int incRowY = g.yres - margin - boxHeight - spacing - buttonH;
    int decRowY = g.yres - margin - boxHeight - spacing - 2 * buttonH -
        spacing;
    int betValues[6] = {1, 5, 10, 25, 50, 100};
    int i;
    if (button == 1) {
        for (i = 0; i < 6; i++) {
            int btnX = margin + i * (buttonW + spacing);
            if (isInsideRect(mouseX, mouseY, btnX, incRowY,
                             buttonW, buttonH)) {
                if (g.currentBet + betValues[i] <= g.currency)
                    g.currentBet += betValues[i];
                else
                    g.currentBet = g.currency;
                return;
            }
        }
    } else if (button == 3) {
        for (i = 0; i < 6; i++) {
            int btnX = margin + i * (buttonW + spacing);
            if (isInsideRect(mouseX, mouseY, btnX, decRowY,
                             buttonW, buttonH)) {
                if (g.currentBet - betValues[i] < 1)
                    g.currentBet = 1;
                else
                    g.currentBet -= betValues[i];
                return;
            }
        }
    }
}
void finalizeBet()
{
    /* Finalize the bet, deduct currency if valid, and start dice game */
    if (g.currentBet <= 0) {
        cout << "[ERROR] Bet must be greater than 0 before starting "
             << "the game.\n";
    } else {
        cout << "[INFO] Bet finalized at " << g.currentBet 
             << ". Starting dice game...\n";
        g.currency -= g.currentBet;
        Start_Dice();
    }
}
void resetBet()
{
    g.currentBet = 0;
}