#include <iostream>
#include <cstdlib>

#include "Deck.h"
#include "Game.h"

int main (int argc, char* argv[]) {
    // Check if the user provided a seed to work with
    if (argc > 1) {
        // update the static global RNG seed
        Deck::rng_seed = atoi(argv[1]);
    }

    Game game;
    game.startGame();

    return 0;
}
