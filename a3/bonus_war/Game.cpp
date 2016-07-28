#include "Game.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"

#include <iostream>

Game::Game() {
    // initialize the deck
    deck_ = new Deck();
    
    // pick players
    for (int i = 0; i < NUM_PLAYERS; i++) {
        std::cout << "Please pick player " << i+1 << " (h - human, r - random strategy CPU, t - top card strategy CPU): ";
        bool valid;
        do {
            valid = true;
            std::string type; 
            // take the first character
            std::cin >> type;
            switch (type[0]) {
                case 'h':
                    players_[i] = new HumanPlayer();
                    break;
                case 'r':
                    players_[i] = new RandPlayer();
                    break;
                case 't':
                    players_[i] = new TopPlayer();
                    break;
                default:
                    valid = false;
                    std::cout << "Please enter a valid player type! - ";
            }
        } while (!valid);
    }
}

Game::~Game() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        delete players_[i];
    }
    delete deck_;
}

void Game::startGame() {
    // shuffle the deck
    deck_->shuffle();

    // deal the cards
    for (int i = 0; i < NUM_PLAYERS; i++) {
        for (int j = 0; j < HAND_SIZE; j++) {
            players_[i]->addCard(deck_->getCardAt((i*HAND_SIZE) + j));
        }
    }

    // run the game loop
    run();
}

void Game::run() {
    for (int turn = 0; turn < HAND_SIZE; turn++) {
        std::cout << "\nNew round begins!" << std::endl;
        // assume two players here
        // store the played cards
        Card* card1 = players_[0]->play();
        Card* card2 = players_[1]->play();

        // print the played cards
        std::cout << "Player 1 played " << *card1 << std::endl;
        std::cout << "Player 2 played " << *card2 << std::endl;

        if (*card2 < *card1) {
            std::cout << "Player 1 ";
            players_[0]->incScore();
        }
        else {
            std::cout << "Player 2 ";
            players_[1]->incScore();
        }
        std::cout << "wins the round!!" << std::endl << std::endl;
        // print the current scores at the end of the round
        for (int i = 0; i < NUM_PLAYERS; i++) {
            std::cout << "Player " << i+1 << " score: " << players_[i]->score() << std::endl;
        }
    }
    // at the end of the game check the scores and declare winner
    if (players_[0]->score() > players_[1]->score()) {
        std::cout << "Player 1 wins!" << std::endl;
    }
    else {
        std::cout << "Player 2 wins!" << std::endl;
    }
}
