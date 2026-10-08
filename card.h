#pragma once

#include <iostream>
#include <string_view>

enum Suits{
    SUIT_HEART,
    SUIT_DIAMOND,
    SUIT_CLUB,
    SUIT_SPADE,
    SUIT_MAX
};

enum Ranks{
    RANK_ACE,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
    RANK_9,
    RANK_10,
    RANK_JACK,
    RANK_QUEEN,
    RANK_KING,
    RANK_MAX
};

class Card{
    private:
        Ranks m_rank;
        Suits m_suit;

        std::string_view DisplayRank() const;
        std::string_view DisplaySuit() const;
    public:
        Card(Ranks rank, Suits suit);
        friend std::ostream& operator<<(std::ostream& out, const Card& card)
        {
            out << card.DisplayRank() << " of " << card.DisplaySuit() << '\n';
            return out;
        }
        Ranks GetRank() const;
        Suits GetSuit() const;
        int GetPoints() const;
};
