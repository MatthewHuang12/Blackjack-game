#include "Player.h"

Player::Player() : bet(0), totalMoney(2000) {}

void Player::clearHand() {
    hand.clear();
}

void Player::makeBet(int amount) {
    bet = amount;
    totalMoney -= amount;
}

void Player::winBet(double multiplier) {
    totalMoney += bet * multiplier;
    bet = 0;
}

void Player::loseBet() {
    bet = 0;
}

bool Player::pushBet() {
    totalMoney += bet;
    bet = 0;
    return true;
}
