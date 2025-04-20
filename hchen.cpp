//Haonan Chen
//april 19, 2025
//
//This file has
//intro animation
//info display
//black jack player/dealer hands logic
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "hchen_functions.h"
#include "global.h"
#include "image.h"
#include "bolayvar.h"
#include "blackjack.h"
#include "fonts.h"
#include <ctime>
#include <iostream>
using namespace std;

time_t current_time = 0;
time_t start_time = 0;
int introplay = 1;
bool introstart = false;
bool disInfo = false;
float centerX;
float centerY;
float logoPosY = 0.0f;
void gameInfo()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1120, 50, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 4;
    float w = g.tex.buttonImage->width / 4;
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

    Rect r;
    glPushMatrix();
    glTranslatef(1120, 25, 0);
    glScalef(5.0f, 5.0f, 1);
    r.bot  = 0;
    r.left = 0;
    if (disInfo == false) {
        //r.left = -32;
        ggprint8b(&r, 16, 0xffffff, "INFO");
    } else if ( disInfo == true) {
        r.left = -15;
        ggprint8b(&r, 16, 0xffffff, "BACK");
    }
    glPopMatrix();
}
bool infoButton(int x, int y)
{
    if (x > 970 && x < 1260 && y > 635 && y < 710) {
        disInfo = !disInfo;
        gameInfo();
    }
    return disInfo;
}
void displayInfo()
{
    Rect r;
    glPushMatrix();
    glTranslatef(100, 520, 0);
    glScalef(3.0f, 3.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xff0000, "Welcome to Ghetto Casino\n");
    ggprint8b(&r, 16, 0xff0000, "Here you can play SLOTS DICE BlackJack");
    ggprint8b(&r, 16, 0xff0000, "and maybe even a secert game");
    ggprint8b(&r, 16, 0xff0000, "Your Goal is To Earn as much");
    ggprint8b(&r, 16, 0xff0000, "as you can before you lose it all");
    ggprint8b(&r, 16, 0xff0000, "Good Luck and Have Fun Gambling!");
    glPopMatrix();
}
void init_intro_logo()
{
    g.tex.logo_xc[0] = 0.0f;  // Left of the texture
    g.tex.logo_xc[1] = 1.0f;  // Right of the texture
    g.tex.logo_yc[0] = 0.0f;  // Start at the top of the texture
    g.tex.logo_yc[1] = 1.0f;  // Start at the bottom of the texture
    logoPosY = g.yres + 150.0f;
}
void intro_physics()
{
    float elapsedTime = time(nullptr) - start_time;
    float startY = g.yres + 150.0f;
    float endY = g.yres / 2.0f;
    float duration = 5.0f;

    float t = elapsedTime / duration;

    if (elapsedTime >= 5.0f)
        return;

    if (t > 1.0f)
        t = 1.0f;

    logoPosY = startY * (1.0f - t) + endY * t;
}
void intro_logo()
{
    float centerX = g.xres / 2.0f;
    float centerY = logoPosY;

    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(centerX, centerY, 0.0f);

    glBindTexture(GL_TEXTURE_2D, g.tex.menulogotex);

    float h = g.tex.menuLogo->height / 2.0f;
    float w = g.tex.menuLogo->width / 2.0f;

    glBegin(GL_QUADS);
    glTexCoord2f(g.tex.logo_xc[0], g.tex.logo_yc[0]);
    glVertex2f(-w,  h);
    glTexCoord2f(g.tex.logo_xc[1], g.tex.logo_yc[0]);
    glVertex2f( w,  h);
    glTexCoord2f(g.tex.logo_xc[1], g.tex.logo_yc[1]);
    glVertex2f( w, -h);
    glTexCoord2f(g.tex.logo_xc[0], g.tex.logo_yc[1]);
    glVertex2f(-w, -h);
    glEnd();
    glPopMatrix();
}
void intro_render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (introplay == 1) {
        if (!introstart) {
            init_intro_logo();
            start_time = time(nullptr);
            //logoPosy = g.yres + 100.0f;
            introstart = true;
        }

        current_time = time(nullptr);
        if (current_time - start_time < 6) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            intro_logo();
            intro_physics();
            Rect r;
            glPushMatrix();
            glTranslatef(640, 50, 0);
            glScalef(2.0f, 2.0f, 1);
            r.bot = 0;
            r.left = 0;
            ggprint8b(&r, 16, 0xffffff, "Press Space to Skip");
            glPopMatrix();
        } else {
            introstart = false;
            introplay = 0;
        }
    }
}
void dealerHands(int x)
{
    //checks if ACE was card
    if (bj.shoe[x] != 1) {
        //if not ace, checks if jack, queen, king
        if (bj.shoe[x] == 11 || bj.shoe[x] == 12 || bj.shoe[x] == 13) {
            bj.dealerHand[bj.shoe[x]] =+ bj.shoe[x];
            bj.dealerHandTotal =+ 10;
        } else {
            bj.dealerHand[bj.shoe[x]] =+ bj.shoe[x];
            bj.dealerHandTotal =+ bj.shoe[x];
        }
    } else {
        if (bj.dealerHandTotal + bj.shoe[x] <= 21)
            bj.dealerHandTotal =+ bj.shoe[x];
        else
            bj.dealerHandTotal =+ 1;
    }

}
void playerHands(int x)
{
    //checks if ACE was card
    if (bj.shoe[x] != 1) {
        //if not ace, checks for jack, queen, king
        if (bj.shoe[x] == 11 || bj.shoe[x] == 12 || bj.shoe[x] == 13) {
            bj.playerHand[bj.shoe[x]] =+ bj.shoe[x];
            bj.dealerHandTotal =+ 10;
        } else {
            bj.playerHand[bj.shoe[x]] =+ bj.shoe[x];
            bj.playerHandTotal =+ bj.shoe[x];
        }
    } else {
        if (bj.playerHandTotal + bj.shoe[x] <= 21)
            bj.playerHandTotal =+  11;
        else
            bj.playerHandTotal =+ 1;
    }
}
