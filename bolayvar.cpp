// Benjamin Olayvar
// last revised: 4/22/2025
//
#include "blackjack.h"
#include "button.h"
#include "fonts.h"
#include "global.h"
#include "hchen_functions.h"
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>

using namespace std;

typedef default_random_engine randomize;

Blackjack bj;
//=============BEGIN DRAW MENU STUFF==========================================
void drawDevscreen()
{
    glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
    glPushMatrix();
    glTranslatef(dev.pos[0], dev.pos[1], dev.pos[2]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.devtex);
    glBegin(GL_QUADS);
    float h = g.tex.devImage->height / 2;
    float w = g.tex.devImage->width / 2;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
void drawBackground()
{
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[1]);
    glVertex2i(0, 0);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[0]);
    glVertex2i(0, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[0]);
    glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[1]);
    glVertex2i(g.xres, 0);
    glEnd();
    glPopMatrix();
}
void drawMenuLogo()
{
    // draw logo for menu
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(menulogo.pos[0], menulogo.pos[1], menulogo.pos[2]);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.menulogotex);
    glBegin(GL_QUADS);
    float h = g.tex.menuLogo->height / 2.5;
    float w = g.tex.menuLogo->width / 2.5;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
void buttonIdleState(int x, int y, int z, int xScale, int yScale, float r,
                     float gg, float b)
{
    glColor4f(r, gg, b, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / yScale;
    float w = g.tex.buttonImage->width / xScale;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}

void buttonHoverState(int x, int y, int z, int xScale, int yScale)
{
    glColor4f(1.0f, 0.53f, 0.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / yScale;
    float w = g.tex.buttonImage->width / xScale;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
void drawButtonTxt()
{
    Rect r[2];
    // slots text
    glPushMatrix();
    glTranslatef(bslot.pos[0], bslot.pos[1] - 30, 0);
    glScalef(5.0f, 5.0f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "SLOTS");
    glPopMatrix();
    // dice text
    glPushMatrix();
    glTranslatef(bdice.pos[0], bdice.pos[1] - 30, 0);
    glScalef(5.0f, 5.0f, 1); // scale up 500%
    r[1].bot  = 0;
    r[1].left = 0;
    ggprint8b(&r[1], 16, 0xffffff, "DICE");
    glPopMatrix();
    // black jack text
    glPushMatrix();
    glTranslatef(bblackjack.pos[0], bblackjack.pos[1] - 23, 0);
    glScalef(4.0f, 4.0f, 1); // scale up 400%
    r[1].bot  = 0;
    r[1].left = 0;
    ggprint8b(&r[1], 16, 0xffffff, "BLACK JACK");
    glPopMatrix();
    // exit text
    glPushMatrix();
    glTranslatef(bexit.pos[0], bexit.pos[1] - 30, 0);
    glScalef(5.0f, 5.0f, 1); // scale up 500%
    r[1].bot  = 0;
    r[1].left = 0;
    ggprint8b(&r[1], 16, 0xffffff, "EXIT");
    glPopMatrix();
}
void drawMenuOptions(int x)
{
    // slots
    if (x == 1) {
        buttonHoverState(bslot.pos[0], bslot.pos[1], bslot.pos[2], 4, 4);
    } else {
        buttonIdleState(bslot.pos[0], bslot.pos[1], bslot.pos[2], 4, 4, 1, 1,
                        1);
    }
    // dice
    if (x == 2) {
        buttonHoverState(bdice.pos[0], bdice.pos[1], bdice.pos[2], 4, 4);
    } else {
        buttonIdleState(bdice.pos[0], bdice.pos[1], bdice.pos[2], 4, 4, 1, 1,
                        1);
    }
    // blackjack
    if (x == 3) {
        buttonHoverState(bblackjack.pos[0], bblackjack.pos[1],
                         bblackjack.pos[2], 4, 4);
    } else {
        buttonIdleState(bblackjack.pos[0], bblackjack.pos[1], bblackjack.pos[2],
                        4, 4, 1, 1, 1);
    }
    // exit
    if (x == 4) {
        buttonHoverState(bexit.pos[0], bexit.pos[1], bexit.pos[2], 4, 4);
    } else {
        buttonIdleState(bexit.pos[0], bexit.pos[1], bexit.pos[2], 4, 4, 1, 1,
                        1);
    }
}
int click(int savex, int savey, int& done)
{
    if (savex > 490 && savex < 490 + 300 && savey > 250 && savey < 250 + 75) {
        printf("[STATE] Slot selected.\n");
        return 2;
    } else if (savex > 490 && savex < 490 + 300 && savey > 335 &&
               savey < 335 + 75) {
        printf("[STATE] Dice selected.\n");
        return 3;
    } else if (savex > 490 && savex < 490 + 300 && savey > 425 &&
               savey < 425 + 75) {
        printf("[STATE] Black Jack selected.\n");
        return 4;
    } else if (savex > 490 && savex < 490 + 300 && savey > 512 &&
               savey < 512 + 75) {
        printf("[STATE] Exit selected.\n");
        return 1;
    } else {
        return done = 0;
    }
}
int checkhover(int savex, int savey, int mouseposition)
{
    if (savex > 490 && savex < 490 + 300 && savey > 250 && savey < 250 + 75) {
        mouseposition = 1;
    } else if (savex > 490 && savex < 490 + 300 && savey > 335 &&
               savey < 335 + 75) {
        mouseposition = 2;
    } else if (savex > 490 && savex < 490 + 300 && savey > 425 &&
               savey < 425 + 75) {
        mouseposition = 3;
    } else if (savex > 490 && savex < 490 + 300 && savey > 512 &&
               savey < 512 + 75) {
        mouseposition = 4;
    } else if (savex > 1162 && savex < 1238 && savey > 672 && savey < 696) {
        mouseposition = 5;
    } else {
        mouseposition = 0;
    }
    return mouseposition;
}
//====================END DRAW MENU STUFF=====================================
//
//==================BEGIN BLACK JACK STUFF====================================
void bjUIClickListener(int savex, int savey)
{
    if (!bj.allIn && bj.showUI) {
        if (savex > 445 && savex < 531 && savey > 280 && savey < 365) {
            g.currentBet += 5;
            // g.currency -= 5;
            bj.betarray[bj.betarraypointer++] = 5;
        } else if (savex > 445 + (100 * 1) && savex < 531 + (100 * 1) &&
                   savey > 280 && savey < 365) {
            g.currentBet += 10;
            // g.currency -= 10;
            bj.betarray[bj.betarraypointer++] = 10;
        } else if (savex > 445 + (100 * 2) && savex < 531 + (100 * 2) &&
                   savey > 280 && savey < 365) {
            g.currentBet += 25;
            // g.currency -= 25;
            bj.betarray[bj.betarraypointer++] = 25;
        } else if (savex > 445 + (100 * 3) && savex < 531 + (100 * 3) &&
                   savey > 280 && savey < 365) {
            g.currentBet += 100;
            // g.currency -= 100;
            bj.betarray[bj.betarraypointer++] = 100;
        }
    }
    if (bj.showUI && savex > 522 && savex < 758 && savey > 498 && savey < 554) {
        bj.showUI         = false;
        bj.allIn          = false;
        bj.gameInProgress = true;
        g.currency -= g.currentBet;

        for (int i = 0; i < bj.betarraypointer + 1; i++) {
            bj.betarray[i] = 0;
        }
        bj.betarraypointer = 0;
    }

    if (bj.showUI && savex > 567 && savex < 713 && savey > 570 && savey < 605) {
        g.currentBet -= bj.betarray[bj.betarraypointer--];
        if (g.currentBet < 0) {
            g.currentBet       = 0;
            bj.betarraypointer = 0;
        }

        if (bj.showUI && g.currentBet < g.currency) {
            bj.allIn = false;
        }
    }

    if (bj.showUI && g.currentBet >= g.currency) {
        g.currentBet = g.currency;
    }
    /*
        // correct the total amount in the array for undo
        if (bj.allIn) {
            g.currentBet = g.currency;

            int temp = 0;
            for (int i = 0; i < bj.betarraypointer - 1; i++) {
                temp += bj.betarray[i];
            }
            temp = temp - bj.currency;
            bj.betarray[bj.betarraypointer - 1] = temp;
        }
    */
}
int bjUIHoverListener(int savex, int savey, int mouseposition)
{
    if (bj.showUI) {
        if (bj.showUI && savex > 522 && savex < 758 && savey > 498 && savey < 554) {
            mouseposition = 6;
        } else if (bj.showUI && savex > 567 && savex < 713 && savey > 570 && savey < 605) {
            mouseposition = 7;
        } else {
        mouseposition = 0;
        }
    }
    return mouseposition;
}
void drawBJBackground(float r, float gg, float b)
{
    glColor3f(r, gg, b);
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.tex.bjtex);
    glBegin(GL_QUADS);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[1]);
    glVertex2i(0, 0);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[0]);
    glVertex2i(0, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[0]);
    glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[1]);
    glVertex2i(g.xres, 0);
    glEnd();
    glPopMatrix();
}
void drawChipsFull(int x, int y, int z, float xScale, float yScale)
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.chiptex);
    glBegin(GL_QUADS);
    float h = g.tex.chipImage->height / yScale;
    float w = g.tex.chipImage->width / xScale;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
