#pragma once
#include <vector>

enum class Suit
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    SPADES
};

enum class Rank
{
    ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING
};

struct Card
{
    Suit suit;
    Rank rank;
};

struct Deck
{
    std::vector<Card> cards;
};

struct Hand
{
    std::vector<Card> cards;
};

void fillDeck(Deck& deck);
void shuffleDeck(Deck& deck);
void dealCard(Deck& deck, Hand& hand);
int calculateScore(const Hand& hand);
