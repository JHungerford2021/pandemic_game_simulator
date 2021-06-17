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

    private:

        virtual std::ostream& write (std::ostream& lhs) const = 0;

    protected:

        int_fast16_t cardNumber;

    public:

        constexpr Card(int_fast16_t n)
            : cardNumber{n}
        {}

        constexpr Card(const Card& rhs)
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

class playerCard : public Card
{
    private:

        bool event;

        std::ostream& write (std::ostream& lhs) const
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

        constexpr playerCard(int_fast16_t n, bool e = 0)
            :Card{n}, event{e}
        {}

        constexpr playerCard(const playerCard& rhs)
            :Card{rhs.cardNumber}, event{rhs.event}
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
    private:

        std::ostream& write (std::ostream& lhs) const
        {
            return lhs << cardNumber << ' ';
        }

    public:

        constexpr infectionCard(int_fast16_t n)
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

    private:

        virtual std::ostream& write(std::ostream& lhs) const = 0;

    protected:

        int_fast16_t drawIndex{0};

    public:

        virtual const C& drawCard() = 0;
};

template <int_fast16_t gameDifficulty>
class playerDeck: public Deck<playerCard>
{
    private:

        std::vector<playerCard> cards
        {
            playerCard{0},
            playerCard{1},
            playerCard{2},
            playerCard{3},
            playerCard{4},
            playerCard{5},
            playerCard{6},
            playerCard{7},
            playerCard{8},
            playerCard{9},
            playerCard{10},
            playerCard{11},
            playerCard{12},
            playerCard{13},
            playerCard{14},
            playerCard{15},
            playerCard{16},
            playerCard{17},
            playerCard{18},
            playerCard{19},
            playerCard{20},
            playerCard{21},
            playerCard{22},
            playerCard{23},
            playerCard{24},
            playerCard{25},
            playerCard{26},
            playerCard{27},
            playerCard{28},
            playerCard{29},
            playerCard{30},
            playerCard{31},
            playerCard{32},
            playerCard{33},
            playerCard{34},
            playerCard{35},
            playerCard{36},
            playerCard{37},
            playerCard{38},
            playerCard{39},
            playerCard{40},
            playerCard{41},
            playerCard{42},
            playerCard{43},
            playerCard{44},
            playerCard{45},
            playerCard{46},
            playerCard{47},
            playerCard{0, 1},
            playerCard{1, 1},
            playerCard{2, 1},
            playerCard{3, 1},
            playerCard{4, 1}
        };
        std::mt19937_64& random;

        std::array<int_fast16_t, gameDifficulty> getDeckSizes() const
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes;
            int_fast16_t deckSize = numPlayerCards + gameDifficulty - drawIndex;
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

        std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            for (auto begin = cards.begin() + drawIndex, end = cards.end(); begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        constexpr playerDeck(std::mt19937_64& r) noexcept
            : random{r}
        {
            for (int_fast16_t i = 0; i < gameDifficulty; ++i)
            {
                cards.emplace_back(playerCard{epidemicCard});
            }
            std::shuffle(cards.begin(), cards.begin() + numPlayerCards, random);
        }

        void prepareDeck() noexcept
        {
            std::array<int_fast16_t, gameDifficulty> deckSizes = getDeckSizes();
            int_fast16_t index = gameDifficulty - 1;
            auto miniDeckStart = cards.begin() + drawIndex;
            auto epidemicCardIterator = cards.rbegin();
            while (index >= 0)
            {
                int_fast16_t deckSize = deckSizes[index];
                int_fast16_t swapIndex = random() % deckSize;
                std::swap(*(epidemicCardIterator + index), *(miniDeckStart + swapIndex));
                miniDeckStart += deckSize;
                --index;
            }
        }

        const playerCard& drawCard() noexcept
        {
            const playerCard& result = *(cards.begin() + drawIndex);
            ++drawIndex;
            return result;
        }  
};

template<int_fast16_t gameDifficulty>
class infectionDeck : public Deck<infectionCard>
{
    private:

        std::vector<infectionCard> cards
        {
            infectionCard{0},
            infectionCard{1},
            infectionCard{2},
            infectionCard{3},
            infectionCard{4},
            infectionCard{5},
            infectionCard{6},
            infectionCard{7},
            infectionCard{8},
            infectionCard{9},
            infectionCard{10},
            infectionCard{11},
            infectionCard{12},
            infectionCard{13},
            infectionCard{14},
            infectionCard{15},
            infectionCard{16},
            infectionCard{17},
            infectionCard{18},
            infectionCard{19},
            infectionCard{20},
            infectionCard{21},
            infectionCard{22},
            infectionCard{23},
            infectionCard{24},
            infectionCard{25},
            infectionCard{26},
            infectionCard{27},
            infectionCard{28},
            infectionCard{29},
            infectionCard{30},
            infectionCard{31},
            infectionCard{32},
            infectionCard{33},
            infectionCard{34},
            infectionCard{35},
            infectionCard{36},
            infectionCard{37},
            infectionCard{38},
            infectionCard{39},
            infectionCard{40},
            infectionCard{41},
            infectionCard{42},
            infectionCard{43},
            infectionCard{44},
            infectionCard{45},
            infectionCard{46},
            infectionCard{47}
        };
        std::mt19937_64& random;
        int_fast16_t epidemicIndex{0};

        std::ostream& write(std::ostream& lhs) const
        {
            std::stringstream result{};
            for (auto begin = cards.begin() + epidemicIndex, end = cards.end() - drawIndex; begin != end; ++begin)
            {
                result << *(begin);
            }
            result << std::endl;
            return lhs << result.str();
        }

    public:

        constexpr infectionDeck(std::mt19937_64& r) noexcept
            : random{r}
        {
            std::shuffle(cards.begin(), cards.end(), random);
        }

        const Cities infect() noexcept
        {
            cards.emplace_back(cards[epidemicIndex]);
            ++drawIndex;
            return cards[epidemicIndex++].getNumber<Cities>();
        }

        void intensify(Cities cityToRemove, bool removeCity) noexcept
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

        const infectionCard& drawCard() noexcept
        {
            const infectionCard& result = *(cards.rbegin() + drawIndex);
            ++drawIndex;
            return result;
        }   
};
#endif