void drawCard(int num, int suit, float posx, float posy)
{
    float cx = posx;
	float cy = posy;
    float h = 180;
	float w = 118;
	glPushMatrix();
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, g.tex.cardtex);
	glTranslatef(0,0, 0);
	glColor4ub(255,255,255,255);
	int ix = num % 14;
	int iy = suit;
	float tx = (float)ix / 14.0;
	float ty = (float)iy / 4.0;
	glBegin(GL_QUADS);
		glTexCoord2f(tx,      ty+0.25); glVertex2i(cx-w, cy-h);
		glTexCoord2f(tx,      ty);    glVertex2i(cx-w, cy+h);
		glTexCoord2f(tx+0.0714, ty);    glVertex2i(cx+w, cy+h);
		glTexCoord2f(tx+0.0714, ty+0.25); glVertex2i(cx+w, cy-h);
	glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_ALPHA_TEST);
}
void drawChipText()
{
    Rect r[2];
    // place your bet
    glPushMatrix();
    glTranslatef(g.xres / 2, g.yres / 1.47, 0);
    glScalef(4.0f, 4.0f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "place your bet");
    glPopMatrix();
    // 100
    glPushMatrix();
    glTranslatef(g.xres / 1.62, g.yres / 1.79 - 30, 0);
    glScalef(3.5f, 3.5f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "100");
    glPopMatrix();
    // 25
    glPushMatrix();
    glTranslatef((g.xres / 1.613) - 100, g.yres / 1.79 - 30, 0);
    glScalef(3.5f, 3.5f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "25");
    glPopMatrix();
    // 10
    glPushMatrix();
    glTranslatef((g.xres / 1.613) - 204, g.yres / 1.79 - 30, 0);
    glScalef(3.5f, 3.5f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "10");
    glPopMatrix();
    // 5
    glPushMatrix();
    glTranslatef((g.xres / 1.613) - 304, g.yres / 1.79 - 30, 0);
    glScalef(3.5f, 3.5f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "5");
    glPopMatrix();
}
void drawBetText()
{
    Rect r[2];
    char betStr[64];
    // current Bet
    if (g.currentBet < g.currency) {
        glPushMatrix();
        glTranslatef(g.xres / 2, g.yres / 2.745, 0);
        glScalef(3.5f, 3.5f, 1); // scale up 500%
        r[0].bot  = 0;
        r[0].left = 0;
        sprintf(betStr, "%d", g.currentBet);
        ggprint8b(&r[0], 16, 0xffffff, betStr);
        glPopMatrix();
    } else {
        glPushMatrix();
        glTranslatef(g.xres / 2, g.yres / 2.745, 0);
        glScalef(3.5f, 3.5f, 1); // scale up 500%
        r[0].bot  = 0;
        r[0].left = 0;
        ggprint8b(&r[0], 16, 0xffffff, "ALL IN");
        glPopMatrix();
        bj.allIn = true;
    }
    // min bet max info
    glPushMatrix();
    glTranslatef(g.xres / 2, g.yres / 3.1, 0);
    glScalef(1.5f, 1.5f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    sprintf(betStr, "Minimum Bet: 5 | Maxiumum Bet: %d", g.currency);
    ggprint8b(&r[0], 16, 0xffffff, betStr);
    glPopMatrix();
    // CUrrent balance
    glPushMatrix();
    glTranslatef(g.xres / 10.53, g.yres / 720, 0);
    glScalef(5.0f, 5.0f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    sprintf(betStr, "%d", g.currency);
    ggprint8b(&r[0], 16, 0xffffff, betStr);
    glPopMatrix();
    // wallet text
    glPushMatrix();
    glTranslatef(g.xres / 10.53, g.yres / 12.53, 0);
    glScalef(4.0f, 4.0f, 1); // scale up 500%
    r[0].bot  = 0;
    r[0].left = 0;
    ggprint8b(&r[0], 16, 0xffffff, "wallet");
    glPopMatrix();

    // play button
    if (g.currentBet >= 5) {
        glPushMatrix();
        glTranslatef(g.xres / 2, g.yres / 4.45, 0);
        glScalef(5.0f, 5.0f, 1); // scale up 500%
        r[0].bot  = 0;
        r[0].left = 0;
        ggprint8b(&r[0], 16, 0xffffff, "PLAY");
        glPopMatrix();
    } else {
        glPushMatrix();
        glTranslatef(g.xres / 2, g.yres / 4.45, 0);
        glScalef(5.0f, 5.0f, 1); // scale up 500%
        r[0].bot  = 0;
        r[0].left = 0;
        ggprint8b(&r[0], 16, 0xc8c8c8, "PLAY");
        glPopMatrix();
    }
    // buttonIdleState(g.xres/2, g.yres/5.5, 0, 8.5, 8.5, 0.7, 0, 0);
    if (g.currentBet >= 5) {
        glPushMatrix();
        glTranslatef(g.xres / 2, g.yres / 6.35, 0);
        glScalef(3.0f, 3.0f, 1); // scale up 500%
        r[0].bot  = 0;
        r[0].left = 0;
        ggprint8b(&r[0], 16, 0xffffff, "undo");
        glPopMatrix();
    }
}
void drawBJShoe(int x, int y, int z, float xScale, float yScale, float r,
                float gg, float b)
{
    glColor4f(r, gg, b, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, z);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, g.tex.shoetex);
    glBegin(GL_QUADS);
    float h = g.tex.shoeImage->height / yScale;
    float w = g.tex.shoeImage->width / xScale;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h); // Top-left
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h); // Top-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h); // Bottom-right
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h); // Botton-left
    glEnd();
    glDisable(GL_BLEND);
    glPopMatrix();
}
void dimBackground()
{
    glPushMatrix();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
    float x      = 0;
    float y      = 0;
    float width  = g.xres;
    float height = g.yres;
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    // glDisable(GL_BLEND);
}
void initShoe()
{
    if (bj.shuffled) {
        return;
    }

    int cardpos = 0;
    // Add 24 of each card value 1–9
    // 4 cards of each value in a deck
    // * 6 decks in a shoe
    for (int value = 1; value <= 9; ++value) {
        for (int count = 0; count < 24; ++count) {
            bj.shoe[cardpos++] = value;
        }
    }

    // Add 96 tens (10, J, Q, K all worth 10)
    // 4 cards of each value in a deck, so 16
    // * 6 decks in a shoe
    for (int count = 0; count < 96; ++count) {
        bj.shoe[cardpos++] = 10;
    }

    // shuffle the shoe
    shuffle(bj.shoe, bj.shoe + bj.maxCards, randomize(time(NULL)));
    cout << "[BLACKJACK] CARDS SHUFFLED, READY TO PLAY" << endl;

    // set the maker to indicate when to shuffle
    bj.marker = rand() % 62 + 218;
    cout << "Marker set At: " << bj.marker << endl;

    // debugging
    for (int i = 0; i < bj.maxCards; ++i) {
        cout << bj.shoe[i] << " ";
    }
    cout << endl;
    // TODO: ADD LOGIC FOR ACES
    //       CAN BE 11 OR 1

    // TODO: ADD LOGIC FOR GAMEPLAY
    //  SPLIT, DOUBLE, DOUBLE FOR LESS, BLACKJACK

    // PAYOUT 1:1
    // BLACK JACK PAYOUT 3:2
    // INSURENCE PAYOUT 2:1
    // DEALER MUST HIT ON SOFT 17

    bj.shuffled = true;
    bj.showUI   = true;
}
void insuranceScam()
{
    // Animation and Logic for insurance here
}
void checkDealerHand()
{
    // If dealer pulls 10 first, dealer always checks to see if they have 21
    if (bj.dealerHand[0] == 10 && bj.dealerHand[1] == 1) {
        // Place animation for 10 and Ace here



        bj.wait = true;
        // debugging
        cout << "DEALER WINS. 21 BJ" << endl;
        g.currentBet = 0;
        bj.showUI    = !bj.showUI;
    }

    // If dealer pulls Ace first, offer insurence
    for (int i = 0; i < bj.dealerHandTotal; i++) {
        if (bj.dealerHand[i] == 1) {
        }
    }
    if (bj.dealerHand[0] == 1) {
        insuranceScam();
    }
}
void initFirstHand()
{
    if (!bj.initialhand) {
        bj.discard[0] = bj.shoe[0];
        bj.shoe[0]    = 0;
        bj.currentPos++;
    }
    bj.initialhand = true;

    for (int i = 0; i < 2; i++) {
        bj.playerHand[i] = bj.shoe[bj.currentPos];
        bj.playerHandTotal += bj.shoe[bj.currentPos];
        bj.currentPos++;
        bj.dealerHand[i] = bj.shoe[bj.currentPos];
        bj.dealerHandTotal += bj.shoe[bj.currentPos];
        bj.currentPos++;
    }
    printf("Dealer: %d %d\nPlayer: %d %d\n", bj.dealerHand[0], bj.dealerHand[1], bj.playerHand[0], bj.playerHand[1]);
}
void bjHit()
{
    // Logic and Animation for Hit Here
}
void showUI(int xx)
{
    drawBJBackground(0.5, 0.5, 0.5);
    drawBJShoe(g.xres / 1.13, g.yres / 1.77, 0, 3, 3, 0.5, 0.5, 0.5);
    // dimBackground(); // ITS NOT WORKING FOR SOME REASON
    drawChipsFull(g.xres / 2, g.yres / 1.82, 0, 5, 5);
    drawChipText();
    buttonIdleState(g.xres / 2, g.yres / 2.53, 0, 4, 6, 0, 0, 0); // black bet
    buttonIdleState(g.xres / 10.53, g.yres / 24, 0, 5.5, 5, 0, 0,
                    0); // black wallet

    // play button
    if (g.currentBet >= 5) {
        if (xx == 6) {
            buttonIdleState(g.xres / 2, g.yres / 3.75, 0, 5, 5, 0, 1.0, 0);
        } else {
            buttonIdleState(g.xres / 2, g.yres / 3.75, 0, 5, 5, 0, 0.75, 0);
        }
    } else {
        buttonIdleState(g.xres / 2, g.yres / 3.75, 0, 5, 5, 0, 0.5, 0);
    }

    // undo button
    if (g.currentBet >= 5) {
        if (xx == 7) {
            buttonIdleState(g.xres / 2, g.yres / 5.5, 0, 8.5, 8.5, 1.0, 0, 0);
        } else {
            buttonIdleState(g.xres / 2, g.yres / 5.5, 0, 8.5, 8.5, 0.75, 0, 0);
        }
    }
    drawBetText();
}
void playBlackJack()
{
    drawBJBackground(1.0, 1.0, 1.0);
    drawBJShoe(g.xres / 1.13, g.yres / 1.77, 0, 3, 3, 1.0, 1.0, 1.0);
    initFirstHand();

    // animation here
    //checkDealerHand();
}
void handleBlackJackGame(int x)
{
    if (!bj.shuffled) {
        initShoe();
    }
    if (bj.showUI) {
        showUI(x);
        drawCard(12,0, g.xres/2, g.yres/2);
    } else {
        playBlackJack();
    }
}
void handleBlackJackKeys(int x)
{
    // lock out the player from doing anything until the delt cards or animaiton
    // is complete
    if (bj.delt && !bj.wait) {
        switch (x) {
        case XK_space:
            bjHit();
            break;

        case XK_d || XK_D:
            // bjDouble();
            break;
        }
    }
}
//====================END BLACK JACK STUFF====================================
//
//======================BEGIN MISC STUFF======================================
void drawSlotFace()
{
    glColor4f(1.0, 1.0, 1.0, 0.9f);
    glPushMatrix();
    glTranslatef(0, 0, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.slottex);
    glBegin(GL_QUADS);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[1]);
    glVertex2i(0, 0);
    glTexCoord2f(-g.tex.xc[0], g.tex.yc[0]);
    glVertex2i(0, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[0]);
    glVertex2i(g.xres, g.yres);
    glTexCoord2f(g.tex.xc[1], g.tex.yc[1]);
    glVertex2i(g.xres, 0);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
}
int check_esc(int x)
{
    // switch case for the esc key
    switch (x) {
    // menu case
    case 0:
        x = 1;
        break;
    // slots case
    case 2:
        x = 0;
        break;
    // dice case
    case 3:
        x = 0;
        break;
    // black jack case
    case 4:
        x           = 0;
        bj.shuffled = false;
        break;
    }
    return x;
}
