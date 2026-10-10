#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include "card.h"

constexpr int DECK_SIZE = 52;

class Deck{
    private:
        std::vector<Card> deck;
    public:
        Deck();
        void Display(void) const;
        void Shuffle(void);
        std::optional<Card> Deal(void);
        int GetNumOfCardsLeft(void) const;
        void GetCardsFromStackBackToDeck(std::vector<Card> stack);
};