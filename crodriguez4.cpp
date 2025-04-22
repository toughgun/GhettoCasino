/*
 * crodriguez4.cpp
 * Author: Christian Rodriguez
 * Last Modified: 4/21/2025 (Enhanced with full dice UI flow)
*/
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "x11.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "fonts.h"
#include "crodriguez4.h"
#include <iostream>
using namespace std;

int die1 = 0, die2 = 0, total = 0;
bool diceRevealed = false;
Choice playerChoice = NONE;
bool bettingUIActive = false;
bool choiceUIActive = false;
bool revealUIActive = false;
bool resultUIActive = false;
bool addMode = true; // true = + mode, false = - mode

extern Global g;
extern X11_wrapper x11;
extern void drawBackground();

// Internal flag
ResultState resultState = ResultState::IDLE;

static inline void seed_rng_once()
{
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(nullptr));
        seeded = true;
    }
}

void updateUIForWindowSize()
{
    g.cupWidth = g.xres * 0.10f;
    g.cupHeight = g.yres * 0.10f;
}
/* ---------------------*/
/*  texture loading     */
/* ---------------------*/
void loadCupTexture()
{
    Image cup("cup.png");
    if (!cup.data) {
        fprintf(stderr, "[ERROR] cup.png not found - texture skipped.\n");
        return;
    }
    glGenTextures(1, &g.cupTexture);
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *alpha = buildAlphaData(&cup);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cup.width, cup.height,
        0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);
    free(alpha);
}
void loadDiceTextures(void)
{
    const char *files[6] = {
        "dice1.png","dice2.png","dice3.png",
        "dice4.png","dice5.png","dice6.png"
    };
    for (int i=0;i<6;i++) {
        Image imgs(files[i]);
        if (!imgs.data) {
            fprintf(stderr,"[WARN] %s missing.\n",files[i]);
            continue;
        }
        glGenTextures(1,&g.tex.diceTex[i]);
        glBindTexture(GL_TEXTURE_2D,g.tex.diceTex[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        unsigned char *a = buildAlphaData(&imgs);
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,
            imgs.width,imgs.height,0,GL_RGBA,GL_UNSIGNED_BYTE,a);
        free(a);
    }
}

void roll_dice()
{
    seed_rng_once();
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    total = die1 + die2;
    diceRevealed = false;
    playerChoice = NONE;
    resultState = ResultState::IDLE;
    printf("[INFO] Dice rolled - choose UNDER/OVER/EXACT.\n");
}

void reveal_dice(void)
{
    if (diceRevealed) return;
    diceRevealed = true;

    bool win = (playerChoice==UNDER && total<7) ||
               (playerChoice==OVER  && total>7) ||
               (playerChoice==EXACT && total==7);

    resultState     = win ? ResultState::SHOW_WIN : ResultState::SHOW_LOSS;
    resultUIActive  = true;         /* <- show dice faces + buttons */
    choiceUIActive  = false;
    revealUIActive  = false;

    if (win) {
        g.currency += g.currentBet * 2;
        g.winstreak++;
    } else {
        g.winstreak = 0;
    }
    printf("[INFO] %d + %d = %d — %s\n",
           die1,die2,total, win?"WIN":"LOSS");
}

/* --------------------------*/
/*  simple cup oscillation   */
/* --------------------------*/
static void cupPhysics()
{
    g.cupPosX += g.cupVelX;
    if (g.cupPosX >  g.cupRange) 
        g.cupVelX = -abs(g.cupVelX) * 2;
    if (g.cupPosX < -g.cupRange) 
        g.cupVelX =  abs(g.cupVelX) * 2;
}
/* ---------------------*/
/*  rendering helpers   */
/* ---------------------*/
static inline void push_ortho()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,g.xres,0,g.yres,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void render_dice()
{
    push_ortho();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();
    glBindTexture(GL_TEXTURE_2D, g.cupTexture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    float cx = g.xres * 0.5f + g.cupPosX;
    float cy = g.yres * 0.5f;
    float hw = g.cupWidth * 0.5f;
    float hh = g.cupHeight * 0.5f;
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(cx-hw,cy-hh);
        glTexCoord2f(1,0); glVertex2f(cx+hw,cy-hh);
        glTexCoord2f(1,1); glVertex2f(cx+hw,cy+hh);
        glTexCoord2f(0,1); glVertex2f(cx-hw,cy+hh);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glFlush();
}
/* draws cup at model origin – used by other modules if needed */
void drawCup()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D,g.cupTexture);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(-g.cupWidth/2,-g.cupHeight/2);
        glTexCoord2f(1,0); glVertex2f( g.cupWidth/2,-g.cupHeight/2);
        glTexCoord2f(1,1); glVertex2f( g.cupWidth/2, g.cupHeight/2);
        glTexCoord2f(0,1); glVertex2f(-g.cupWidth/2, g.cupHeight/2);
    glEnd();
    glDisable(GL_BLEND);
}
/* -----------------------------------*/
/*  run a dice round (7 s animation)  */
/* -----------------------------------*/
void Start_Dice()
{
    g.cupVelX = 4.0f;
    time_t start = time(nullptr);
    while (difftime(time(nullptr), start) < 7) {
        cupPhysics();
        render_dice();
        x11.swapBuffers();
        usleep(30000);
    }
    g.cupPosX = g.cupVelX = 0;
    roll_dice();
    choiceUIActive = true;
}

