#include "player.h"

Player::Player(std::string name) : m_name(name)
{  

}

std::string_view Player::GetName() const
{
    return m_name;
}

int Player::GetPoints() const
{
    int points = 0;
    for(auto& card : hand)
    {
        points += card.GetPoints();
    }
    return points;
}

bool Player::DrawCard(Deck& deck)
{
    auto card = deck.Deal();
    if(card.has_value())
    {
        hand.emplace_back(*card);
        return true;
    }
    else
    {
        return false;
    }
}

void Player::ReturnWrongCardIntoHand(Card& card)
{
    std::cout << "Cannot place this card on the table!\n\n";
    hand.emplace_back(card);
}

std::optional<Card> Player::PlayCard(Deck& deck, bool& isDeckEmpty)
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
            if(true == DrawCard(deck))
            {
                isDeckEmpty = false;
            }
            else
            {
                isDeckEmpty = true;
            }
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

void Player::DisplayHand()
{
    int cardNumber = 1;
    std::cout << "Player " << m_name << " has in hands " << hand.size() << " cards: \n";
    for(auto& element : hand)
    {
        std::cout << cardNumber << ": " << element;
        cardNumber++;
    }
}

int Player::GetNumberOfCardsInHand() const
{
    return hand.size();
}