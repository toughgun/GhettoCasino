/*
 * crodriguez4.cpp
 * Author: Christian Rodriguez
 * Last Modified: 2025-04-24 (moved dice vars to dice.h)
*/
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "x11.h"
#include "fonts.h"
#include "crodriguez4.h"
#include "dice.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

/* global dice object + convenient aliases to preserve old references */
Dice dice;
GLuint diceTex[4][6];

#define die1             (dice.die1)
#define die2             (dice.die2)
#define total            (dice.total)
#define diceRevealed     (dice.diceRevealed)
#define playerChoice     (dice.playerChoice)
#define bettingUIActive  (dice.bettingUIActive)
#define choiceUIActive   (dice.choiceUIActive)
#define revealUIActive   (dice.revealUIActive)
#define resultUIActive   (dice.resultUIActive)
#define addMode          (dice.addMode)
#define resultState      (dice.resultState)
/* ------------------------------------------------------------------ */

extern Global g;
extern X11_wrapper x11;
extern void drawBackground();
unsigned char *buildAlphaData(Image *img);

/*==================*/
/*     helpers      */
/*==================*/
static inline void seed_rng_once(void)
{
	static bool seeded = false;
	if (!seeded) {
		srand((unsigned)time(nullptr));
		seeded = true;
	}
}

static GLuint makeTextureRGBA(Image *src)
{
	GLuint id;
	int w = src->width, h = src->height;
	unsigned char *pix = buildAlphaData(src);

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
	             GL_RGBA, GL_UNSIGNED_BYTE, pix);
	free(pix);
	return id;
}

/*=================================*/
/* window-size-dependent metrics   */
/*=================================*/
void updateUIForWindowSize(void)
{
	dice.cupWidth  = g.xres * 0.10f;
	dice.cupHeight = g.yres * 0.10f;
}

/* ------ texture loading ------ */

void loadCupTexture(void)
{
	Image cup("cup.png");
	if (!cup.data) {
		fprintf(stderr, "[ERROR] cup.png not found\n");
		return;
	}
	g.cupTexture = makeTextureRGBA(&cup);
}
void loadDiceTextures(void)
{
    Image sheet("diceSheet.png");              /* 600×900: 4×6 tiles */
    if (!sheet.data) {
        fprintf(stderr, "[ERROR] diceSheet.png not found\n");
        return;
    }
    const int cols = 4;
    const int rows = 6;
    const int cw   = sheet.width  / cols;      /* 150 */
    const int ch   = sheet.height / rows;      /* 150 */

    unsigned char *rgba = buildAlphaData(&sheet);

    for (int c = 0; c < cols; ++c) {
        for (int r = 0; r < rows; ++r) {
            unsigned char *tile =
                (unsigned char*)malloc(cw * ch * 4);
            for (int y = 0; y < ch; ++y) {
                int src = ((r * ch + y) * sheet.width +
                           c * cw) * 4;
                memcpy(tile + y * cw * 4, rgba + src, cw * 4);
            }
            glGenTextures(1, &diceTex[c][r]);
            glBindTexture(GL_TEXTURE_2D, diceTex[c][r]);
            glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,
                GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                         cw, ch, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, tile);
            free(tile);
        }
    }
    free(rgba);
}

/* -------- dice logic -------- */
void roll_dice(void)
{
	seed_rng_once();
	die1  = rand() % 6 + 1;
	die2  = rand() % 6 + 1;
	total = die1 + die2;
	diceRevealed = false;
	playerChoice = NONE;
	resultState  = IDLE;
	printf("[INFO] Dice rolled - choose UNDER / OVER / EXACT.\n");
}

void reveal_dice(void)
{
    if (diceRevealed)
        return;

    seed_rng_once();
    dice.colDie1 = rand() % 4;
    dice.colDie2 = rand() % 4;
    /* optional: make sure duplicates look different */
    if (die1 == die2 && dice.colDie2 == dice.colDie1)
        dice.colDie2 = (dice.colDie2 + 1) % 4;

    diceRevealed = true;
    bool win = (playerChoice == UNDER && total < 7) ||
               (playerChoice == OVER  && total > 7) ||
               (playerChoice == EXACT && total == 7);

    resultState    = win ? SHOW_WIN : SHOW_LOSS;
    resultUIActive = true;
    choiceUIActive = false;
    revealUIActive = false;

    if (win) {
        g.currency += g.currentBet * 2;
        g.winstreak++;
    } else {
        g.winstreak = 0;
    }
    printf("[INFO] %d + %d = %d — %s\n",
           die1, die2, total, win ? "WIN" : "LOSS");
}

