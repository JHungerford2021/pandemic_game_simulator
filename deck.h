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
    friend std::ostream& operator << (std::ostream& lhs, const Card& rhs)
    {
        return rhs.write(lhs);
    }

    protected:

        int_fast16_t cardNumber;

        virtual std::ostream& write (std::ostream& lhs) const = 0;

    public:

        Card(int_fast16_t n)
            : cardNumber{n}
        {}

        Card(const Card& rhs)
            : cardNumber{rhs.cardNumber}
        {}

        Card(Card&& rhs)
            : cardNumber{rhs.cardNumber}
        {}

        Card& operator = (Card&& rhs)
        {
            cardNumber = rhs.cardNumber;
            return *this;
        }

        template <typename T>
        T getNumber() const
        {
            return static_cast<T>(cardNumber);
        }

        virtual bool isEvent() const = 0;
};

class cardHash
{
    public:

        size_t operator()(const Card& c) const
        {
            size_t result = c.isEvent() ? numCityCards : 0;
            return result + c.getNumber<int_fast16_t>();
        }
};

class playerCard : public Card
{
    private:

        bool event;

    protected:

        virtual std::ostream& write (std::ostream& lhs) const
        {
            if (cardNumber == epidemicCard)
            {
                return lhs << "EP ";
            }
            else if (event)
            {
                return lhs << 'E' << cardNumber << ' ';
            }
            else
            {
                return lhs << 'C' << cardNumber << ' ';
            }
        }

    public:

        playerCard(int_fast16_t n, bool e = 0)
            :Card{n}, event{e}
        {}

        playerCard(const playerCard& rhs)
            :Card{rhs.cardNumber}, event{rhs.event}
        {}

        playerCard(playerCard&& rhs)
            :Card(rhs.cardNumber), event{rhs.event}
        {}

        playerCard& operator = (playerCard&& rhs)
        {
            cardNumber = rhs.cardNumber;
            event = rhs.event;
            return *this;
        }

        bool isEvent() const
        {
            return event;
        }
};

class infectionCard : public Card
{
    protected:

        virtual std::ostream& write (std::ostream& lhs) const
        {
            return lhs << cardNumber << ' ';
        }

    public:

        infectionCard(int_fast16_t n)
            :Card{n}
        {}

        bool isEvent() const
        {
            return false;
        }
};

template <class C>
class Deck 
{
    
    friend std::ostream& operator << (std::ostream& lhs, Deck& rhs) {
        return rhs.write(lhs);
    }

    protected:

        std::vector<C> cards;
        std::mt19937_64& random;
        int_fast16_t drawIndex{0};
        
        Deck(std::mt19937_64& r, int_fast16_t size)
            : random {r}
        {
            cards.reserve(size);
        }

        virtual std::ostream& write(std::ostream& lhs) const = 0;

    public:

        virtual const C& drawCard()
        {
            const C& result = *(cards.crbegin() + drawIndex);
            ++drawIndex;
            return result;
        }
};

template <int_fast16_t gameDifficulty>
class playerDeck: public Deck<playerCard>
{
    private:

