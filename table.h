#pragma once

#include <vector>
#include "card.h"

class Table{
    private:
        std::vector<Card> stack;
    public:
        Table();
        void PlaceCardOnTable(Card card);
        Card WhatIsOnTopOfTheTableStack() const;
        int GetNumOfCardsOnTable() const;
        friend std::ostream& operator<<(std::ostream& out, const Table& table)
        {
            out << "The top card on the table is: \n" << table.stack.back() << '\n';
            return out;
        }
        std::vector<Card> GetAllCardsBelowTopFromStack();
};