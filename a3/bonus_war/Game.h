#ifndef GAME_H_
#define GAME_H_

#include "Deck.h"
#include <array>
#include "Player.h"

#define NUM_PLAYERS 2
#define HAND_SIZE 26

class Game {
public:
    Game();                                     // constructor
    ~Game();                                    // destructor
    void startGame();                           // start the game
    void run();                                 // game loop
private:
    std::array<Player*, NUM_PLAYERS> players_;  // container for the two players
    Deck* deck_;                                // store the deck
};

#endif
