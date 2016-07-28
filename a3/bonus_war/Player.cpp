#include "Player.h"

Player::~Player() {
    hand_.clear();
}

void Player::incScore() {
    score_++;
}

int Player::score() {
    return score_;
}

void Player::addCard(Card* card) {
    hand_.push_back(card);
}
