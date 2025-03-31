//Haonan Chen
//March 31, 2025
//
//has intro animation along with black jack hands

// #include "fonts.h"
// void show_hchen(Rect *r)
// {
//     ggprint8b(r, 0x00ff00ff, "Haonan");
// }
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
#include <time.h>
void game_info()
{
    /*
       Rect r;
       r.bot = 0;
       r.left = 0;
       ggprint8b(&r, 16, 0xffffff, "GHETTO CASINO");
       glPopMatrix();
       */
}

void intro_physics()
{
    //set image as logo
    //make logo physics as scrolling down
    g.tex.yc[0] -= 0.1;
    g.tex.yc[1] -= 0.1;
}

//set start time for when intro starts
//set intro playing as true
time_t start_time;
time_t current_time;
bool introplay = true;
void intro_render()
{
    //glClear(GL_COLOR_BUFFER_BIT);
    //get current time to check if its over 5 seconds
    //if not render game logo and call for intro_physics()
    start_time = time(NULL);
    current_time = time(NULL);
    while (current_time - start_time >= 150) {
        current_time = time(NULL);
        //intro_physics();
        drawMenuLogo();
    }
    introplay = false;
    //glClear(GL_COLOR_BUFFER_BIT);
}

void dealerHands()
{
    //checks if ACE was card
    if (bj.shoe[1] != 1) {
        //if not ace, checks if jack, queen, king
        if (bj.shoe[1] == 11 || bj.shoe[1] == 12 || bj.shoe[1] == 13) {
            bj.dealerHand[bj.shoe[1]] =+ bj.shoe[1];
            bj.dealerHandTotal =+ 10;
        } else {
            bj.dealerHand[bj.shoe[1]] =+ bj.shoe[1];
            bj.dealerHandTotal =+ bj.shoe[1];
        }
    } else {
        if (bj.dealerHandTotal + bj.shoe[1] <= 21)
            bj.dealerHandTotal =+ bj.shoe[2];
        else
            bj.dealerHandTotal =+ 1;
    }

}
void playerHands()
{
    //checks if ACE was card
    if (bj.shoe[2] != 1) {
        //if not ace, checks for jack, queen, king
        if (bj.shoe[2] == 11 || bj.shoe[2] == 12 || bj.shoe[2] == 13) {
            bj.playerHand[bj.shoe[2]] =+ bj.shoe[2];
            bj.dealerHandTotal =+ 10;
        } else {
            bj.playerHand[bj.shoe[2]] =+ bj.shoe[2];
            bj.playerHandTotal =+ bj.shoe[2];
        }
    } else {
        if (bj.playerHandTotal + bj.shoe[2] <= 21)
            bj.playerHandTotal =+  11;
        else
            bj.playerHandTotal =+ 1;
    }
}
/*void show_hchen(Rect *r)
  {

  ggprint8b(r, 0x00ff00ff, "Haonan");
  }*/

