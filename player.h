#pragma once

#include <vector>
#include <string>
#include <optional>
#include "card.h"
#include "deck.h"

class Player{
    private:
        std::string m_name;
        std::vector<Card> hand;
    public:
        Player(std::string name);
        std::string_view GetName() const;
        int GetPoints() const;
        bool DrawCard(Deck& deck);
        void ReturnWrongCardIntoHand(Card& card);
        std::optional<Card> PlayCard(Deck& deck, bool& isDeckEmpty);
        friend std::ostream& operator<<(std::ostream& out, const Player& player)
        {
            out << player.m_name;
            return out;
        }
        void DisplayHand();
        int GetNumberOfCardsInHand() const;
};