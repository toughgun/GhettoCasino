#ifndef BOLAYVAR_H
#define BOLAYVAR_H

// menu stuff
extern int  click(int savex, int savey, int& done);
extern int  check_esc(int x);
extern void drawBackground();
extern void drawBJBackground();
extern void drawMenuLogo();
extern void drawMenuOptions(int x);
extern void drawButtonTxt();
extern void drawDevscreen();
extern void drawSlotFace();
void        beginBJPlay();
extern int  checkhover(int savex, int savey, int mouseposition);

// blackjack stuff
extern void initShoe();
#endif