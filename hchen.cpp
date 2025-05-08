//Haonan Chen
//May 7, 2025
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
#include "include/hchen_functions.h"
#include "include/global.h"
#include "include/image.h"
#include "include/bolayvar.h"
#include "include/blackjack.h"
#include "include/fonts.h"
#include "include/button.h"
#include <ctime>
#include <algorithm>
#include <iostream>
using namespace std;

bool disInfo = false;

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
    ggprint8b(&r, 16, 0xff0000, "SLOTS - DICE - Black Jack");
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

float centerX;
float centerY;
float logoPosY = 0.0f;
float logoFade = 0.0f;
float backFade = 0.0f;
time_t current_time = 0;
time_t start_time = 0;
int introplay = 1;
bool introstart = false;
/*=======================INTRO ANIMATION START=========================*/
void init_background()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(1.0f, 1.0f, 1.0f, backFade);

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
    g.tex.logo_xc[0] = 0.0f;
    g.tex.logo_xc[1] = 1.0f;
    g.tex.logo_yc[0] = 0.0f;
    g.tex.logo_yc[1] = 1.0f;
    logoPosY = g.yres * 0.8f;
}
void intro_logo()
{
    centerX = g.xres / 2.0f;
    centerY = logoPosY;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, logoFade);
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
    glDisable(GL_BLEND);
}
void callMenuButton(int x, int y)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(1.0f, 1.0f, 1.0f, backFade);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.buttontex);
    glBegin(GL_QUADS);
    float h = g.tex.buttonImage->height / 4;
    float w = g.tex.buttonImage->width / 4;
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(-w, h);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(w, h);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(w, -h);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(-w, -h);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
    glDisable(GL_BLEND);
}
void callAllMenuButton()
{
    callMenuButton(g.xres / 2, 430);
    callMenuButton(g.xres / 2, 430 - 87);
    callMenuButton(g.xres / 2, 430 - (87 * 2));
    callMenuButton(g.xres / 2, 430 - (87 * 3));
}
void intro_physics()
{
    float elapsed = time(nullptr) - start_time;
    float offset;

    if (elapsed < 9.0f && elapsed > 1.0f) {
        logoFade += 0.03f;
        offset = sinf(elapsed * 2.0f) * 0.01f;
        g.tex.logo_yc[0] += offset;
        g.tex.logo_yc[1] += offset;
    }
    if (elapsed > 5.0f) {
        backFade += 0.08f;
    }
}
void intro_render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (introplay == 2) {
        if (!introstart) {
            init_intro_logo();
            start_time = time(nullptr);
            //logoPosy = g.yres + 100.0f;
            introstart = true;
        }

        current_time = time(nullptr);
        if (current_time - start_time < 9.0f) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            intro_logo();
            intro_physics();
            init_background();
            callAllMenuButton();
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

int payoutType = 0;
/*=======================BLACKJACK LOGIC START=========================*/

void playBJ()
{
    drawBJBackground(1.0, 1.0, 1.0);
    drawBJShoe(g.xres / 1.13, g.yres / 1.77, 0, 3, 3, 1.0, 1.0, 1.0);
    initFirstHand();
    check2CardBJ();
    //dealer playing
    if (bj.dealerTurn == true || bj.playerStand == true) {
        dealerPlay();
        bj.dealerTurn = false;
        if (bj.playerStand == false) {
            bj.playerTurn = true;
        } else {
            bj.dealerTurn = true;
        }
    }
    //checks for bust and does payout & resets values
    if (bj.playerBust == true || bj.dealerBust == true
            || (bj.dealerStand == true && bj.playerStand == true)) {
        bjPayout();
        cout << "Game Reseting\n";
        for (int i = 0; i < 5; i++) {
            bj.playerHand[i] = 0;
            bj.dealerHand[i] = 0;
        }
        bj.playerTurn = true;
        bj.dealerTurn = false;
        bj.playerBust = false;
        bj.dealerBust = false;
        bj.pTotalCards = 0;
        bj.dTotalCards = 0;
        payoutType = 0;
        bj.playerHandTotal = 0;
        bj.dealerHandTotal = 0;
        bj.playerStand = false;
        bj.dealerStand = false;
        bj.dealFirstHand = false;
        standRender = true;
        hitRender = true;
        doubleRender = true;
        bj.dealFirstHand = false;
    }
}
void check2CardBJ()
{
    if (((bj.dealerHand[0] >= 10 && bj.dealerHand[1] == 1) ||
                (bj.dealerHand[0] == 1 && bj.dealerHand[1] >= 10)) &&
                ((bj.playerHand[0] >= 10 && bj.playerHand[1] == 1) ||
                (bj.playerHand[0] == 1 && bj.playerHand[1] >= 10))) {
        payoutType = 4;
    }
    if ((bj.dealerHand[0] >= 10 && bj.dealerHand[1] == 1) ||
                (bj.dealerHand[0] == 1 && bj.dealerHand[1] >= 10)) {
        bj.playerBust = true;
    }
    if ((bj.playerHand[0] >= 10 && bj.playerHand[1] == 1) ||
                (bj.playerHand[0] == 1 && bj.playerHand[1] >= 10)) {
        bj.dealerBust = true;
    }
}
void hit()
{
    if (bj.dealerTurn && bj.dealerHandTotal <= 17) {
        //cout << "dealer hit\n";
        Card newCard = bj.shoe[bj.shoeCardNum++];
        bj.dealerHand[bj.dTotalCards++] = newCard.value;
        bj.dealerHandTotal += (newCard.value > 10)
                    ? 10 : (newCard.value == 1 ? 11 : newCard.value);
        printf("**DEALER DRAWED %i\n", newCard.value);
        printf("**DEALER HAND CURRENT TOTAL VALUE: %i**\n",
                                            bj.dealerHandTotal);
        cout << "**DEALER CURRENT HAND: ";
        for (int i = 0; i < 5; i++) {
            cout << bj.dealerHand[i] << " ";
        }
        if (bj.dealerHandTotal == 21) {
            bj.playerBust = true;
        }
        cout << endl;
        printf("**DEALER TOTAL CARDS: %i**\n", bj.dTotalCards);
        //checks for dealer bust
        if (bj.dealerHandTotal > 21) {
            cout << "**CHECKING FOR DEALER BUST**\n";
            bj.dealerHandTotal = 0;
            sortHands();
            bj.dealerHandTotal = dealerHands();
            if (bj.dealerHandTotal > 21) {
                printf("Dealer bust current total: %i\n",
                                                    bj.dealerHandTotal);
                bj.dealerBust = true;
                bj.dealerTurn = false;
                bj.playerTurn = false;
            }
        }
    }
    if (bj.playerTurn == true && bj.playerStand == false) {
        //cout << " player hit\n";
        Card newCard = bj.shoe[bj.shoeCardNum++];
        bj.playerHand[bj.pTotalCards++] = newCard.value;
        bj.playerHandTotal += (newCard.value > 10)
                    ? 10 : (newCard.value == 1 ? 11 : newCard.value);
        printf("--PLAYER DRAWED %i--\n", newCard.value);
        printf("--PLAYER HAND CURRENT VALUE: %i--\n", bj.playerHandTotal);
        cout << "--PLAYER CURRENT HAND: ";
        for (int i = 0; i < 5; i++) {
            cout << bj.playerHand[i] << " ";
        }
        if ( bj.playerHandTotal == 21) {
            bj.dealerBust = true;
        }
        cout << endl;
        printf("--PLAYER TOTAL CARDS: %i--\n", bj.pTotalCards);
        if (bj.playerHandTotal > 21 || bj.playerBust == true) {
            cout << "--CHECKING FOR PLAYER BUST--\n";
            bj.playerHandTotal = 0;
            sortHands();
            bj.playerHandTotal = playerHands();
            if (bj.playerHandTotal > 21) {
                printf("--PLAYER BUSTED CURRENT TOTAL: %i--\n",
                                                    bj.playerHandTotal);
                bj.playerBust = true;
                bj.dealerTurn = false;
                bj.playerTurn = false;
            }
        }
        if (bj.dDown == true && bj.playerTurn == true) {
            bj.playerStand = true;
        }
    }
}
void dealerPlay()
{
    if (bj.dealerHandTotal <= 17 && bj.dTotalCards < 5) {
        cout << "**DEALER CHOOSEN HIT**\n";
        hit();
        bj.dealerTurn = false;
        bj.playerTurn = true;
    } else {
        bj.dealerTurn = false;
        bj.dealerStand = true;
        printf("**DEALER CHOOSEN STAND**\n");
    }
}
//void dealerHands(int x)
int dealerHands()
{
    int dealerHandValue = 0;
    int aceCount = 0;

    // First pass: Count non-ace cards
    for (int i = 0; i < 5; i++) {
        if (bj.dealerHand[i] == 0) {
            continue;
        } else if (bj.dealerHand[i] == 1) {
            aceCount++;
        } else if (bj.dealerHand[i] > 10) {
            dealerHandValue += 10;
        } else {
            dealerHandValue += bj.dealerHand[i];
        }
    }

    int bestTotal = dealerHandValue + aceCount;

    if (aceCount > 0 && dealerHandValue + 11 + (aceCount-1) <= 21) {
        bestTotal = dealerHandValue + 11 + (aceCount-1);
    }
    printf("**DEALER HAND VALUE AFTER BUST CHECK: %i**\n", bestTotal);
    return bestTotal;
}
//void playerHands(int x)
int playerHands()
{
    int playerHandValue = 0;
    int aceCount = 0;

    for (int i = 0; i < 5; i++) {
        if (bj.playerHand[i] == 0) {
            continue;
        } else if (bj.playerHand[i] == 1) {
            aceCount++;
        } else if (bj.playerHand[i] > 10) {
            playerHandValue += 10;
        } else {
            playerHandValue += bj.playerHand[i];
        }
    }

    int bestTotal = playerHandValue + aceCount;

    if (aceCount > 0 && playerHandValue + 11 + (aceCount-1) <= 21) {
        bestTotal = playerHandValue + 11 + (aceCount-1);
    }

    printf("--PLAYLER HAND VALUE AFTER BUST CHECK: %i--\n", bestTotal);
    return bestTotal;
}
void sortHands()
{
    int dealersize = sizeof(bj.dealerHand) / sizeof(bj.dealerHand[0]);
    sort(bj.dealerHand, bj.dealerHand + dealersize, greater<int>());
    int playersize = sizeof(bj.playerHand) / sizeof(bj.playerHand[0]);
    sort(bj.playerHand, bj.playerHand + playersize, greater<int>());
}
void dealerCheckHand()
{
    if (bj.dealerHand[0] == 1) {
    }
}
void bjPayout()
{
    //int payoutType = 0;
    if (bj.dDown == true) {
        payoutType = 1;
        printf("Payment type set to double\n");
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
        if (bj.dealerBust == true || (bj.playerHandTotal <= 21
                        && bj.playerHandTotal > bj.dealerHandTotal)) {
            printf("--YOU WON %i BUCKS--\n", g.currentBet);
            g.currency += g.currentBet * 2;
            printf("--CURRENT CURRENCY: %i--\n", g.currency);
        } else if (bj.dealerHandTotal == bj.playerHandTotal) {
            printf("--DRAW--\n");
            g.currency += g.currentBet;
        } else {
            printf("--YOU LOST %i BUCKS--\n", g.currentBet);
            //g.currency -= g.currentBet;
            printf("--CURRENT CURRENCY: %i--\n", g.currency);
        }
            break;
        //double down
        case 1:
            if (bj.dealerBust == true || (bj.playerHandTotal <= 21
                            && bj.playerHandTotal > bj.dealerHandTotal)) {
                printf("--YOU WON %i BUCKS--\n", g.currentBet * 2);
                g.currency += g.currentBet * 3;
                printf("--CURRENT CURRENCY: %i--\n", g.currency);
            } else if (bj.dealerHandTotal == bj.playerHandTotal) {
                printf("--DRAW--");
                g.currency += g.currentBet;
            } else {
                printf("--YOU LOST %i BUCKS--\n", g.currentBet);
                g.currency -= g.currentBet;
                printf("--CURRENT CURRENCY: %i--\n", g.currency);
            }
            bj.dDown = false;
            break;
        //insurance
        case 2:
            if (bj.dealerBust == true || (bj.playerHandTotal < 21
                            && bj.playerHandTotal > bj.dealerHandTotal)) {
                printf("--YOU WON %i BUCKS--\n", g.currentBet);
                g.currency += g.currentBet;
                printf("Curreny: %i\n", g.currency);
            } else if (bj.dealerHandTotal == bj.playerHandTotal) {
                printf("--DRAW--\n");
                g.currentBet = 0;
            } else {
                printf("--YOU LOST %i BUCKS--\n", g.currentBet);
                //g.currency -= g.currentBet;
                printf("--CURRENT CURRENY: %i--\n", g.currency);
            }
            bj.insure = false;
            break;
            //insure and double
        case 3:
            if ((bj.playerHandTotal < 21 &&
                        (bj.playerHandTotal > bj.dealerHandTotal))
                    || bj.playerHandTotal == 21) {
                printf("You won %i bucks\n", g.currentBet);
                g.currency += g.currentBet;
                printf("Curreny: %i\n", g.currency);
            } else if (bj.dealerHandTotal == bj.playerHandTotal) {
                printf("--DRAW--\n");
                g.currentBet = 0;
                printf("--CURRENT CURRENCY: %i--\n", g.currency);
            } else {
                printf("You lost %i bucks\n", g.currentBet);
                //g.currentBet = 0;
                printf("Curreny: %i\n", g.currency);
            }
            bj.dDown = false;
            bj.insure = false;
            break;
        case 4:
            printf("--DRAW--\n");
            g.currency += g.currentBet;
            printf("--CURRENT CURRENCY: %i--\n", g.currency);
            break;
    }
}
bool hitRender = true;
bool standRender = true;
bool doubleRender = true;
void bjButtonRender()
{
    if (bj.dDown == false && bj.playerTurn == true
                                && doubleRender == true) {
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
        if (bj.dDown == false) {
            //g.currentBet = g.currentBet * 2;
            printf("--PLAYER CHOOSEN DOUBLEDOWN BET IS NOW: %i--\n",
                                                     g.currentBet * 2);
            bj.dDown = true;
            doubleRender = false;
            hitRender = false;
            standRender = false;
            if (bj.playerTurn == true) {
                hit();
            }
            bj.playerTurn = false;

            if (bj.dealerStand == false) {
                bj.dealerTurn = true;
            }
        }
    }
    //hit button
    if (x > 1095 && x < 1243 && y > 405 && y < 450) {
        if (bj.playerTurn == true && bj.pTotalCards < 5
                                && bj.playerStand == false) {
            cout << "--PLAYER CHOOSEN HIT--\n";
            hit();
            bj.playerTurn = false;
            bj.dealerTurn = true;
        } else if (bj.pTotalCards == 5){
            bj.playerStand = true;
        }
    }
    //stand button
    if (x > 1095 && x < 1243 && y > 465 && y < 510) {
        cout << "--PLAYER CHOOSEN STAND--\n";
        if (bj.playerTurn == true && bj.playerStand == false) {
            standRender = false;
            hitRender = false;
            doubleRender = false;
            bj.playerTurn = false;
            bj.playerStand = true;
            if (bj.dealerBust == false){
                bj.dealerTurn = true;
            }
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
