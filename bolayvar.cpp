// Benjamin Olayvar
// last revised: 3/18/2025
//
#include <GL/glx.h>
#include <X11/Xlib.h>
#include <iostream>
#include "global.h"
#include "button.h"
#include "texture.h"

void drawMenu()
{
	//draw background image of the menu
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	
	glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
	glBegin(GL_QUADS);
		glTexCoord2f(-g.tex.xc[0], g.tex.yc[1]); glVertex2i(0,      0);
		glTexCoord2f(-g.tex.xc[0], g.tex.yc[0]); glVertex2i(0,      g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[0]); glVertex2i(g.xres, g.yres);
		glTexCoord2f(g.tex.xc[1], g.tex.yc[1]); glVertex2i(g.xres, 0);
	glEnd();
	glPopMatrix();
}

void buttonIdleState(int x, int y, int z)
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
void buttonHoverState(int x, int y, int z)
{
		//slots button
		glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
		glPushMatrix();
		glTranslatef(x,y,z);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBindTexture(GL_TEXTURE_2D, g.tex.buttontexHover);
		glBegin(GL_QUADS);
			float h = g.tex.buttonImageHover->height/4;
			float w = g.tex.buttonImageHover->width/4;
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-w,  h); //Top-left
			glTexCoord2f(1.0f, 0.0f); glVertex2f( w,  h); //Top-right
			glTexCoord2f(1.0f, 1.0f); glVertex2f( w, -h); //Bottom-right
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-w, -h); //Botton-left
		glEnd();
		glDisable(GL_ALPHA_TEST);
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
		buttonHoverState(bblackjack.pos[0],bblackjack.pos[1],bblackjack.pos[2]);
	} else {
		buttonIdleState(bblackjack.pos[0],bblackjack.pos[1],bblackjack.pos[2]);
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
	if (savex > 490 && savex < 490+300 && savey > 250 && savey < 250+75) { 
	printf("pressed slots\n");
	return done = 2;
	} else if (savex > 490 && savex < 490+300 && savey > 335 && savey < 335+75) {
	printf("pressed dice\n");
	return done = 3;
	} else if (savex > 490 && savex < 490+300 && savey > 425 && savey < 425+75) {
	printf("pressed Black Jack\n");
	return done = 4;	
	} else if (savex > 490 && savex < 490+300 && savey > 512 && savey < 512+75) {
	printf("pressed Exit\n");
	return done = 1;
	} else {
		return done = 0;
	}
}