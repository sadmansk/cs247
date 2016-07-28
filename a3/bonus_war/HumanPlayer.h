#ifndef HUMAN_PLAYER_H_
#define HUMAN_PLAYER_H_

#include "Player.h"

class HumanPlayer : public Player {
public:
    virtual Card* play();
private:
    void printHand();
};

#endif
