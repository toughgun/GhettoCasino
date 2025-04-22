#ifndef CRODRIGUEZ4_H
#define CRODRIGUEZ4_H

#include "global.h"
#include "image.h"
#include <algorithm>

/*---------------------------------------------------------------------
 * Dice‑module public interface
 * Author: Christian Rodriguez          Last modified: 2025‑04‑21
 *----------------------------------------------------------------------*/

/* ---------- rendering ---------- */
extern void render_dice(void);
extern void renderBettingUI(void);
extern void renderChoiceUI(void);
extern void renderResultUI(void);          /* dice faces + buttons */
extern void renderRevealButton(void);
extern void renderResult(void);
extern void drawCup(void);
extern void draw_button(float x, float y, float w, float h, const char *label);
void draw_button_colored(float x,float y,float w,float h,
                const char *label,float r,float g,float b);
extern void show_credits(void);

/* ---------- dice control ---------- */
extern void roll_dice(void);
extern void reveal_dice(void);
extern void Start_Dice(void);

/* ---------- betting control ---------- */
extern void processBettingInput(int mx, int my, int button);
extern void finalizeBet(void);
extern void resetBet(void);
extern void toggleBetMode(void);


/* ---------- helpers ---------- */
extern void loadCupTexture(void);
extern void loadDiceTextures(void);
extern void updateUIForWindowSize(void);
extern unsigned char *buildAlphaData(Image *img);

/* ---------- key input ---------- */
extern void handleDiceKeys(KeySym key);
extern void handleChoiceInput(int mx, int my);
extern void handleRevealClick(int mx, int my);
extern void handleResultInput(int mx, int my);

/* ---------- state ---------- */
enum Choice { UNDER, OVER, EXACT, NONE };
extern bool   diceRevealed;
extern int    die1, die2, total;
extern Choice playerChoice;
extern bool   addMode;
extern int    gameState;

enum class ResultState { IDLE, SHOW_WIN, SHOW_LOSS };
extern bool bettingUIActive;
extern bool choiceUIActive;   // show Over/Under/Exact buttons
extern bool revealUIActive;   // show the REVEAL button
extern bool resultUIActive;   /* true after Reveal */
extern ResultState resultState;

#endif
