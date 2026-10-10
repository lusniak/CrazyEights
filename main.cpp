#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <optional>
#include "main.h"
#include "card.h"
#include "deck.h"
#include "player.h"
#include "table.h"

#define DEVELOPMENT

enum GameStates{
    GAME_START,
    GAME_IN_PROGRESS,
    GAME_END,
    GAME_MAX
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

            table.PlaceCardOnTable(*deck.Deal());
        }

        bool ValidateCardAgainstTable(Card& card)
        {
            static Card virtualEight(RANK_MAX, SUIT_MAX);
            static bool isVirtual = false;
            int num;
            std::string input;
            bool retVal = false;
            Card topStackCard(table.WhatIsOnTopOfTheTableStack());
            if(card.GetRank() == RANK_8)
            {
                do
                {
                    std::cout << "Choose suit: \n";
                    std::cout << "1: HEARS\n";
                    std::cout << "2: DIAMONDS\n";
                    std::cout << "3: CLUBS\n";
                    std::cout << "4: SPADES\n";
                    std::getline(std::cin, input);
                    if (input.empty() || !std::all_of(input.begin(), input.end(), [](char c){return std::isdigit(c);}))
                    {
                        std::cout << "\nPlease enter a valid number of suit!\n\n";
                        continue;
                    }
                    else
                    {
                        num = std::stoi(input);
                        if(num > 0 && num < 5)
                        {
                            switch(num)
                            {
                                case 1:
                                    virtualEight = Card(RANK_8, SUIT_HEART);
                                    isVirtual = true;
                                    break;
                                case 2:
                                    virtualEight = Card(RANK_8, SUIT_DIAMOND);
                                    isVirtual = true;
                                    break;
                                case 3:
                                    virtualEight = Card(RANK_8, SUIT_CLUB);
                                    isVirtual = true;
                                    break;
                                case 4:
                                    virtualEight = Card(RANK_8, SUIT_SPADE);
                                    isVirtual = true;
                                    break;
                                default:
                                    /* Do nothing */
                                    break;
                            }
                            std::cout << "\nPlayer demands " << virtualEight.GetSuit() << "!\n";
                            retVal = true;
                            break;
                        }
                        else
                        {
                            std::cout << "\nPlease enter a valid number of suit!\n\n";
                            continue;
                        }
                    }
                }
                while(true);
            }
            else if(isVirtual)
            {
                if(card.GetSuit() == virtualEight.GetSuit())
                {
                    /* Played card suit matches suit choosen by previous player */
                    isVirtual = false;
                    retVal = true;
                }
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
            state = GAME_IN_PROGRESS;
        }

        void SetState(GameStates newState)
        {
            state = newState;
        }

        void PutCardsFromTableToDeckAndReshuffle()
        {
            deck.GetCardsFromStackBackToDeck(table.GetAllCardsBelowTopFromStack());
            deck.Shuffle();
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
                    player.DisplayHand();
                    std::cout << table;
                    bool isDeckEmpty = false;
                    auto card = player.PlayCard(deck, isDeckEmpty);
                    if(isDeckEmpty == true)
                    {
                        if((deck.GetNumOfCardsLeft() == 0) && (table.GetNumOfCardsOnTable() < 1))
                        {
                            /* All cards in players hands, end the game */
                            state = GAME_END;
                            return;
                        }
                        else
                        {
                            PutCardsFromTableToDeckAndReshuffle();
                        }
                        card = player.PlayCard(deck, isDeckEmpty);
                    }
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
                    else
                    {
                        /* Player draw a card */
                        break;
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
                system("start main.exe");
                std::exit(0);
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
            case GAME_START:
                break;
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