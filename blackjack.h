#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
  public:
    int  maxCards;
    int  playerHand[15];
    int  dealerHand[15];
    int  playerHandTotal, dealerHandTotal;
    int  currentPos;
    int  shoe[312];
    int  discard[300];
    int  marker;
    bool shuffled;
    bool delt;
    bool split;
    bool insure;
    // GLuint cards;
    float cardWidth;
    float cardHeight;
    float showUI;
    float cardPosX, cardPosY, cardVelX;

    Blackjack()
    {
        playerHandTotal = 0;
        dealerHandTotal = 0;
        currentPos      = 0;
        maxCards        = 312;
        marker          = 0;
        shuffled        = false;
        delt            = false;
        split           = false;
        insure          = false;
        showUI          = false;
    }
};

extern Blackjack bj;

#endif
