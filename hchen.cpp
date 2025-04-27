//Haonan Chen
//april 27, 2025
//
//This file has
//intro animation
//info display
//black jack player/dealer hands logic
//black jack split
//black jack payout
//black jack insure, double buttons
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <math.h>
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
float fade = 0.0f;
bool doubleDown = false;
void gameInfo(int xx)
{
    if (xx == 5) {
        glColor4f(1.0f, 0.53f, 0.0f, 1.0f);
        glPushMatrix();
        glTranslatef(1200, 35, 0);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
        glBegin(GL_QUADS);
        float h = g.tex.buttonImage->height / 12;
        float w = g.tex.buttonImage->width / 15;
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
    } else {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(1200, 35, 0);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
        glBegin(GL_QUADS);
        float h = g.tex.buttonImage->height / 12;
        float w = g.tex.buttonImage->width / 15;
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

    Rect r;
    glPushMatrix();
    glTranslatef(1200, 23.5, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot  = 0;
    r.left = 0;
    if (disInfo == false) {
        //r.left = -32;
        ggprint8b(&r, 16, 0xffffff, "INFO");
    } else if ( disInfo == true) {
        //r.left = -15;
        ggprint8b(&r, 16, 0xffffff, "BACK");
    }
    glPopMatrix();
}
bool infoButton(int x, int y)
{
    if (x > 1162 && x < 1238 && y > 672 && y < 696) {
        disInfo = !disInfo;
        gameInfo(5);
    }
    return disInfo;
}
void displayInfo()
{
    infoRetangle();
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
void infoRetangle()
{
    glColor3f(0.0f, 0.0f, 0.0f);
    float x = 50.0f;
    float y = 100.0f;
    float width = 800.0f;
    float height = 500.0f;
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
    glFlush();
}
void init_background()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, fade);

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, g.tex.backTexture);
    glBegin(GL_QUADS);

    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(0, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, g.yres);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(g.xres, g.yres);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(g.xres, 0);
    glEnd();
    glPopMatrix();

    glDisable(GL_BLEND);
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

    if (elapsedTime >= 9.0f)
        return;

    if (t > 1.0f)
        t = 1.0f;

    logoPosY = startY * (1.0f - t) + endY * t;

    if (elapsedTime > 4.5f) {
        fade += 0.08f;
        if (fade > 1.0f) {
            fade = 1.0f;
        }
    }
}
void intro_logo()
{
    float centerX = g.xres / 2.0f;
    float centerY = logoPosY;

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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
        if (current_time - start_time < 8.0f) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            intro_logo();
            intro_physics();
            init_background();
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
void dealerHandrecheck()
{
    //recheck if game end & dealer has ace for greater total
    if (bj.dealerHand[1] > 1) {
        if (bj.dealerHandTotal < 21 && bj.dealerHandTotal + 10 <= 21)
            bj.dealerHandTotal =+ 10;
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
void playerHandrecheck()
{
    //recheck if game end & player has ace for greater total
    if (bj.playerHand[1] > 1) {
        if (bj.playerHandTotal < 21 && bj.playerHandTotal + 10 <= 21)
            bj.playerHandTotal =+ 10;
    }
}
void bjPayout()
{
    //double down bet
    if (doubleDown == true) {
        g.currentBet = g.currentBet * 2;
        if (bj.playerHandTotal <= 21 &&
                bj.playerHandTotal > bj.dealerHandTotal) {
            g.currency =+ g.currentBet * 2;
        } else {
            g.currency =- g.currentBet;
        }
    }

    //player win
    if (bj.playerHandTotal <= 21
            && bj.playerHandTotal > bj.dealerHandTotal) {
        g.currency =+ g.currentBet * 2;

        //dealer wins with 21 /  player bust  / dealer > player
    } else if (bj.dealerHandTotal == 21 || bj.playerHandTotal > 21
            || bj.dealerHandTotal > bj.playerHandTotal) {
        g.currency =- g.currentBet;
    } else if (bj.dealerHandTotal == 21 && bj.insure == true) {
        //insure
        g.currency =+ g.currentBet;
    }
}
void bjButtonRender()
{
    renderDoubleButton();
    if (bj.dealerHand[1] > 0) {
        renderInsuranceButton();
    }
}
void renderDoubleButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1120, 50, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 6;
    float w = g.tex.buttonImage->width / 6;
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
    glTranslatef(1120, 50, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "DOUBLE");
    glPopMatrix();

}
void renderInsuranceButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1120, 150, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 6;
    float w = g.tex.buttonImage->width / 6;
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
    glTranslatef(1120, 150, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "INSURANCE");
    glPopMatrix();
}
