#ifndef HCHEN_H
#define HCHEN_H

extern void intro_physics();
extern void game_info();
extern void intro_render();
extern int introplay;
extern bool introstart;

extern void dealerHands(int x);
extern void playerHands(int x);
extern void dealerHandrecheck();
extern void playerHandrecheck();
extern void bjPayout();
extern void bjButtonRender();
extern void renderDoubleButton();
extern void renderInsuranceButton();

extern void gameInfo(int xx);
extern void displayInfo();
extern void infoRetangle();
extern bool infoButton(int x, int y);
extern bool disInfo;

#endif
