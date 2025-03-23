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
time_t start_time = time(NULL);
time_t current_time;
bool introplay = true;
void intro_render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //get current time to check if its over 5 seconds
    //if not render game logo and call for intro_physics()
    current_time = time(NULL);
    while (current_time - start_time >= 5) {
        intro_physics();
        /*
    glPushMatrix();
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBindTexture(GL_TEXTURE_2D, g.tex.menulogotex);
    glColor4f(1.0f, 1.0f, 1.0f, 0.8f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(0,0);
        //glVertex2f(-g.tex.menuLogo/2, -g.tex.menuLogo/2);

        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(0,g.yres);
        //glVertex2f( g.tex.menuLogo/2, -g.tex.menuLogo/2);

        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(g.xres, g.yres);
        //glVertex2f( g.tex.menuLogo/2,  g.tex.menuLogo/2);

        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(g.xres, 0);
        //glVertex2f(-g.tex.menuLogo/2,  g.tex.menuLogo/2);
    glEnd();
    glDisable(GL_ALPHA_TEST);
    glPopMatrix();
    */
    }
    //set intro playing to false
    introplay = false;
}

 void drawString(int left, int top, char *str)
{
//    XDrawString(100, 200, g.glc, left, top, "gameinfo", 8);

}


/*void show_hchen(Rect *r)
{

    ggprint8b(r, 0x00ff00ff, "Haonan");
}*/
