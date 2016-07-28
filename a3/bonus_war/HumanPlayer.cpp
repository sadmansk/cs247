#include "HumanPlayer.h"
#include <iostream>

Card* HumanPlayer::play() {
    // print the cards out to the player
    printHand();

    std::cout << "Please pick a card to play (pick the position of the card in the hand: ";
    int index = 1;
    std::cin >> index;

    while (index < 1 || (unsigned)index > hand_.size()) {
        std::cout << "Please pick a valid position: ";
        std::cin >> index;
    }

    auto it = hand_.begin()+index-1;
    Card* played = *it;
    hand_.erase(it);
    return played;
}

void HumanPlayer::printHand() {
    std::cout << "Hand: ";
    for (unsigned int i = 0; i < hand_.size(); i++) {
        std::cout << "(" << i+1 << ") " << *hand_[i] << " ";
    }
    std::cout << std::endl;
}
