#ifndef COMPUTER_PLAYER_H_
#define COMPUTER_PLAYER_H_

#include "Player.h"

// break out the CPU strategies
class TopPlayer : public Player {
public:
    Card* play();
};

class RandPlayer : public Player {
public:
    Card* play();
};

#endif
