#include <iostream>
#include "Player.h"
#include "game.h"
#include <vector>
#include <limits>

int main() {
    // Create and initialize the deck of cards.
    std::vector<Card> deck;
    initializeDeck(deck);

    // Create player and dealer objects.
    Player player, dealer;

    // Variable to control the game loop.
    char playAgain = 'y';
    while (playAgain == 'y' && player.totalMoney > 0) {
        std::cout << "Welcome to Blackjack!" << std::endl;

        // Prompt the player to place a bet within allowed limits and their current balance.
        int bet = getValidBet(25, 500, player.totalMoney);
        player.makeBet(bet);

        // Reinitialize the deck if it has fewer than 10 cards remaining.
        if (deck.size() < 10) {
            initializeDeck(deck);
        }

        // Clear the player and dealer hands from previous rounds.
        player.clearHand();
        dealer.clearHand();

        // Deal initial cards to both player and dealer.
        dealInitialCards(player, dealer, deck);

        // Show the dealer's first card and the player's hand and score.
        std::cout << "Dealer's showing: " << dealer.hand[0].value << dealer.hand[0].suit << std::endl;
        displayHandAndScore(player);

        // Player makes decisions (hit, stand, double down) until they stand or bust.
        playerDecision(player, deck);

        // If the player hasn't busted, the dealer takes their turn.
        if (calculateHandScore(player.hand) <= 21) {
            std::cout << "Dealer's turn." << std::endl;
            dealerPlay(dealer, deck);
            displaydealerHand(dealer);
        }

        // Determine and announce the winner of the round.
        determineWinner(player, dealer);
        std::cout << "Your balance is $" << player.totalMoney << std::endl;

        // Ask the player if they want to play another round, provided they have money left.
        if (player.totalMoney > 0) {
            std::cout << "Do you want to play again? (y/n): ";
            std::cin >> playAgain;
            playAgain = std::tolower(playAgain);

            // Validate the input for replay decision.
            while (std::cin.fail() || (playAgain != 'y' && playAgain != 'n')) {
                std::cout << "Invalid input. Please enter 'y' for yes or 'n' for no: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin >> playAgain;
                playAgain = std::tolower(playAgain);
            }
        } else {
            std::cout << "You've run out of money. Game over!" << std::endl;
            break; // Exit the game loop if the player has no money left.
        }
    }
    return 0;
}