/* ----------------*/
/*  betting UI     */
/* ----------------*/

/* --------------------------------------------
* Dice‑specific keyboard handler
*----------------------------------------------*/

void handleDiceKeys(KeySym key)
{
    /* first branch: betting overlay is visible */
    if (bettingUIActive) {
        /* Return or space finalises the bet; user can also press Esc */
        if (key == XK_KP_Enter || key == XK_space)
            finalizeBet();
        if (key == XK_r)
            resetBet(); /* keep overlay, just zero bet */
        return;
    }
    /* second branch: actual dice round */
    if (choiceUIActive) {
        if (key == XK_u) 
            playerChoice = UNDER;
        else if (key == XK_o) 
            playerChoice = OVER;
        else if (key == XK_e) 
            playerChoice = EXACT;
    } else if (revealUIActive && key == XK_r) {
        if (playerChoice != NONE) 
            reveal_dice();
    }
}

void draw_button(float x, float y, float width, float height,
    const char *label)
{
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y - height);
        glVertex2f(x, y - height);
    glEnd();

    Rect r;
    r.center = 1;
    r.left = x + width / 2;
    r.bot = y - height / 2 - (16 / 3); // Use advance value of 16 here
    ggprint16(&r, 16, 0, "%s", label);
}

/* replace the whole body of draw_button_colored in crodriguez4.cpp */
void draw_button_colored(float x,float y,float w,float h,
    const char *label,float r,float g,float b)
{
    GLboolean wasTex = glIsEnabled(GL_TEXTURE_2D);  // remember state
    if (wasTex) glDisable(GL_TEXTURE_2D);           // ① solid quad
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
        glVertex2f(x,   y);
        glVertex2f(x+w, y);
        glVertex2f(x+w, y-h);
        glVertex2f(x,   y-h);
    glEnd();

    if (wasTex) glEnable(GL_TEXTURE_2D);            // ② print label
    Rect rc;  
    rc.center = 1;
    rc.left = x + w/2;  
    rc.bot = y - h/2 - 4;
    ggprint16(&rc,16,0,"%s",label);
}


