#include "gameConstants.h"
#include "cities.h"
#include <random>
#include <numeric>
#include <iostream>
#include <sstream>

#ifndef DECK
#define DECK

class Card
{
    friend std::ostream& operator<<(std::ostream& lhs, const Card& rhs)
    {
        return rhs.write(lhs);
    }

    private:

        virtual std::ostream& write(std::ostream& lhs) const = 0;

    protected:

        int_fast16_t cardNumber;

    public:

        constexpr Card()
            : cardNumber{epidemicCard}
        {}

        constexpr Card(int_fast16_t n)
            : cardNumber{n}
        {}

        constexpr Card(const Card& rhs)
            : cardNumber{rhs.cardNumber}
        {}

        Card& operator=(const Card& rhs)
        {
            cardNumber = rhs.cardNumber;
            return *this;
        }

        Card& operator=(Card&& rhs)
        {
            cardNumber = rhs.cardNumber;
            return *this;
        }

        template <typename T>
        T getNumber() const
        {
            return static_cast<T>(cardNumber);
        }
};

class playerCard : public Card
{
    private:

        std::ostream& write(std::ostream& lhs) const
        {
            if (cardNumber == epidemicCard)
            {
                return lhs << "EP ";
            }
            else if (cardNumber >= numCityCards)
            {
                return lhs << 'E' << cardNumber - numCityCards << ' ';
            }
            else
            {
                return lhs << 'C' << cardNumber << ' ';
            }
        }

    public:

        constexpr playerCard()
            : Card{epidemicCard}
        {}

        constexpr playerCard(int_fast16_t n)
            : Card{n}
        {}
};

class infectionCard : public Card
{
    private:

        std::ostream& write(std::ostream& lhs) const
        {
            return lhs << cardNumber << ' ';
        }

    public:

        constexpr infectionCard()
            : Card{epidemicCard}
        {}

        constexpr infectionCard(int_fast16_t n)
            : Card{n}
        {}
};

template <class C>
class Deck 
{
    
    friend std::ostream& operator<<(std::ostream& lhs, Deck& rhs) {
        return rhs.write(lhs);
    }

    private:

        virtual std::ostream& write(std::ostream& lhs) const = 0;

    public:

        virtual const C& drawCard() = 0;
        virtual void beginningShuffle() = 0;
};

class playerDeck: public Deck<playerCard>
{
    private:

        std::array<playerCard, numPlayerCards + gameDifficulty> cards;
        std::mt19937_64& random;
        int_fast16_t drawIndex{numPlayerCards + gameDifficulty - 1};

        constexpr std::array<int_fast16_t, gameDifficulty> getDeckSizes() const
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes;
            int_fast16_t deckSize = drawIndex + 1;
            int_fast16_t minSize = 1;
            int_fast16_t sumOfMins = ((gameDifficulty + minSize) * (gameDifficulty >> 1)) + ((gameDifficulty & 1) * ((gameDifficulty + minSize) >> 1));
            int_fast16_t multiplesThatFit = (deckSize - sumOfMins) / gameDifficulty;
            minSize += multiplesThatFit;
            std::iota(deckSizes.begin(), deckSizes.end(), minSize);
            int_fast16_t remainder = deckSize - (sumOfMins + (multiplesThatFit * gameDifficulty));
            while (remainder > 0)
            {
                ++deckSizes[--remainder];
            }
            return deckSizes;
        }

        std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            result << "playerDeck: ";
            for (auto begin = cards.begin(), end = cards.begin() + drawIndex + 1; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        constexpr playerDeck(std::mt19937_64& r)
            : random{r}
        {
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards[i + gameDifficulty] = playerCard{i};
            }
            for (int_fast16_t i = numCityCards; i < numEventCards + numCityCards; ++i)
            {
                cards[gameDifficulty + i] = playerCard{i};
            }
        }

        void prepareDeck()
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes = getDeckSizes();
            int_fast16_t index = gameDifficulty - 1;
            auto miniDeckStart = cards.begin() + drawIndex;
            auto epidemicCardIterator = cards.begin();
            while (index >= 0)
            {
                int_fast16_t deckSize = deckSizes[index];
                int_fast16_t swapIndex = random() % deckSize;
                std::swap(*(epidemicCardIterator + index), *(miniDeckStart - swapIndex));
                miniDeckStart -= deckSize;
                --index;
            }
        }

        const playerCard& drawCard()
        {
            const playerCard& result = *(cards.begin() + drawIndex);
            --drawIndex;
            return result;
        }

        void beginningShuffle()
        {
            std::shuffle(cards.begin() + gameDifficulty, cards.end(), random);
        }
};

class infectionDeck : public Deck<infectionCard>
{
    private:

        std::array<infectionCard, numCityCards + gameDifficulty> cards;
        std::mt19937_64& random;
        int_fast16_t drawIndex{numCityCards - 1};
        int_fast16_t epidemicIndex{0};
        int_fast16_t backOfDeck{numCityCards};

        std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            result << "infectionDeck: ";
            for (auto begin = cards.begin() + epidemicIndex, end = cards.begin() + drawIndex + 1; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << "\ndiscard: ";
            for (auto begin = cards.begin() + drawIndex + 1, end = cards.begin() + backOfDeck; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        constexpr infectionDeck(std::mt19937_64& r)
            : random{r}
        {
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards[i] = infectionCard{i};
            }
            for (int_fast16_t i = 0; i < gameDifficulty; ++i)
            {
                cards[i + numCityCards] = infectionCard{epidemicCard};
            }
        }

        const Cities infect()
        {
            cards[backOfDeck] = cards[epidemicIndex];
            ++backOfDeck;
            return cards[epidemicIndex++].getNumber<Cities>();
        }

        void intensify(Cities cityToRemove, bool removeCity)
        {
            if (removeCity)
            {
                auto removedCity = std::find_if(cards.begin() + drawIndex + 1, cards.begin() + backOfDeck
                                                    , [&](const infectionCard& c){ return c.getNumber<Cities>() == cityToRemove; });
                --backOfDeck;
                std::swap(*removedCity, *(cards.begin() + backOfDeck));
            }
            std::shuffle(cards.begin() + drawIndex + 1, cards.begin() + backOfDeck, random);
            drawIndex = backOfDeck - 1;
        }

        const infectionCard& drawCard()
        {
            const infectionCard& result = *(cards.begin() + drawIndex);
            --drawIndex;
            return result;
        }

        void beginningShuffle()
        {
            std::shuffle(cards.begin(), cards.begin() + numCityCards, random);
        }
};
#endif