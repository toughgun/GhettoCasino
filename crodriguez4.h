#ifndef CRODRIGUEZ4_H
#define CRODRIGUEZ4_H

#include "global.h"
#include "image.h"

extern void render_dice();
extern void drawCup();
extern void draw_button(float x, float y, float width, float height,
                         const char* label);
extern void roll_dice();
extern void reveal_dice();
extern void render_blackjack();
extern void render_slots();
extern void show_credits();
extern void loadCupTexture();
extern void Start_Dice();

// Dice game state variables:
extern bool diceRevealed;
extern int die1, die2, total;
enum Choice { UNDER, OVER, EXACT, NONE };
extern Choice playerChoice;

// Betting function prototypes:
extern void renderBettingUI();
extern void processBettingInput(int mouseX, int mouseY, int button);
extern void finalizeBet();
extern void resetBet();

extern unsigned char *buildAlphaData(Image *img);

#endif