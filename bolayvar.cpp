// Benjamin Olayvar
// last revised: 3/29/2025
//
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <random>
#include "global.h"
#include "button.h"
#include "fonts.h"
#include "blackjack.h"
using namespace std;

typedef default_random_engine randomize;

Blackjack bj;
//=============BEGIN DRAW MENU STUFF==========================================
void drawDevscreen()
{
	glColor4f(1.0f, 1.0f, 1.0f, 0.9f);
	glPushMatrix();
	glTranslatef(dev.pos[0],dev.pos[1],dev.pos[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.tex.devtex);
	glBegin(GL_QUADS);
		float h = g.tex.devImage->height/2;
		float w = g.tex.devImage->width/2;
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  h); //Top-left
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h); //Top-right
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h); //Bottom-right
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h); //Botton-left
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
		glTexCoord2f(-g.tex.xc[0], g.tex.yc[1]); glVertex2i(0, 0);
		glTexCoord2f(-g.tex.xc[0], g.tex.yc[0]); glVertex2i(0, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
	glPopMatrix();
}

void drawMenuLogo()
{
	//draw logo for menu
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
	glPushMatrix();
	glTranslatef(menulogo.pos[0],menulogo.pos[1],menulogo.pos[2]);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g.tex.menulogotex);
	glBegin(GL_QUADS);
		float h = g.tex.menuLogo->height/2.5;
		float w = g.tex.menuLogo->width/2.5;
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  h); //Top-left
		glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h); //Top-right
		glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h); //Bottom-right
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h); //Botton-left
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void buttonIdleState(int x, int y, int z)
{
		glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
		glPushMatrix();
		glTranslatef(x,y,z);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
		glBegin(GL_QUADS);
			float h = g.tex.buttonImage->height/4;
			float w = g.tex.buttonImage->width/4;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  h); //Top-left
			glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h); //Top-right
			glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h); //Bottom-right
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h); //Botton-left
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
}

void buttonHoverState(int x, int y, int z)
{
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glPushMatrix();
		glTranslatef(x,y,z);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
		glBegin(GL_QUADS);
			float h = g.tex.buttonImage->height/4;
			float w = g.tex.buttonImage->width/4;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  h); //Top-left
			glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h); //Top-right
			glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h); //Bottom-right
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h); //Botton-left
		glEnd();
		glDisable(GL_ALPHA_TEST);
		glPopMatrix();
}

void drawButtonTxt()
{
	Rect r[2];
	//slots text
	glPushMatrix();
	glTranslatef(bslot.pos[0], bslot.pos[1]-30, 0);
	glScalef(5.0f, 5.0f, 1); // scale up 500%
	r[0].bot = 0;
	r[0].left = 0;
	ggprint8b(&r[0], 16, 0xffffff, "SLOTS");
	glPopMatrix();
	//dice text
	glPushMatrix();
	glTranslatef(bdice.pos[0], bdice.pos[1]-30, 0);
	glScalef(5.0f, 5.0f, 1); // scale up 500%
	r[1].bot = 0;
	r[1].left = 0;
	ggprint8b(&r[1], 16, 0xffffff, "DICE");
	glPopMatrix();
	//black jack text
	glPushMatrix();
	glTranslatef(bblackjack.pos[0], bblackjack.pos[1]-23, 0);
	glScalef(4.0f, 4.0f, 1); // scale up 400%
	r[1].bot = 0;
	r[1].left = 0;
	ggprint8b(&r[1], 16, 0xffffff, "BLACK JACK");
	glPopMatrix();
	//exit text
	glPushMatrix();
	glTranslatef(bexit.pos[0], bexit.pos[1]-30, 0);
	glScalef(5.0f, 5.0f, 1); // scale up 500%
	r[1].bot = 0;
	r[1].left = 0;
	ggprint8b(&r[1], 16, 0xffffff, "EXIT");
	glPopMatrix();
}

