#include "gameConstants.h"
#include "deck.h"
#include <unordered_set>

class Player
{
    private:

        std::unordered_set<const Card&, cardHash> cards;
        Cities location{Cities::atlanta};

    public:

        Player()
        {}

        int_fast32_t maxPopulation() const
        {
            int_fast32_t result{0};
            for (const auto& card: cards)
            {   
                int_fast32_t population = cityPopulations[card.getNumber()];
                result = !card.isEvent() && population > result ? population : result;
            }
            return result;
        }

        bool addCard(const Card& card)
        {
            cards.insert(card);
        }

        void removeCard(const Card& card)
        {
            cards.erase(card);
        }
};