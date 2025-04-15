//Haonan Chen
//april 6, 2025
//
//This file has
//intro animation
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

time_t current_time = 0;
time_t start_time = 0;
int introplay = 1;
bool introstart = false;
float centerX;
float centerY;
float logoPosY = 0.0f;
void game_info(Rect *r)
{
    // Rect r;
    // r.bot = 0;
    // r.left = 0;
    // ggprint8b(&r, 16, 0xffffff, "GHETTO CASINO");
    // glPopMatrix();
    ggprint8b(r, 16, 0x00ff00ff, "WELCOME");
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
        glTexCoord2f(g.tex.logo_xc[0], g.tex.logo_yc[0]); glVertex2f(-w,  h);
        glTexCoord2f(g.tex.logo_xc[1], g.tex.logo_yc[0]); glVertex2f( w,  h);
        glTexCoord2f(g.tex.logo_xc[1], g.tex.logo_yc[1]); glVertex2f( w, -h);
        glTexCoord2f(g.tex.logo_xc[0], g.tex.logo_yc[1]); glVertex2f(-w, -h);
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
