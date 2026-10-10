#include "table.h"

Table::Table()
{

}

void Table::PlaceCardOnTable(Card card)
{
    stack.emplace_back(card);
}

Card Table::WhatIsOnTopOfTheTableStack() const
{
    return stack.back();
}

int Table::GetNumOfCardsOnTable() const
{
    return stack.size();
}

std::vector<Card> Table::GetAllCardsBelowTopFromStack()
{
    std::vector<Card> retStack;
    for(int i = 0; i < stack.size() - 1; i++)
    {
        retStack.emplace_back(stack[i]);
    }
    stack.erase(stack.begin(), stack.end() - 1);
    return retStack;
}