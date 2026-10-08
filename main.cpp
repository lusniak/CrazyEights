#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include "main.h"
#include "card.h"
#include "deck.h"

#define DEVELOPMENT

enum GameStates{
    GAME_START,
    GAME_IN_PROGRESS,
    GAME_END,
    GAME_MAX
};

class Player{
    private:
        std::string m_name;
        std::vector<Card> hand;
    public:
        Player(std::string name) : m_name(name)
        {  

        }

        std::string_view GetName() const
        {
            return m_name;
        }

        int GetPoints() const
        {
            int points = 0;
            for(auto& card : hand)
            {
                points += card.GetPoints();
            }
            return points;
        }

        void DrawCard(Deck& deck)
        {
            hand.emplace_back(deck.Deal());
        }

        void ReturnWrongCardIntoHand(Card& card)
        {
            std::cout << "Cannot place this card on the table!\n\n";
            hand.emplace_back(card);
        }

        std::optional<Card> PlayCard(Deck& deck)
        {
            std::string input;
            int num = 0;

            do
            {
                std::cout << "Player " << m_name << " has to choose card to play or draw from deck\n";
                std::cout << "Enter certain number to play or enter SPACE to draw\n";
                std::getline(std::cin, input);
                if(input == " ")
                {
                    std::cout << "Player " << m_name << " draw card from deck\n";
                    DrawCard(deck);
                    return std::nullopt;
                }
                if (input.empty() || !std::all_of(input.begin(), input.end(), [](char c){return std::isdigit(c);}))
                {
                    std::cout << "\nPlease enter a valid number of card!\n\n";
                    continue;
                }
                num = std::stoi(input);
                if(num > 0 && num <= GetNumberOfCardsInHand())
                {
                    std::cout << "Player " << m_name << " plays " << hand[num-1] << "\n";
                    num = std::stoi(input);
                    Card retCard(hand[num-1]);
                    hand.erase(hand.begin() + num - 1);
                    return retCard;
                }
            } while (true);
        }

        friend std::ostream& operator<<(std::ostream& out, const Player& player)
        {
            out << player.m_name;
            return out;
        }

        void DisplayHand()
        {
            int cardNumber = 1;
            std::cout << "Player " << m_name << " has in hands " << hand.size() << " cards: \n";
            for(auto& element : hand)
            {
                std::cout << cardNumber << ": " << element;
                cardNumber++;
            }
        }

        int GetNumberOfCardsInHand() const
        {
            return hand.size();
        }

};

class Table{
    private:
        std::vector<Card> stack;
    public:
        Table()
        {

        }
        void PlaceCardOnTable(Card card)
        {
            stack.emplace_back(card);
        }
        Card WhatIsOnTopOfTheTableStack() const
        {
            return stack.back();
        }
        friend std::ostream& operator<<(std::ostream& out, const Table& table)
        {
            out << "The top card on the table is: \n" << table.stack.back() << '\n';
            return out;
        }
};

class Game{
    private:
        GameStates state;
        Deck deck;
        Table table;
        int m_numberOfPlayers;
        std::vector<Player> players;

        void EnterNumberOfPlayers(void)
        {
            #ifndef DEVELOPMENT
            std::string input = "0";
            int num = 0;
            do
            {
                std::cout << "************************\n";
                std::cout << "NEW GAME OF CRAZY EIGHTS\n";
                std::cout << "************************\n\n";
                std::cout << "Enter number of players (max 6): \n";
                std::getline(std::cin, input);

                if (input.empty() || !std::all_of(input.begin(), input.end(), [](char c){return std::isdigit(c);}))
                {
                    std::cout << "\nPlease enter a valid number of players!\n\n";
                    continue;
                }
                else
                {
                    num = std::stoi(input);
                    if(num <= 0)
                    {
                        std::cout << "\nNumber of players incorrect!\n\n";
                        continue;
                    }
                    else if(num > 6)
                    {
                        std::cout << "\nNumber of players can't exceed 6!\n\n";
                        continue;
                    }
                    else
                    {
                        m_numberOfPlayers = num;
                        break;
                    }
                }
            } while (true);
            #else
            m_numberOfPlayers = 2;
            #endif
        }

