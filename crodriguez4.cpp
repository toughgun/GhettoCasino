// Author: Christian Rodriguez
// Last modified: 3/11/2025

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>  // Needed for cos() and sin()
#include <GL/gl.h>
#include <GL/glx.h>

using namespace std;

// Function Prototypes
void refresh_screen();
void draw_button(float x, float y, float width, float height, 
                                                    const char* label);
void show_menu();
void render_slots();
void render_dice();
void render_blackjack();
void drawCup();
void drawDice(float x, float y, float z);
void handle_keypress(char key);
void show_credits();

int gameState = 0;  // 0 = Menu, 1 = Slots, 2 = Dice, 3 = Blackjack

// Dice Game Variables
int die1 = 0, die2 = 0, total = 0;
bool diceRevealed = false;
enum Choice { UNDER, OVER, EXACT, NONE };
Choice playerChoice = NONE;

void refresh_screen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    switch (gameState) {
        case 0:
            cout << "[STATE] Menu selected.\n";
            show_menu();
            break;
        case 1:
            cout << "[STATE] Slots selected.\n";
            render_slots();
            break;
        case 2:
            cout << "[STATE] Dice selected.\n";
            render_dice();
            break;
        case 3:
            cout << "[STATE] Blackjack selected.\n";
            render_blackjack();
            break;
    }

    glFlush();
}

void draw_button(float x, float y, float width, float height,
                                                     const char* label) {
    glColor3f(0.2f, 0.6f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glVertex2f(x, y - height);
    glEnd();
}

void show_menu() {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_button(-0.3f, 0.5f, 0.6f, 0.2f, "Slots (1)");
    draw_button(-0.3f, 0.2f, 0.6f, 0.2f, "Dice (2)");
    draw_button(-0.3f, -0.1f, 0.6f, 0.2f, "Blackjack (3)");
    draw_button(-0.3f, -0.4f, 0.6f, 0.2f, "Credits (C)");
}

void render_slots() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cout << "[INFO] Rendering Slots game.\n";
    glFlush();
}

void render_dice() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    cout << "[INFO] Rendering Dice game.\n";
    if (!diceRevealed) {
        drawCup();
    } else {
        drawDice(-1, 0, 0);
        drawDice(1, 0, 0);
    }

    glFlush();
}

void render_blackjack() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    cout << "[INFO] Rendering Blackjack game.\n";
    glFlush();
}

void roll_dice() {
    srand(time(0));
    die1 = rand() % 6 + 1;
    die2 = rand() % 6 + 1;
    total = die1 + die2;
    diceRevealed = false;
    cout << "[INFO] Dice rolled. Awaiting user choice...\n";
}

void reveal_dice() {
    diceRevealed = true;
    cout << "[INFO] Revealing dice: " << die1 << " and ";
    cout << die2 << " (Total: " << total << ")\n";

    if ((playerChoice == UNDER && total < 7) ||
        (playerChoice == OVER && total > 7) ||
        (playerChoice == EXACT && total == 7)) {
        cout << "[RESULT] You won!\n";
    } else {
        cout << "[RESULT] You lost. Try again!\n";
    }
}

void drawDice(float x, float y, float z) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glBegin(GL_QUADS);
    glVertex3f(-0.5, -0.5, 0.5); glVertex3f(0.5, -0.5, 0.5);
    glVertex3f(0.5, 0.5, 0.5); glVertex3f(-0.5, 0.5, 0.5);
    glEnd();
    glPopMatrix();
}

void drawCup() {
    glPushMatrix();
    static float angle = 0.0;
    angle += 10.0;
    glRotatef(angle, 0, 1, 0);
    glBegin(GL_QUADS);
    for (int i = 0; i < 360; i += 30) {
        float rad = i * M_PI / 180.0;
        float nextRad = (i + 30) * M_PI / 180.0;
        glVertex3f(cos(rad), sin(rad), 0.0);
        glVertex3f(cos(nextRad), sin(nextRad), 0.0);
        glVertex3f(cos(nextRad), sin(nextRad), 1.5);
        glVertex3f(cos(rad), sin(rad), 1.5);
    }
    glEnd();
    glPopMatrix();
}

void handle_keypress(char key) {
    switch (key) {
        case '1': gameState = 1; refresh_screen(); break;
        case '2': gameState = 2; refresh_screen(); break;
        case '3': gameState = 3; refresh_screen(); break;
        case '0': gameState = 0; refresh_screen(); break;
        case 'c': case 'C': show_credits(); break;
    }
}

/* Credits functionality */
// Function to display credits window using X11 fonts
static void show_child_credits_window()
{
    // Open a display
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Child: Could not open display.\n");
        return;
    }

    // Create a window
    int screen = DefaultScreen(dpy);
    Window root = DefaultRootWindow(dpy);
    int winWidth = 350;
    int winHeight = 250;

    Window win = XCreateSimpleWindow(
        dpy, root,
        100, 100,             // window x/y on screen
        winWidth, winHeight,  
        1,                    // border width
        BlackPixel(dpy, screen),     // border color
        WhitePixel(dpy, screen));    // background color

    // Set window title
    XStoreName(dpy, win, "Credits");

    // Select input events
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | 
                                                StructureNotifyMask);

    // Show window
    XMapWindow(dpy, win);

    // Load a larger font
    XFontStruct *font = XLoadQueryFont(dpy, "10x20");
    if (!font) {
        fprintf(stderr, "Failed to load font.\n");
        XDestroyWindow(dpy, win);
        XCloseDisplay(dpy);
        return;
    }

    // Create a graphics context (GC)
    GC gc = XCreateGC(dpy, win, 0, NULL);
    XSetFont(dpy, gc, font->fid);
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));

    bool done = false;
    while (!done) {
        XEvent e;
        XNextEvent(dpy, &e);

        if (e.type == Expose) {
            // Draw text when window is exposed
            XDrawString(dpy, win, gc, 20, 50, "Credits:", 8);
            XDrawString(dpy, win, gc, 20, 80, "  Christian Rodriguez", 21);
            XDrawString(dpy, win, gc, 20, 110, "  Philp Lakes", 13);
            XDrawString(dpy, win, gc, 20, 140, "  Haonan Chen", 13);
            XDrawString(dpy, win, gc, 20, 170, "  Ben", 5);
        }

        if (e.type == KeyPress) {
            done = true;
        }
        if (e.type == DestroyNotify) {
            done = true;
        }
    }

    // Cleanup
    XFreeFont(dpy, font);
    XFreeGC(dpy, gc);
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

// Function to show credits
void show_credits()
{
    pid_t cpid;
    int status;

    cpid = fork();
    if (cpid == 0) {
        // Child process
        show_child_credits_window();
        exit(0);
    } else if (cpid > 0) {
        // Parent process
        wait(&status);
        if (WIFEXITED(status)) {
            int es = WEXITSTATUS(status);
            printf("Child (credits) exited with status %d\n", es);
        } else {
            printf("Child did not exit normally.\n");
        }
    } else {
        perror("fork");
    }
}