static bool isInsideRect(int px, int py,int rx, int ry, int rw, int rh)
{
    int xMin = min(rx, rx + rw);
    int xMax = max(rx, rx + rw);
    int yMin = min(ry, ry - rh);       
    int yMax = max(ry, ry - rh);
    return px >= xMin && px <= xMax &&
            py >= yMin && py <= yMax;
}
void renderBettingUI(void)
{
    push_ortho();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    /* ---- layout ---- */
    const int bw       = 100;
    const int bh       = 40;
    const int spacing  = 20;
    const int buttons  = 6;
    const int totalW   = buttons * bw + (buttons - 1) * spacing;
    const int startX   = (g.xres - totalW) / 2;
    const int y        = g.yres / 4;

    /* ---- TOGGLE ---- */
    const char *toggle = addMode ? "+" : "-";
    float red   = addMode ? 0.0f : 1.0f;
    float green = addMode ? 0.6f : 0.2f;
    draw_button_colored(startX, y, bw, bh, toggle, red, green, 1.0f);

    /* ---- BET AMOUNTS ---- */
    draw_button_colored(startX + (bw + spacing) * 1, y, bw, bh, "25",
                        0.2f, 0.6f, 1.0f);
    draw_button_colored(startX + (bw + spacing) * 2, y, bw, bh, "50",
                        0.2f, 0.6f, 1.0f);
    draw_button_colored(startX + (bw + spacing) * 3, y, bw, bh, "75",
                        0.2f, 0.6f, 1.0f);
    draw_button_colored(startX + (bw + spacing) * 4, y, bw, bh, "100",
                        0.2f, 0.6f, 1.0f);

    /* ---- BET! ---- */
    draw_button_colored(startX + (bw + spacing) * 5, y, bw, bh, "BET!",
                        0.1f, 0.8f, 0.1f);

    /* ---- STATUS ---- */
    char info[128];
    snprintf(info, sizeof(info),
             "Currency: $%d | Bet: $%d | Win Streak: %d",
             g.currency, g.currentBet, g.winstreak);

    Rect txt;
    txt.left   = g.xres / 2;
    txt.center = 1;
    txt.bot    = 20;
    ggprint16(&txt, 16, 0x00ff00ff, "%s", info);
}
void renderChoiceUI()
{
    int y = g.yres / 3;
    draw_button_colored(g.xres/2 - 160, y, 100, 40, "UNDER", .4f, .5f, 1);
    draw_button_colored(g.xres/2 -  50, y, 100, 40, "EXACT", 1, .5f, .0f);
    draw_button_colored(g.xres/2 +  60, y, 100, 40, "OVER", .5f, 1, .5f);
}
void renderResultUI(void)
{
    push_ortho();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    /* ---- dice faces ---- */
    // float dw = g.xres * 0.10f;          /* size of a die */
    // float dh = dw;
    float cx = g.xres * 0.5f;
    // float cy = g.yres * 0.55f;

    // glEnable(GL_TEXTURE_2D);
    // glEnable(GL_ALPHA_TEST);
    // glAlphaFunc(GL_GREATER,0);

    // /* left die */
    // glBindTexture(GL_TEXTURE_2D,g.tex.diceTex[die1-1]);
    // glBegin(GL_QUADS);
    //     glTexCoord2f(0,0); glVertex2f(cx-dw-10,cy+dh/2);
    //     glTexCoord2f(1,0); glVertex2f(cx-10,   cy+dh/2);
    //     glTexCoord2f(1,1); glVertex2f(cx-10,   cy-dh/2);
    //     glTexCoord2f(0,1); glVertex2f(cx-dw-10,cy-dh/2);
    // glEnd();

    // /* right die */
    // glBindTexture(GL_TEXTURE_2D,g.tex.diceTex[die2-1]);
    // glBegin(GL_QUADS);
    //     glTexCoord2f(0,0); glVertex2f(cx+10,   cy+dh/2);
    //     glTexCoord2f(1,0); glVertex2f(cx+dw+10,cy+dh/2);
    //     glTexCoord2f(1,1); glVertex2f(cx+dw+10,cy-dh/2);
    //     glTexCoord2f(0,1); glVertex2f(cx+10,   cy-dh/2);
    // glEnd();

    // glDisable(GL_ALPHA_TEST);

    /* ---- result text ---- */
    renderResult();                     /* re‑use previous routine */

    /* ---- action buttons ---- */
    float bw  = g.xres * 0.18f;
    float bh  = g.yres * 0.08f;
    float y   = g.yres * 0.25f;
    draw_button_colored(cx-(bw-20), y, bw, bh, "NEW BET",
                        0.20f,0.60f,1.0f);
    draw_button_colored(cx+20,    y, bw, bh, "SAME BET",
                        0.10f,0.80f,0.10f);
}

