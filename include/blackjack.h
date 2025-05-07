#ifndef BLACKJACK_H
#define BLACKJACK_H

struct Card {
    int value; // 1–13 (Ace to King)
    int suit;  // 0–3 (Clubs, Diamonds, Hearts, Spades)
};

class Blackjack {
  public:
    int  maxCards;
    int  playerHand[5];
    int  dealerHand[5];
    int  betarray[100];
    int  betarraypointer;
    int  playerHandTotal, dealerHandTotal;
    int  currentPos;
    Card shoe[312];
    Card discard[300];
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
    bool playerStand;
    bool dealerStand;
    bool dealFirstHand;
    int pTotalCards;
    int dTotalCards;
    int shoeCardNum;

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
        playerStand     = false;
        dealerStand     = false;
        dealFirstHand   = false;
        pTotalCards     = 0;
        dTotalCards     = 0;
        shoeCardNum     = 0;
    }
};

extern Blackjack bj;

#endif
