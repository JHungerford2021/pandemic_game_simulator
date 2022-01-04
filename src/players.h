#include "gameConstants.h"
#include "deck.h"
#include <bitset>


class Player
{
    friend std::ostream& operator<<(std::ostream& lhs, const Player& rhs)
    {
        std::stringstream result{};
        result << "Role: " << rhs.role << "\nDeck: ";
        for (int_fast16_t i = 0; i < numPlayerCards; ++i)
        {
            if (rhs.cards.test(i))
                result << i << ' ';
        }
        result << "\nLocation: " << rhs.location << std::endl;
        return lhs << result.str();
    }

    private:

        std::bitset<numPlayerCards> cards;
        Cities location{Cities::atlanta};
        Roles role;

    public:

        Player(Roles r)
            :role{r}
        {}

        int_fast32_t maxPopulation() const
        {
            int_fast32_t result{0};
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {   
                if (cards.test(i)) {
                    int_fast32_t population = cityPopulations[i];
                    result = population > result ? population : result;
                }
            }
            return result;
        }

        bool addCard(const playerCard& card)
        {
            cards.set(card.getNumber<int_fast16_t>());
            if (cards.count() > maxCards)
                return true;
            return false;
        }

        void removeCard(const playerCard& card)
        {
            cards.reset(card.getNumber<int_fast16_t>());
        }
};