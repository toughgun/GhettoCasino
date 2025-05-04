#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
  public:
    int  maxCards;
    int  playerHand[5];
    int  dealerHand[5];
    int  betarray[100];
    int  betarraypointer;
    int  playerHandTotal, dealerHandTotal;
    int  currentPos;
    int  shoe[312];
    int  discard[300];
    int  marker;
    bool shuffled;
    bool delt;
    bool split;
    bool insure;
    bool dDown;
    bool initialhand;
    bool gameInProgress;
    bool wait;
    bool dealerTurn;
    bool playerTurn;
    bool dealerBust;
    bool playerBust;
    int pTotalCards;
    int dTotalCards;

    // GLuint cards;
    float cardWidth;
    float cardHeight;
    float showUI;
    float allIn;
    float cardPosX, cardPosY, cardVelX;

    Blackjack()
    {
        playerHandTotal = 0;
        dealerHandTotal = 0;
        currentPos      = 0;
        maxCards        = 312;
        marker          = 0;
        betarraypointer = 0;
        shuffled        = false;
        delt            = false;
        split           = false;
        insure          = false;
        showUI          = false;
        allIn           = false;
        initialhand     = false;
        gameInProgress  = false;
        wait            = false;
        dealerTurn      = false;
        playerTurn      = false;
        dDown           = false;
        dealerBust      = false;
        playerBust      = false;
        pTotalCards     = 0;
        dTotalCards     = 0;
    }
};

extern Blackjack bj;

#endif
