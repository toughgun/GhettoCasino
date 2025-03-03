// Author: Christian Rodriguez
// Last modified: 3/2/2025

#include <stdio.h>       // printf
#include <stdlib.h>      // exit, system
#include <unistd.h>      // fork
#include <sys/types.h>   // pid_t
#include <sys/wait.h>    // wait
#include <X11/Xlib.h>    // XOpenDisplay, XCreateSimpleWindow, etc.
#include <X11/Xutil.h>   // XMapWindow, XEvent, etc.
#include <X11/Xatom.h>   // For window properties, if needed

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
    XSelectInput(dpy, win, ExposureMask | KeyPressMask | StructureNotifyMask);

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
        // -------------------------
        // Child process
        // -------------------------
        show_child_credits_window();
        exit(0);
    } else if (cpid > 0) {
        // -------------------------
        // Parent process
        // -------------------------
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
