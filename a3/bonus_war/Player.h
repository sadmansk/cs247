#ifndef PLAYER_H_
#define PLAYER_H_

#include <vector>
#include "Card.h"

class Player {
public:
    virtual ~Player();                  // contrutor
    virtual void addCard(Card* card);   // add card to the hand
    virtual void incScore();            // increment score
    virtual int score();               // accessor for the score
    virtual Card* play() = 0;           // play a card
protected:
    std::vector<Card*> hand_;           // store the hand of the player
    int score_;
};

#endif