void drawMenuOptions(int x)
{
	//slots
	if(x == 1) {
		buttonHoverState(bslot.pos[0],bslot.pos[1],bslot.pos[2]);
	} else {
		buttonIdleState(bslot.pos[0],bslot.pos[1],bslot.pos[2]);
	}
	//dice
	if(x == 2) {
		buttonHoverState(bdice.pos[0],bdice.pos[1],bdice.pos[2]);
	} else {
		buttonIdleState(bdice.pos[0],bdice.pos[1],bdice.pos[2]);
	}
	//blackjack
	if(x == 3) {
		buttonHoverState(bblackjack.pos[0],bblackjack.pos[1],
		bblackjack.pos[2]);
	} else {
		buttonIdleState(bblackjack.pos[0],bblackjack.pos[1],
		bblackjack.pos[2]);
	}
	//exit
	if(x == 4) {
		buttonHoverState(bexit.pos[0],bexit.pos[1],bexit.pos[2]);
	} else {
		buttonIdleState(bexit.pos[0],bexit.pos[1],bexit.pos[2]);
	}
}

int click(int savex, int savey, int& done)
{
	if (savex > 490 && savex < 490+300 && 
	savey > 250 && savey < 250+75) { 
	printf("[STATE] Slot selected.\n");
	return 2;
	} else if (savex > 490 && savex < 490+300 && 
	savey > 335 && savey < 335+75) {
	printf("[STATE] Dice selected.\n");
	return 3;
	} else if (savex > 490 && savex < 490+300 && 
	savey > 425 && savey < 425+75) {
	printf("[STATE] Black Jack selected.\n");
	return 4;	
	} else if (savex > 490 && savex < 490+300 && 
	savey > 512 && savey < 512+75) {
	printf("[STATE] Exit selected.\n");
	return 1;
	} else {
		return done = 0;
	}
}

int checkhover(int savex, int savey, int mouseposition)
{
	if (savex > 490 && savex < 490+300 && 
	savey > 250 && savey < 250+75) {
	mouseposition = 1;
	} else if (savex > 490 && savex < 490+300 && 
	savey > 335 && savey < 335+75) {
	mouseposition = 2;
	} else if (savex > 490 && savex < 490+300 && 
	savey > 425 && savey < 425+75) {
	mouseposition = 3;
	} else if (savex > 490 && savex < 490+300 && 
	savey > 512 && savey < 512+75) {
	mouseposition = 4;
	} else {
		mouseposition = 0;
	}
	return mouseposition;
}
//====================END DRAW MENU STUFF=====================================
//
//==================BEGIN BLACK JACK STUFF====================================

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
	
	//shuffle the shoe
	shuffle(bj.shoe, bj.shoe + bj.maxCards, randomize(time(NULL)));

	cout << "[BLACKJACK] CARDS SHUFFLED, READY TO PLAY" << endl;
	//cout << "[BLACKJACK] GOOD LUCK, HAVE FUN!" << endl;

	//debugging
	for (int i = 0; i < bj.maxCards; ++i) {
    cout << bj.shoe[i] << " ";
    }
	cout << endl;

	
	//TODO: ADD LOGIC FOR ACES
	//      CAN BE 11 OR 1


	//TODO: ADD LOGIC FOR GAMEPLAY
	// SPLIT, DOUBLE, DOUBLE FOR LESS, BLACKJACK


	//PAYOUT 1:1
	//BLACK JACK PAYOUT 3:2
	//INSURENCE PAYOUT 2:1
	//DEALER MUST HIT ON SOFT 17

	bj.shuffled = true;
}
//====================END BLACK JACK STUFF====================================
//
//======================BEGIN MISC STUFF======================================

int check_esc(int x)
{
	//switch case for the esc key
	switch (x) {
		//menu case
		case 0:
		x = 1;
		break;
		//slots case
		case 2:
		x = 0;
		break;
		//dice case
		case 3:
		x = 0;
		break;
		//black jack case
		case 4:
		x = 0;
		bj.shuffled = false;
		break;
	}
	return x;
}