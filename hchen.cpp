//Haonan Chen
//May 4, 2025
//
//This file has
//intro animation
//info display
//black jack logic n stuff
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

/*===========================GAME INFO START============================*/

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
    ggprint8b(&r, 16, 0xff0000, "Here you can play:");
    ggprint8b(&r, 16, 0xff0000, "SLOTSS - DICE - BlackJack");
    ggprint8b(&r, 16, 0xff0000, "You will start out with 1000 GhettoBucks");
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
/*==========================GAME INFO END==============================*/


/*=======================INTRO ANIMATION START=========================*/
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
            intro_text();
        } else {
            introstart = false;
            introplay = 0;
        }
    }
}
void intro_text()
{
    Rect r;
    glPushMatrix();
    glTranslatef(640, 50, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "Press Space to Skip");
    glPopMatrix();

}
/*========================INTRO ANIMATION END==========================*/


/*=======================BLACKJACK LOGIC START=========================*/

void playBJ()
{
    drawBJBackground(1.0, 1.0, 1.0);
    drawBJShoe(g.xres / 1.13, g.yres / 1.77, 0, 3, 3, 1.0, 1.0, 1.0);
    initFirstHand();

    //bj.playerTurn = true;
    if (bj.playerBust == true || bj.dealerBust == true) {
        dealerHandrecheck();
        playerHandrecheck();
        bjPayout();
    }
    // animation here
    // checkDealerHand();
}
void hit()
{
    if (bj.dealerTurn == true) {
        bj.dealerHand[bj.dTotalCards - 1] = bj.shoe[bj.shoeCardNum];
        dealerHands(bj.shoeCardNum);
    }
    if (bj.playerTurn == true) {
        bj.playerHand[bj.dTotalCards - 1] = bj.shoe[bj.shoeCardNum];
        playerHands(bj.shoeCardNum);
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
    }// might delete above
    for (int i = 0; i < 5; i++) {
        //looks for higher total under 21
        if (bj.dealerHand[i] == 1 && bj.dealerHandTotal + 10 <= 21) {
            bj.dealerHandTotal =+ 10;
        }
        //looks to lower total if over 21
        if (bj.dealerHandTotal > 21) {
            if (bj.dealerHand[i] == 1) {
                bj.dealerHandTotal =- 10;
            }
        }
    }
}
void dealerCheckHand() // checks dealers first 2 cards
{
    //offer insurence
    //if (bj.dealerHand[0] == 1) {
    //    insureRender == true;
    //}
    //dealer 2 card 21, player bust
    if (bj.dealerHand[0] == 10 && bj.dealerHand[1] == 1) {
        bj.playerBust = true;
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
    }// might delete above
    for (int i = 0; i < 5; i++) {
        //looks for higher total under 21
        if (bj.playerHand[i] == 1 && bj.playerHandTotal + 10 <= 21) {
            bj.playerHandTotal =+ 10;
        }
        //looks to lower total if over 21
        if (bj.playerHandTotal > 21) {
            if (bj.playerHand[i] == 1) {
                bj.playerHandTotal =- 10;
            }
        }
    }

}
void bjPayout()
{
    //double down bet
    if (bj.dDown == true) {
        g.currentBet = g.currentBet * 2;
        if ((bj.playerHandTotal <= 21 &&
                    bj.playerHandTotal > bj.dealerHandTotal)
                || bj.playerHandTotal == 21) {
            g.currency =+ g.currentBet * 2;
        } else {
            g.currency =- g.currentBet;
        }
        //bj.dDown == false;
    }

    //player win
    if ((bj.playerHandTotal <= 21
                && bj.playerHandTotal > bj.dealerHandTotal)
            || bj.playerHandTotal == 21) {
        g.currency =+ g.currentBet;
        //dealer wins with 21 /  player bust  / dealer > player
    } else if (bj.dealerHandTotal == 21 || bj.playerHandTotal > 21
            || bj.dealerHandTotal > bj.playerHandTotal) {
        g.currency =- g.currentBet;
    } else if (bj.dealerHandTotal == 21 && bj.insure == true) {
        //insure
        g.currency =+ g.currentBet;
    }
    int payoutType = 0;
    if (bj.dDown == true) {
        g.currentBet = g.currentBet * 2;
        payoutType = 1;
    }
    if (bj.insure == true) {
        payoutType = 2;
    }
    if (bj.dDown == true && bj.insure == true) {
        payoutType = 3;
    }
    switch(payoutType) {
        //normal payout/lose
        case 0:
            if ((bj.playerHandTotal == 21) || (bj.playerHandTotal < 21 &&
                        (bj.playerHandTotal > bj.dealerHandTotal))) {
                g.currency =+ g.currentBet;
            } else {
                g.currency =- g.currentBet;
            }
            break;
            //double down
        case 1:
            if ((bj.playerHandTotal < 21 &&
                        (bj.playerHandTotal > bj.dealerHandTotal))
                    || bj.playerHandTotal == 21) {
                g.currency =+ g.currentBet;
            } else {
                g.currency =- g.currentBet;
            }
            bj.dDown = false;
            break;
            //insurance
        case 2:
            if ((bj.playerHandTotal == 21) || (bj.playerHandTotal < 21 &&
                        (bj.playerHandTotal > bj.dealerHandTotal))) {
                g.currency =+ g.currentBet;
            } else {
                g.currentBet = 0;
            }
            bj.insure = false;
            break;
            //insure and double
        case 3:
            if ((bj.playerHandTotal < 21 &&
                        (bj.playerHandTotal > bj.dealerHandTotal))
                    || bj.playerHandTotal == 21) {
                g.currency =+ g.currentBet;
            } else {
                g.currentBet = 0;
            }
            bj.dDown = false;
            bj.insure = false;
            break;
    }
}
bool hitRender = true;
bool standRender = true;
void bjButtonRender()
{
    if (bj.dDown == false && bj.playerTurn == true) {
        renderDoubleButton();
    }
    if (bj.playerTurn == true && hitRender == true) {
        renderHitButton();
    }
    if (bj.playerTurn == true && standRender == true) {
        renderStandButton();
    }
    bjInfoButton();
}
void bjButtonClick(int x, int y)
{
    //double down button
    if (x > 1095 && x < 1243 && y > 525 && y < 570) {
        cout << "downdown pressed\n";
        bj.dDown = true;
    }
    //hit button
    if (x > 1095 && x < 1243 && y > 405 && y < 450) {
        cout << "hit pressed\n";
        if (bj.playerTurn == true) {
            hit();
            bj.playerTurn = false;
            bj.dealerTurn = true;
        }//change this to else
        if (bj.dealerTurn == true) {
            hit();
            bj.dealerTurn = false;
            bj.playerTurn = true;
        }
    }
    //stand button
    if (x > 1095 && x < 1243 && y > 465 && y < 510) {
        cout << "stand pressed\n";
        if (bj.playerTurn == true) {
            standRender = false;
            hitRender = false;
            bj.playerTurn = false;
            bj.dealerTurn = true;
        }// change this to else
        if (bj.dealerTurn == true) {
            bj.dealerTurn = false;
            bj.playerTurn = true;
        }
    }
    //black jack infomation button
    if (x > 1160 && x < 1240 && y > 670 && y < 695) {
        renderBJInfo = !renderBJInfo;
    }
}
void bjInfoRender()
{
    if (renderBJInfo == true) {
        drawBJBackground(0.5, 0.5, 0.5);
        bjInfo();
        bjInfoButton();
    }
}
/*=======================BLACKJACK LOGIC END===========================*/
bool renderBJInfo = false;
void bjInfo()
{
    Rect r;
    glPushMatrix();
    glTranslatef(640, 625, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "RULES:");
    ggprint8b(&r, 16, 0xffffff, "Dealer must hit on 17\n");
    ggprint8b(&r, 16, 0xffffff, " ");
    ggprint8b(&r, 16, 0xffffff, "Hit:");
    ggprint8b(&r, 16, 0xffffff, "Draw another card");
    ggprint8b(&r, 16, 0xffffff, " ");
    ggprint8b(&r, 16, 0xffffff, "Stand:");
    ggprint8b(&r, 16, 0xffffff, "No longer be able to draw cards");
    ggprint8b(&r, 16, 0xffffff, " ");
    ggprint8b(&r, 16, 0xffffff, "Double:");
    ggprint8b(&r, 16, 0xffffff, "Will double your current bet");
    ggprint8b(&r, 16, 0xffffff, "You will only get 1 more card\n");
    ggprint8b(&r, 16, 0xffffff, " ");
    ggprint8b(&r, 16, 0xffffff, "INSURANCE:");
    ggprint8b(&r, 16, 0xffffff, "Offered if dealer first card is ACE");
    ggprint8b(&r, 16, 0xffffff, "If dealer gets 2 card blackjack");
    ggprint8b(&r, 16, 0xffffff, "You will not lose your money");
    ggprint8b(&r, 16, 0xffffff,
                        "Or you will get double ur money or something");
    glPopMatrix();
}
void renderDoubleButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1170, 170, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 6;
    float w = g.tex.buttonImage->width / 8;
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
    glTranslatef(1170, 160, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "DOUBLE");
    glPopMatrix();

}
void renderHitButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1170, 290, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 6;
    float w = g.tex.buttonImage->width / 8;
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
    glTranslatef(1170, 280, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "HIT");
    glPopMatrix();
}
void renderStandButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(1170, 230, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 6;
    float w = g.tex.buttonImage->width / 8;
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
    glTranslatef(1170, 220, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot = 0;
    r.left = 0;
    ggprint8b(&r, 16, 0xffffff, "STAND");
    glPopMatrix();
}
void bjInfoButton()
{
    glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
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

    Rect r;
    glPushMatrix();
    glTranslatef(1200, 23.5, 0);
    glScalef(2.0f, 2.0f, 1);
    r.bot  = 0;
    r.left = 0;
    if (renderBJInfo == false) {
        ggprint8b(&r, 16, 0xffffff, "INFO");
    } else if (renderBJInfo == true) {
        ggprint8b(&r, 16, 0xffffff, "BACK");
    }
    glPopMatrix();

}
/*
   void renderInsuranceButton()
   {
   glColor4f(0.9f, 0.9f, 0.9f, 0.8f);
   glPushMatrix();
   glTranslatef(1120, 160, 0);
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GREATER, 0.0f);
   glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
   glBegin(GL_QUADS);
   float h = g.tex.buttonImage->height / 6;
   float w = g.tex.buttonImage->width / 8;
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
   }*/