        void EnterPlayers(void)
        {
            #ifndef DEVELOPMENT
            for(int i = 0; i < m_numberOfPlayers; i++)
            {
                std::string name;
                std::cout << "Enter name of player " << i+1 << ": \n";
                std::getline(std::cin, name);
                players.emplace_back(Player(name));
            }2
            #else
            players.emplace_back(Player("Kamil"));
            players.emplace_back(Player("Ewa"));
            #endif
        }

        void DealCards()
        {
            int numberOfCardsToDraw = 5;
            if(m_numberOfPlayers < 3)
            {
                numberOfCardsToDraw = 7;
            }

            for(int j = 0; j < numberOfCardsToDraw; j++)
            {
                for(int i = 0; i < m_numberOfPlayers; i++)
                {
                    players[i].DrawCard(deck);
                }
            }

            table.PlaceCardOnTable(deck.Deal());
        }

        bool ValidateCardAgainstTable(Card& card)
        {
            bool retVal = false;
            Card topStackCard(table.WhatIsOnTopOfTheTableStack());
            if(card.GetRank() == RANK_8)
            {
                retVal = true;
            }
            else if((card.GetRank() == topStackCard.GetRank()) || (card.GetSuit() == topStackCard.GetSuit()))
            {
                retVal = true;
            }
            else
            {
                /* Do nothing, return false */
            }
            return retVal;
        }

    public:
        Game()
        {
            state = GAME_START;
            EnterNumberOfPlayers();
            players.reserve(m_numberOfPlayers);
            EnterPlayers();
            deck.Shuffle();
            //deck.Display();
            std::cout << "number of cards in deck " << deck.GetNumOfCardsLeft() << '\n';
            DealCards();
            std::cout << "number of cards in deck " << deck.GetNumOfCardsLeft() << '\n';
            std::cout << table;
            players[0].DisplayHand();
            players[1].DisplayHand();
            state = GAME_IN_PROGRESS;
        }

        GameStates GetGameState() const
        {
            return state;
        }

        void PlayRound()
        {
            for(auto& player : players)
            {
                do
                {
                    auto card = player.PlayCard(deck);
                    if(card.has_value())
                    {
                        if(ValidateCardAgainstTable(*card))
                        {
                            table.PlaceCardOnTable(*card);
                            break;
                        }
                        else
                        {
                            player.ReturnWrongCardIntoHand(*card);
                        }
                    }   
                } while (1);            
            }
            for(auto& player : players)
            {
                if(player.GetNumberOfCardsInHand() == 0)
                {
                    state = GAME_END;
                }
            }

            players[0].DisplayHand();
            players[1].DisplayHand();
            std::cout << "number of cards in deck " << deck.GetNumOfCardsLeft() << '\n';
            std::cout << table;
        }

        void EndGame()
        {   
            std::cout << "Game has ended\n";
            for(auto& player : players)
            {
                if(player.GetNumberOfCardsInHand() == 0)
                {
                    std::cout << player.GetName() << " won the game!\n";
                }
                else
                {
                    std::cout << player.GetName() << " lost having " << player.GetPoints() << " points in hand\n";
                }
            }
            std::cout << "To quit enter 'q' to play again enter 'n'\n";
            std::string input;
            std::getline(std::cin, input);

            if (input == "q" || input == "Q")
            {
                std::exit(0);
            }
            else if(input == "n" || input == "N")
            {
                state = GAME_START;
            }
            else
            {
                /* Do nothing */
            }
        }
};

int main()
{
    Game game;
    while(1)
    {
        switch(game.GetGameState())
        {
            case GAME_IN_PROGRESS:
                game.PlayRound();
                break;
            case GAME_END:
                game.EndGame();
                break;
            default:
                // TODO: some error handling?
                break;
        }
    }
    
    return 0;
}