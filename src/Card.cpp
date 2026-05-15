#include "Card.h"
#include <algorithm>
#include <random>

void fillDeck(Deck& deck)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 13; j++)
        {
            deck.cards.push_back(
                Card{static_cast<Suit>(i), static_cast<Rank>(j)});
        }
    }
}

void shuffleDeck(Deck& deck)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(deck.cards.begin(), deck.cards.end(), gen);
}

void dealCard(Deck& deck, Hand& hand)
{
    hand.cards.push_back(deck.cards.back());
    deck.cards.pop_back();
}

int calculateScore(const Hand& hand)
{
    int sum = 0;
    int aceCount = 0;
    for (int i = 0; i < hand.cards.size(); i++)
    {
        switch (hand.cards[i].rank)
        {
        case Rank::JACK:
            sum += 10;
            break;
        case Rank::QUEEN:
            sum += 10;
            break;
        case Rank::KING:
            sum += 10;
            break;
        case Rank::ACE:
            aceCount++;
            sum += 11;
            break;
        default:
            sum += static_cast<int>(hand.cards[i].rank) + 1;
            break;
        }
    }

    while (sum > 21 && aceCount != 0)
    {
        aceCount--;
        sum -= 10;
    }
    return sum;
}