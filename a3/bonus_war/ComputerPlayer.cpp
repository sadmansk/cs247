#include "ComputerPlayer.h"
#include <random>

Card* TopPlayer::play() {
    auto it = hand_.begin();
    Card* played = *it;
    hand_.erase(it);
    return played;
}

Card* RandPlayer::play() {
    static std::mt19937 rng(0);
    int index = rng() % hand_.size();
    auto it = hand_.begin()+index;
    Card* played = *it;
    hand_.erase(it);
    return played;
}