        std::array<int_fast16_t, gameDifficulty> getDeckSizes() const
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes;
            int_fast16_t deckSize = static_cast<int_fast16_t>(cards.size()) - drawIndex;
            int_fast16_t minSize = 1;
            int_fast16_t sumOfMins = ((gameDifficulty + minSize) * (gameDifficulty >> 1)) + ((gameDifficulty & 1) * ((gameDifficulty + minSize) >> 1));
            int_fast16_t multiplesThatFit = (deckSize - sumOfMins) / gameDifficulty;
            minSize += multiplesThatFit;
            std::iota(deckSizes.begin(), deckSizes.end(), minSize);
            int_fast16_t remainder = deckSize - (sumOfMins + (multiplesThatFit * gameDifficulty));
            std::unordered_set<int_fast16_t> takenValues;
            while (remainder > 0)
            {
                int_fast16_t deckSelection = random() % gameDifficulty;
                while (takenValues.find(deckSelection) != takenValues.end())
                {
                    deckSelection = random() % gameDifficulty;
                }
                takenValues.insert(deckSelection);
                ++deckSizes[deckSelection];
                --remainder;
            }
            return deckSizes;
        }

        /*
        void getDeckSizesAlternative(std::vector<int_fast16_t>& deckSizes, int_fast16_t numEpidemicCards)
        {
            int_fast16_t deckSize = static_cast<int_fast16_t>(cards.size()) - drawIndex;
            while (numEpidemicCards > 1)
            {
                std::cout << "Epidemic Cards at start of loop: " << numEpidemicCards << std::endl;
                std::cout << "Deck size at start of loop: " << deckSize << std::endl;
                int_fast16_t minSize = 1;
                int_fast16_t sumOfMins = ((numEpidemicCards + minSize) * (numEpidemicCards >> 1)) + ((numEpidemicCards & 1) * ((numEpidemicCards + minSize) >> 1));
                std::cout << "sumOfMins: " << sumOfMins << std::endl;
                int_fast16_t multiplesThatFit = (deckSize - sumOfMins) / numEpidemicCards;
                std::cout << "multiplesThatFit: " << multiplesThatFit << std::endl;
                int_fast16_t remainder = deckSize - (sumOfMins + (multiplesThatFit * numEpidemicCards));
                std::cout << "remainder: " << remainder << std::endl;
                int_fast16_t min = numEpidemicCards + multiplesThatFit + remainder;
                int_fast16_t max = deckSize - sumOfMins + numEpidemicCards;
                int_fast16_t size{min};
                if (min != max)
                {
                    size = (random() % (max - min)) + min;
                }
                std::cout << "Min: " << min << " Max: " << max << " Size: " << size << "\n\n";
                deckSizes[--numEpidemicCards] = size;
                deckSize -= size;
            }
            deckSizes[0] = deckSize;
        }
        */

    protected:

        virtual std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            auto begin = cards.begin();
            auto end = cards.end() - drawIndex;
            for (; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        playerDeck(std::mt19937_64& r)
            : Deck<playerCard>{r, numPlayerCards + gameDifficulty}
        {
            for (int_fast16_t i = 0; i < gameDifficulty; ++i)
            {
                cards.emplace_back(playerCard{epidemicCard});
            }
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards.emplace_back(playerCard{i});
            }
            for (int_fast16_t i = 0; i < numEventCards; ++i)
            {
                cards.emplace_back(playerCard{i, 1});
            }
            std::shuffle(cards.begin() + gameDifficulty, cards.end(), random);
        }

        void prepareDeck()
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes = getDeckSizes();
            int_fast16_t index = gameDifficulty - 1;
            auto miniDeckStart = cards.rbegin() + drawIndex;
            while (index >= 0)
            {
                int_fast16_t deckSize = deckSizes[index];
                int_fast16_t swapIndex = random() % deckSize;
                std::swap(cards[index], *(miniDeckStart + swapIndex));
                miniDeckStart += deckSize;
                --index;
            }
        }
};

template<int_fast16_t gameDifficulty>
class infectionDeck : public Deck<infectionCard>
{
    private:
        int_fast16_t epidemicIndex{0};

    protected:

        virtual std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            auto begin = cards.begin() + epidemicIndex;
            auto end = cards.end() - drawIndex;
            for (; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        infectionDeck(std::mt19937_64& r)
            : Deck<infectionCard>{r, numCityCards + gameDifficulty}
        {
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards.emplace_back(infectionCard{i});
            }
            std::shuffle(cards.begin(), cards.end(), random);
        }

        const Cities infect()
        {
            cards.emplace_back(cards[epidemicIndex]);
            ++drawIndex;
            return cards[epidemicIndex++].getNumber<Cities>();
        }

        void intensify(Cities cityToRemove, bool removeCity)
        {
            if (removeCity)
            {
                if (cityToRemove != cards[epidemicIndex - 1].getNumber<Cities>())
                {
                    auto removedCity = std::find_if(cards.rbegin(), cards.rbegin() + drawIndex
                                                    , [&](const infectionCard& c){ return c.getNumber<Cities>() == cityToRemove; });
                    std::swap(*removedCity, *cards.rbegin());
                }
                cards.pop_back();
                --drawIndex;
            }
            std::shuffle(cards.rbegin(), cards.rbegin() + drawIndex, random);
            drawIndex = 0;
        }   
};
#endif