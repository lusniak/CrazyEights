#include <optional>
#include "deck.h"

Deck::Deck()
{
    deck.reserve(DECK_SIZE);
    for(int i = 0; i < RANK_MAX; i++)
    {
        for(int j = 0; j < SUIT_MAX; j++)
        {
            deck.emplace_back(static_cast<Ranks>(i), static_cast<Suits>(j));
        }
    }
}

void Deck::Display(void) const
{
    for(auto element : deck)
    {
        std::cout << element;
    }
}

void Deck::Shuffle(void)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(deck.begin(), deck.end(), gen);
}

std::optional<Card> Deck::Deal(void)
{
    if(deck.size() > 0)
    {
        Card card = deck.back();
        deck.pop_back();
        return card;
    }
    else
    {
        return std::nullopt;
    }
    
}

int Deck::GetNumOfCardsLeft(void) const
{
    return deck.size();
}

void Deck::GetCardsFromStackBackToDeck(std::vector<Card> stack)
{
    deck.insert(deck.end(), stack.begin(), stack.end());
}