void renderRevealButton()
{
    draw_button_colored(g.xres/2 - 50, g.yres/3 - 60, 100, 40, "REVEAL",
                                                        .8f, .1f, .1f);
}
void renderResult()
{
    const char *txt = (resultState == ResultState::SHOW_WIN)
                                                ? "YOU WON!" : "YOU LOST";
    int cref = (resultState == ResultState::SHOW_WIN)
                                                ? 0x00ff00ff : 0xff0000ff;
    Rect r = { g.xres/2, g.yres/2, 0, 0, 0 };
    r.center = 1;
    ggprint16(&r, 32, cref, txt);
}

void handleResultInput(int mx,int my)
{
    my = g.yres - my;
    float bw  = g.xres * 0.18f;
    float bh  = g.yres * 0.08f;
    float cx  = g.xres * 0.5f;
    float y   = g.yres * 0.25f;

    /* NEW BET */
    if (isInsideRect(mx,my, int(cx-bw-20), int(y), int(bw), int(bh))) {
        resultUIActive  = false;
        bettingUIActive = true;
        resultState     = ResultState::IDLE;
        return;
    }
    /* SAME BET */
    if (isInsideRect(mx,my, int(cx+20),int(y), int(bw), int(bh))) {
        resultUIActive  = false;
        Start_Dice();              /* reuse current bet */
    }
}
void handleChoiceInput(int mx, int my)
{
    my = g.yres - my;
    const int bw = 100, bh = 40;
    int topY = g.yres / 3;

    if (isInsideRect(mx, my, g.xres/2 - 160, topY, bw, bh))
        playerChoice = UNDER;
    else if (isInsideRect(mx, my, g.xres/2 -  50, topY, bw, bh))
        playerChoice = EXACT;
    else if (isInsideRect(mx, my, g.xres/2 +  60, topY, bw, bh))
        playerChoice = OVER;
    else
        return;          // clicked outside

    choiceUIActive  = false;
    revealUIActive  = true;
}

void handleRevealClick(int mx, int my)
{
    my = g.yres - my;
    if (isInsideRect(mx, my, g.xres/2 - 50, g.yres/3 - 60, 100, 40)) {
        reveal_dice();
    }
}

void processBettingInput(int mx, int my, int /*button*/)
{
    my = g.yres - my;                        // X11‑to‑OpenGL flip

    /* layout identical to renderBettingUI */
    const int bw      = 100, bh = 40, spacing = 20;
    const int totalW  = 6 * bw + 5 * spacing;
    const int startX  = (g.xres - totalW) / 2;
    const int topY    = g.yres / 4;

    /* 0.  toggle */
    if (isInsideRect(mx, my, startX, topY, bw, bh)) {
        addMode = !addMode;
        return;
    }

    /* 1‑4. chip amounts */
    const int chips[4] = {25, 50, 75, 100};
    for (int i = 0; i < 4; ++i) {
        int bx = startX + (i + 1) * (bw + spacing);
        if (isInsideRect(mx, my, bx, topY, bw, bh)) {
            int delta = addMode ?  chips[i] : -chips[i];
            g.currentBet = std::clamp(g.currentBet + delta, 0, g.currency);
            return;
        }
    }

    /* 5.  BET! */
    int bxBet = startX + 5 * (bw + spacing);
    if (isInsideRect(mx, my, bxBet, topY, bw, bh)) {
        finalizeBet();
    }
}
void finalizeBet()
{
    if (g.currentBet<=0) {
        fprintf(stderr,"[ERROR] Bet must be >0.\n");
        return;
    }
    if (g.currentBet>g.currency) {
        fprintf(stderr,"[ERROR] Not enough currency.\n");
        return;
    }
    printf("[INFO] Bet locked at %d - starting dice.\n",g.currentBet);
    g.currency -= g.currentBet;
    bettingUIActive = false;
    Start_Dice();
}
void resetBet()
{
    g.currentBet = 0;
}
/*----------------------*/
/* Credits functionality*/
/*----------------------*/
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
                        " Phillip Lakes", 14);
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
