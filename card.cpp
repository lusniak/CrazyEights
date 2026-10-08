#include "card.h"

 std::string_view Card::DisplayRank() const
        {
            switch(m_rank)
            {
                case RANK_ACE:
                    return "ACE";
                case RANK_2:
                    return "2";
                case RANK_3:
                    return "3";
                case RANK_4:
                    return "4";
                case RANK_5:
                    return "5";
                case RANK_6:
                    return "6";
                case RANK_7:
                    return "7";
                case RANK_8:
                    return "8";
                case RANK_9:
                    return "9";
                case RANK_10:
                    return "10";
                case RANK_JACK:
                    return "JACK";
                case RANK_QUEEN:
                    return "QUEEN";
                case RANK_KING:
                    return "KING";
                default:
                    return "Error: wrong rank!";
            }
        }

        std::string_view Card::DisplaySuit() const
        {
            switch(m_suit)
            {
                case SUIT_HEART:
                    return "HEARTS";
                case SUIT_DIAMOND:
                    return "DIAMONDS";
                case SUIT_CLUB:
                    return "CLUBS";
                case SUIT_SPADE:
                    return "SPADES";
                default:
                    return "Error: wrong suit!";
            }
        }

        Card::Card(Ranks rank, Suits suit) : m_rank(rank), m_suit(suit)
        {

        }
        
        Ranks Card::GetRank() const
        {
            return m_rank;
        }
        Suits Card::GetSuit() const
        {
            return m_suit;
        }
        int Card::GetPoints() const
        {
            int points = 0;
            switch(m_rank)
            {
                case RANK_ACE:
                    points += 1;
                    break;
                case RANK_2:
                    points += 2;
                    break;
                case RANK_3:
                    points += 3;
                    break;
                case RANK_4:
                    points += 4;
                    break;
                case RANK_5:
                    points += 5;
                    break;
                case RANK_6:
                    points += 6;
                    break;
                case RANK_7:
                    points += 7;
                    break;
                case RANK_8:
                    points += 50;
                    break;
                case RANK_9:
                    points += 9;
                    break;
                case RANK_10:
                case RANK_JACK:
                case RANK_QUEEN:
                case RANK_KING:
                    points += 10;
                    break;
                default:
                    /* Do nothing */
                    break;
            }
            return points;
        }