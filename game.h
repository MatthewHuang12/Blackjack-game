#ifndef GAME_H
#define GAME_H

#include <vector>
#include "Card.h"
#include "Player.h"

void initializeDeck(std::vector<Card>& deck);
int calculateHandScore(const std::vector<Card>& hand);
void displayHandAndScore(const Player& player);
bool playerHit(Player& player, std::vector<Card>& deck);
void dealInitialCards(Player& player, Player& dealer, std::vector<Card>& deck);
int getValidBet(int minBet, int maxBet, int playerMoney);
void playerDecision(Player& player, std::vector<Card>& deck);
void dealerPlay(Player& dealer, std::vector<Card>& deck);
void determineWinner(Player& player, Player& dealer);
bool isBlackjack(const std::vector<Card>& hand);
void displaydealerHand(const Player& dealer);

#endif 
