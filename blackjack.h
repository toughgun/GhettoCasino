#ifndef BLACKJACK_H
#define BLACKJACK_H

class Blackjack {
public:
    int maxCards;
    int playerHand, dealerHand;
    int shoe[312];
    int marker;
    bool shuffled;
    GLuint cards;
    float cardWidth;
    float cardHeight;
    float cardPosX, cardPosY, cardVelX;

    Blackjack() {
        playerHand = 0;
        dealerHand = 0;
        maxCards = 312;
        marker = 0;
        shuffled = false;
    }
};

extern Blackjack bj;

#endif