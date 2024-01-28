#include "game.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <limits>
#include <cctype>

// Initializes the deck of cards for the game.
void initializeDeck(std::vector<Card>& deck) 
{
    deck.clear(); // Clears any existing cards in the deck.

    // Array of card suits.
    std::string suits[] = {" of Hearts", " of Diamonds", " of Clubs", " of Spades"};

    // Loop through each suit and card value to create a full deck.
    for (const std::string& suit : suits) {
        for (int value = 1; value <= 13; ++value) {
            // Assigns the appropriate card value (Ace, Jack, Queen, King, or number).
            std::string cardValue = (value == 1) ? "A" : (value == 11) ? "J" : (value == 12) ? "Q" : (value == 13) ? "K" : std::to_string(value);
            deck.push_back({suit, cardValue}); // Adds the card to the deck.
        }
    }

    // Shuffling the deck.
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

// Calculates the total score of a given hand.
int calculateHandScore(const std::vector<Card>& hand) {
    int score = 0; // Initial score.
    int aceCount = 0; // Count of aces in the hand.

    // Calculate score based on card values.
    for (const Card& card : hand) {
        if (card.value == "A") {
            aceCount++;
            score += 11; // Ace can be 1 or 11.
        } else if (card.value == "J" || card.value == "Q" || card.value == "K") {
            score += 10; // Face cards are worth 10.
        } else {
            score += std::stoi(card.value); // Number cards are worth their value.
        }
    }

    // Adjust score if Aces are present and total score exceeds 21.
    while (score > 21 && aceCount > 0) {
        score -= 10; // Convert an Ace from 11 to 1.
        aceCount--;
    }

    return score;
}

// Displays a player's hand and their score.
void displayHandAndScore(const Player& player) {
    std::cout << "Hand: ";
    for (const Card& card : player.hand) {
        std::cout << card.value << card.suit << " ";
    }
    std::cout << "Score: " << calculateHandScore(player.hand) << std::endl;
}

// Displays the dealer's hand.
void displaydealerHand(const Player& dealer) {
    std::cout << "Hand: ";
    for (const Card& c : dealer.hand) {
        std::cout << c.value << c.suit << " ";
    }
    std::cout << "Score: " << calculateHandScore(dealer.hand) << std::endl;
}

// Processes the player's choice to hit and adds a card to their hand.
bool playerHit(Player& player, std::vector<Card>& deck) {
    player.hand.push_back(deck.back()); // Adds the top card from the deck to the player's hand.
    deck.pop_back(); // Removes the card from the deck.
    displayHandAndScore(player); // Displays the updated hand and score.

    return calculateHandScore(player.hand) > 21; // Returns true if player busts.
}

// Deals initial two cards to each player and dealer.
void dealInitialCards(Player& player, Player& dealer, std::vector<Card>& deck) 
{
    // Each player and dealer receive two cards.
    player.hand.push_back(deck.back());
    deck.pop_back();
    dealer.hand.push_back(deck.back());
    deck.pop_back();
    player.hand.push_back(deck.back());
    deck.pop_back();
    dealer.hand.push_back(deck.back());
    deck.pop_back();
}

// Ensures the player enters a valid bet amount.
int getValidBet(int minBet, int maxBet, int playerMoney) {
    int bet;
    std::cout << "Enter your bet: ";
    // Validates the bet input.
    while (!(std::cin >> bet) || bet < minBet || bet > maxBet || bet > playerMoney) {
        std::cout << "Invalid bet. Please enter a bet between $" << minBet << " and $" << std::min(maxBet, playerMoney) << ": ";
        std::cin.clear(); // Clears the error flag on cin.
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignores incorrect input.
    }
    return bet;
}

void playerDecision(Player& player, std::vector<Card>& deck) {
    char decision;
    bool canDoubleDown = (player.hand.size() == 2) && (player.totalMoney >= 2 * player.bet); // Checks if double down is possible.

    do {
        std::cout << "Choose an action: Hit (H), Stand (S)" << (canDoubleDown ? ", Double Down (D)" : "") << ": ";
        std::cin >> decision;
        decision = std::toupper(decision); // Converts input to uppercase.

        if (decision == 'H') {
            if (playerHit(player, deck)) { // Player chooses to hit.
                std::cout << "Bust!" << std::endl;
                return;
            }
        } else if (decision == 'S') { // Player chooses to stand.
            return;
        } else if (decision == 'D' && canDoubleDown) { // Player chooses to double down.
            player.totalMoney -= player.bet; // Deducts the additional bet.
            player.bet *= 2; // Doubles the bet.
            if (playerHit(player, deck)) { // Hits once and ends the turn.
                std::cout << "Bust!" << std::endl;
            }
            return; // Ends player's turn after doubling down.
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (true);
}


void dealerPlay(Player& dealer, std::vector<Card>& deck) {
    // Dealer keeps hitting until the score is 17 or higher.
    while (calculateHandScore(dealer.hand) < 17) {
        playerHit(dealer, deck);
        if (calculateHandScore(dealer.hand) > 21) {
            std::cout << "Dealer busts!" << std::endl;
            break;
        }
    }
}

// Determines the winner of the round.
void determineWinner(Player& player, Player& dealer) {
    int playerScore = calculateHandScore(player.hand);
    int dealerScore = calculateHandScore(dealer.hand);

    // Evaluating various win, lose, and push conditions.
    if (playerScore > 21) {
        std::cout << "You busted. Dealer wins." << std::endl;
        player.loseBet();
    } else if (dealerScore > 21 || playerScore > dealerScore) {
        if (isBlackjack(player.hand)) {
            std::cout << "Blackjack! You win!" << std::endl;
            player.winBet(2.5); // Payout for blackjack.
        } else {
            std::cout << "You win!" << std::endl;
            player.winBet(2); // Regular payout.
        }
    } else if (dealerScore > playerScore) {
        std::cout << "Dealer wins." << std::endl;
        player.loseBet();
    } else {
        std::cout << "It's a push." << std::endl;
        player.pushBet(); // Neither wins nor loses.
    }
}

// Checks if the hand is a blackjack (an ace and a 10-value card).
bool isBlackjack(const std::vector<Card>& hand) {
    return hand.size() == 2 && calculateHandScore(hand) == 21;
}