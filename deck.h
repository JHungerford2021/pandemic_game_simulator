#include "gameConstants.h"
#include "cities.h"
#include <random>
#include <numeric>
#include <iostream>
#include <sstream>

#ifndef DECK
#define DECK

class abstractCard
{
    friend std::ostream& operator << (std::ostream& lhs, abstractCard* rhs)
    {
        return rhs->write(lhs);
    }
    
    protected:

        int_fast16_t cardNumber;

        abstractCard(int_fast16_t n)
            :cardNumber {n}
        {}

        virtual std::ostream& write (std::ostream& lhs) = 0;
        
    public:

        virtual int_fast16_t getNumber()
        {
            return cardNumber;
        }

        virtual bool isEvent() = 0;
};

class Card
{
    friend std::ostream& operator << (std::ostream& lhs, const Card& rhs)
    {
        return lhs << rhs.p;
    }

    friend bool operator == (const Card& lhs, const Card& rhs)
    {
        return lhs.p == rhs.p;
    }

    friend bool operator != (const Card& lhs, const Card& rhs)
    {
        return lhs.p != rhs.p;
    }

    private:

        abstractCard* p;

    public:

        Card(abstractCard* point)
            : p{point}
        {}

        Card(const Card& rhs)
        {
            this->p = rhs.p;
        }

        Card(Card&& rhs)
        {
            this->p = rhs.p;
            rhs.p = nullptr;
        }

        Card& operator= (const Card& rhs)
        {
            delete this->p;
            this->p = rhs.p;
            return *this;
        }

        Card& operator= (Card&& rhs)
        {
            delete this->p;
            this->p = rhs.p;
            rhs.p = nullptr;
            return *this;
        }
        
        ~Card()
        {
            delete p;
        }

        bool operator==(const Card& rhs) const
        {
            return this->p == rhs.p;
        }

        int_fast16_t getNumber() const
        {
            return p->getNumber();
        }

        bool isEvent() const
        {
            return p->isEvent();
        }
};

class cardHash
{
    public:

        size_t operator()(const Card& c) const
        {
            size_t result = c.isEvent() ? numCityCards : 0;
            return result + c.getNumber();

        }
};

class playerCard : public abstractCard
{
    private:

        bool event;

    protected:

        virtual std::ostream& write (std::ostream& lhs)
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
            :abstractCard{n}, event{e}
        {}

        playerCard(playerCard&& rhs)
            :abstractCard(rhs.cardNumber), event{rhs.event}
        {}

        playerCard& operator= (const playerCard& rhs)
        {
            cardNumber = rhs.cardNumber;
            event = rhs.event;
            return *this;
        }

        playerCard& operator= (playerCard&& rhs)
        {
            cardNumber = rhs.cardNumber;
            event = rhs.event;
            return *this;
        }

        bool isEvent()
        {
            return event;
        }
};

class infectionCard : public abstractCard
{
    protected:

        virtual std::ostream& write (std::ostream& lhs)
        {
            return lhs << cardNumber << ' ';
        }

    public:

        infectionCard(int_fast16_t n)
            :abstractCard{n}
        {}

        bool isEvent()
        {
            return false;
        }
};


class Deck 
{
    
    friend std::ostream& operator << (std::ostream& lhs, Deck& rhs) {
        return rhs.write(lhs);
    }

    protected:

        std::vector<Card> cards;
        std::mt19937_64& random;
        int_fast16_t drawIndex{0};
        
        Deck(std::mt19937_64& r, int_fast16_t size)
            : random {r}
        {
            cards.reserve(size);
        }

        virtual std::ostream& write(std::ostream& lhs) = 0;

    public:

        virtual const Card& drawCard()
        {
            const Card& result = *(cards.crbegin() + drawIndex);
            ++drawIndex;
            return result;
        }
};

class playerDeck : public Deck
{
    private:

        void getDeckSizes(std::vector<int_fast16_t>& deckSizes, int_fast16_t numEpidemicCards)
        {
            int_fast16_t deckSize = static_cast<int_fast16_t>(cards.size()) - drawIndex;
            int_fast16_t minSize = 1;
            int_fast16_t sumOfMins = ((numEpidemicCards + minSize) * (numEpidemicCards >> 1)) + ((numEpidemicCards & 1) * ((numEpidemicCards + minSize) >> 1));
            int_fast16_t multiplesThatFit = (deckSize - sumOfMins) / numEpidemicCards;
            minSize += multiplesThatFit;
            std::iota(deckSizes.begin(), deckSizes.end(), minSize);
            int_fast16_t remainder = deckSize - (sumOfMins + (multiplesThatFit * numEpidemicCards));
            std::unordered_set<int_fast16_t> takenValues;
            while (remainder > 0)
            {
                int_fast16_t deckSelection = random() % numEpidemicCards;
                while (takenValues.find(deckSelection) != takenValues.end())
                {
                    deckSelection = random() % numEpidemicCards;
                }
                takenValues.insert(deckSelection);
                ++deckSizes[deckSelection];
                --remainder;
            }
        }

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

    protected:

        virtual std::ostream& write(std::ostream& lhs)
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

        playerDeck(std::mt19937_64& r, int_fast16_t numEpidemicCards)
            : Deck{r, numPlayerCards + numEpidemicCards}
        {
            playerCard* epidemic = new playerCard{epidemicCard};
            for (int_fast16_t i = 0; i < numEpidemicCards; ++i)
            {
                cards.emplace_back(Card{epidemic});
            }
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards.emplace_back(Card{new playerCard{i}});
            }
            for (int_fast16_t i = 0; i < numEventCards; ++i)
            {
                cards.emplace_back(Card{new playerCard{i, 1}});
            }
            std::shuffle(cards.begin() + numEpidemicCards, cards.end(), random);
        }

        void prepareDeck(int_fast16_t numEpidemicCards)
        {
            std::vector<int_fast16_t> deckSizes(numEpidemicCards);
            getDeckSizes(deckSizes, numEpidemicCards);
            int_fast16_t index = numEpidemicCards - 1;
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

class infectionDeck : public Deck
{
    private:
        int_fast16_t epidemicIndex{0};

    protected:

        virtual std::ostream& write(std::ostream& lhs)
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

        infectionDeck(std::mt19937_64& r, int_fast16_t numEpidemicCards)
            : Deck{r, numCityCards + numEpidemicCards}
        {
            for (int_fast16_t i = 0; i < numCityCards; ++i)
            {
                cards.emplace_back(Card{new infectionCard{i}});
            }
            std::shuffle(cards.begin(), cards.end(), random);
        }

        const Cities infect()
        {
            cards.emplace_back(cards[epidemicIndex]);
            ++drawIndex;
            return static_cast<Cities>(cards[epidemicIndex++].getNumber());
        }

        std::vector<Card>::reverse_iterator getIndexOfCityToRemove(Cities cityToRemove)
        {
            auto begin = cards.rbegin();
            bool found = static_cast<Cities>((*begin).getNumber()) == cityToRemove;
            while (!found)
            {
                ++begin;
                found = static_cast<Cities>((*begin).getNumber()) == cityToRemove; 
            }
            return begin;

        }

        void intensify(Cities cityToRemove, bool removeCity)
        {
            if (removeCity)
            {
                if (cityToRemove != static_cast<Cities>(cards[epidemicIndex - 1].getNumber()))
                {
                    auto removedCity = getIndexOfCityToRemove(cityToRemove);
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