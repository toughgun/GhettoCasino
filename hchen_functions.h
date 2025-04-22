#ifndef HCHEN_H
#define HCHEN_H

extern void intro_physics();
extern void game_info();
extern void intro_render();
extern int introplay;
extern bool introstart;

extern void dealerHands();
extern void playerHands();
extern void bjPayout();
extern void bjButtonRender();
extern void renderDoubleButton();
extern void renderSplitButton();
extern void renderInsuranceButton();
extern int splitVal;

extern void gameInfo();
extern void displayInfo();
extern void infoRetangle();
extern bool infoButton(int x, int y);
extern bool disInfo;

#endif
