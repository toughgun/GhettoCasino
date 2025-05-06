#ifndef HCHEN_H
#define HCHEN_H

extern void gameInfo(int xx);
extern void displayInfo();
extern void infoRetangle();
extern bool infoButton(int x, int y);
extern bool disInfo;

extern void intro_physics();
extern void intro_render();
extern int introplay;
extern bool introstart;
extern void intro_text();

extern void playBJ();
extern void hit();
extern void dealerHands(int x);
extern void playerHands(int x);
extern void dealerCheckHand();
extern void dealerHandrecheck();
extern void playerHandrecheck();
extern void bjPayout();
extern bool standRender;
extern bool hitRender;

extern bool renderBJInfo;
extern void bjButtonRender();
extern void bjButtonClick(int x, int y);
extern void renderDoubleButton();
extern void renderHitButton();
extern void renderStandButton();
extern void bjInfoRender();
extern void bjInfo();
extern void bjInfoButton();
#endif
