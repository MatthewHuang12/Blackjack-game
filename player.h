#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "Card.h"

class Player {
public:
    std::vector<Card> hand;
    int bet;
    int totalMoney;

    Player();
    void clearHand();
    void makeBet(int amount);
    void winBet(double multiplier);
    void loseBet();
    bool pushBet();
};

#endif 