/* ------------ cup physics ------------- */
static void cupPhysics(void)
{
	dice.cupPosX += dice.cupVelX;
	if (dice.cupPosX >  dice.cupRange)
		dice.cupVelX = -fabs(dice.cupVelX);
	if (dice.cupPosX < -dice.cupRange)
		dice.cupVelX =  fabs(dice.cupVelX);
}
/* ---------------------*/
/*  rendering helpers   */
/* ---------------------*/
static inline void push_ortho(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, g.xres, 0, g.yres, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void render_dice(void)
{
	push_ortho();
	glClear(GL_COLOR_BUFFER_BIT);
	drawBackground();
	glBindTexture(GL_TEXTURE_2D, g.cupTexture);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	float cx = g.xres * 0.5f + dice.cupPosX;
	float cy = g.yres * 0.5f;
	float hw = dice.cupWidth  * 0.5f;
	float hh = dice.cupHeight * 0.5f;
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex2f(cx - hw, cy - hh);
		glTexCoord2f(1, 0); glVertex2f(cx + hw, cy - hh);
		glTexCoord2f(1, 1); glVertex2f(cx + hw, cy + hh);
		glTexCoord2f(0, 1); glVertex2f(cx - hw, cy + hh);
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glFlush();
}

/* 7-second rolling animation, then prompt for choice */
void Start_Dice(void)
{
	dice.cupVelX = 4.0f;
	time_t start = time(nullptr);

	while (difftime(time(nullptr), start) < 7.0) {
		cupPhysics();
		render_dice();
		x11.swapBuffers();
		usleep(30'000);
	}
	dice.cupPosX = dice.cupVelX = 0.0f;
	roll_dice();
	choiceUIActive = true;
}

/* --------------------------------------------
* Dice‑specific keyboard handler
*----------------------------------------------*/

void handleDiceKeys(KeySym key)
{
	if (bettingUIActive) {                 /* betting overlay */
		if (key == XK_KP_Enter || key == XK_space)
			finalizeBet();
		if (key == XK_r)
			resetBet();
		return;
	}

	if (choiceUIActive) {                  /* choose O/U/E */
		if (key == XK_u)      playerChoice = UNDER;
		else if (key == XK_o) playerChoice = OVER;
		else if (key == XK_e) playerChoice = EXACT;
	} else if (revealUIActive && key == XK_r && playerChoice != NONE) {
		reveal_dice();
	}
}

/*======================*/
/*      GUI widgets     */
/*======================*/

void draw_button_colored(float x, float y, float w, float h,
    const char *label,
    float r, float g, float b)
{
    GLboolean wasTex = glIsEnabled(GL_TEXTURE_2D);
	if (wasTex) glDisable(GL_TEXTURE_2D);

	glColor3f(r, g, b);
	glBegin(GL_QUADS);
		glVertex2f(x,     y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y - h);
		glVertex2f(x,     y - h);
	glEnd();

	if (wasTex) glEnable(GL_TEXTURE_2D);

	Rect rc;
	rc.center = 1;
	rc.left   = x + w / 2;
	rc.bot    = y - h / 2 - 4;
	ggprint16(&rc, 16, 0, "%s", label);
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
/*  Betting overlay */
void renderBettingUI(void)
{
    push_ortho();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    const int bw = 100;
    const int bh = 40;
    const int spacing = 20;
    const int totalW = 6*bw + 5*spacing;
    const int startX = (g.xres-totalW)/2;
    const int topY = g.yres/4;

    /* toggle */
    draw_button_colored(startX,topY,bw,bh,
                        addMode ? "+" : "-",
                        addMode ? 0.0f : 1.0f,
                        addMode ? 0.6f : 0.2f,1.0f);

    /* chips */
    const int chips[4]={25,50,75,100};
    for(int i=0; i < 4; ++i){
        char s[4]; snprintf(s,4,"%d",chips[i]);
        draw_button_colored(startX + (i+1) * (bw + spacing),
                            topY, bw, bh, s,
                            0.2f, 0.6f, 1.0f);
    }

    /* BET! */
    draw_button_colored(startX + 5 * (bw + spacing),
                        topY,bw,bh,"BET!",
                        0.1f,0.8f,0.1f);

    /* status */
    char info[128];
   	snprintf(info,sizeof(info),
	         "Currency: $%d | Bet: $%d | Win Streak: %d",
	         g.currency,g.currentBet,g.winstreak);
	Rect t; t.left=g.xres/2; t.center=1; t.bot=20;
	ggprint16(&t,16,0x00ff00ff,"%s",info);
}
/*  Choice UI    */
void renderChoiceUI(void)
{
    int y=g.yres/3;
    draw_button_colored(g.xres/2-160,y,100,40,"UNDER",.4f,.5f,1);
    draw_button_colored(g.xres/2-50 ,y,100,40,"EXACT",1,.5f,0);
    draw_button_colored(g.xres/2+60 ,y,100,40,"OVER" ,.5f,1,.5f);
}
/*  Result UI    */
void renderResultUI(void)
{
    push_ortho();
    glClear(GL_COLOR_BUFFER_BIT);
    drawBackground();

    /* dice faces */
    float dw = g.xres * 0.10f;
    float dh = dw;
    float cx = g.xres * 0.5f;
    float cy = g.yres * 0.55f;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER,0.0f);

    /* left die */
    glBindTexture(GL_TEXTURE_2D, diceTex[dice.colDie1][die1 - 1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(cx-dw-10,cy+dh/2);
        glTexCoord2f(1,0); glVertex2f(cx-10,   cy+dh/2);
        glTexCoord2f(1,1); glVertex2f(cx-10,   cy-dh/2);
        glTexCoord2f(0,1); glVertex2f(cx-dw-10,cy-dh/2);
    glEnd();

    /* right die */
    glBindTexture(GL_TEXTURE_2D, diceTex[dice.colDie2][die2 - 1]);
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(cx+10,   cy+dh/2);
        glTexCoord2f(1,0); glVertex2f(cx+dw+10,cy+dh/2);
        glTexCoord2f(1,1); glVertex2f(cx+dw+10,cy-dh/2);
        glTexCoord2f(0,1); glVertex2f(cx+10,   cy-dh/2);
    glEnd();
    glDisable(GL_ALPHA_TEST);

    /* result text */
    renderResult();

    /* action buttons */
    float bw = g.xres * 0.18f;
    float bh = g.yres * 0.08f;
    float y  = g.yres * 0.25f;
    draw_button_colored(cx - (bw - 20), y, bw, bh,"NEW BET",
                        0.2f,0.6f,1);
    draw_button_colored(cx + 20, y, bw, bh, "SAME BET",
                        0.1f,0.8f,0.1f);
}

void renderRevealButton(void)
{
    draw_button_colored(g.xres/2-50, g.yres/3-60,
                        100,40,"REVEAL",.8f,.1f,.1f);
}

void renderResult(void)
{
    const char *msg = (resultState == SHOW_WIN)
                        ? "YOU WON!":"YOU LOST";
    int col         = (resultState == SHOW_WIN)
                        ? 0x00ff00ff:0xff0000ff;
    Rect r = {
                g.xres/2,
                g.yres/2,0,0,0
            }; 
    r.center = 1;
    ggprint16(&r,32,col,"%s",msg);
}
/*  Mouse / click handlers    */
void handleResultInput(int mx,int my)
{
    my = g.yres - my;
    float bw = g.xres*0.18f;
    float bh = g.yres*0.08f;
    float cx = g.xres*0.5f;
    float y = g.yres*0.25f;

    /* NEW BET */
    if(isInsideRect(mx, my, int(cx - bw - 20), int(y), int(bw), int(bh))){
        resultUIActive = false;
        bettingUIActive = true;
        resultState = IDLE;
        return;
    }
    /* SAME BET */
    if(isInsideRect(mx, my, int(cx + 20), int(y), int(bw), int(bh))){
        if(g.currentBet > g.currency){
            fprintf(stderr,"[ERROR] Not enough currency.\n");
            return;
        }
        resultUIActive = false;
        g.currency -= g.currentBet;
        printf("[INFO] Bet locked at %d - starting dice.\n",
                g.currentBet);
        Start_Dice();
    }
}

void handleChoiceInput(int mx,int my)
{
    my = g.yres - my;
    const int bw = 100;
    const int bh = 40;
    int topY = g.yres/3;

    if(isInsideRect(mx, my, g.xres/2 - 160, topY, bw, bh))
        playerChoice = UNDER;
    else if(isInsideRect(mx, my, g.xres/2 - 50, topY, bw, bh))
        playerChoice = EXACT;
    else if(isInsideRect(mx, my, g.xres/2 + 60, topY, bw, bh))
        playerChoice = OVER;
    else
        return;

    choiceUIActive = false;
    revealUIActive = true;
}

void handleRevealClick(int mx, int my)
{
    my = g.yres - my;
    if (isInsideRect(mx, my, g.xres/2 - 50, g.yres/3 - 60, 100, 40))
        reveal_dice();
}

void processBettingInput(int mx, int my, int /*button*/)
{
	my = g.yres - my;

	const int bw = 100, bh = 40, spacing = 20;
	const int totalW = 6 * bw + 5 * spacing;
	const int startX = (g.xres - totalW) / 2;
	const int topY   = g.yres / 4;

	/* 0. toggle + / - */
	if (isInsideRect(mx, my, startX, topY, bw, bh)) {
		addMode = !addMode;
		return;
	}

	/* 1-4. chip amounts */
	const int chips[4] = {25, 50, 75, 100};
	for (int i = 0; i < 4; ++i) {
		int bx = startX + (i + 1) * (bw + spacing);
		if (isInsideRect(mx, my, bx, topY, bw, bh)) {
			int delta = addMode ? chips[i] : -chips[i];
			g.currentBet = clamp(g.currentBet + delta, 0, g.currency);
			return;
		}
	}

	/* 5. BET! */
	int bxBet = startX + 5 * (bw + spacing);
	if (isInsideRect(mx, my, bxBet, topY, bw, bh))
		finalizeBet();
}
void finalizeBet(void)
{
	if (g.currentBet <= 0) {
		fprintf(stderr, "[ERROR] Bet must be > 0\n");
		return;
	}
	if (g.currentBet > g.currency) {
		fprintf(stderr, "[ERROR] Not enough currency\n");
		return;
	}
	printf("[INFO] Bet locked at %d - starting dice.\n", g.currentBet);
	g.currency -= g.currentBet;
	bettingUIActive = false;
	Start_Dice();
}
void resetBet(void)
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