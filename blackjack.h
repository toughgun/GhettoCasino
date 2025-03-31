#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
public:
    int maxCards;
    int playerHand[15];
    int dealerHand[15];
    int playerHandTotal, dealerHandTotal;
    int shoe[312];
    int marker;
    bool shuffled;
    GLuint cards;
    float cardWidth;
    float cardHeight;
    float cardPosX, cardPosY, cardVelX;

    Blackjack() {
        playerHandTotal = 0;
        dealerHandTotal = 0;
        maxCards = 312;
        marker = 0;
        shuffled = false;
    }
};

extern Blackjack bj;

#endif
