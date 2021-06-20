#include "cities.h"
#include "diseases.h"
#include "deck.h"
#include "players.h"
#include <string>
#include <sstream>

class Timer
{
private:
	// Type aliases to make accessing nested type easier
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;
	
	std::chrono::time_point<clock_t> m_beg;
 
public:
	Timer() : m_beg(clock_t::now())
	{
	}
	
	void reset()
	{
		m_beg = clock_t::now();
	}
	
	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

template <int_fast64_t seed = 0>
class Game
{
    private:

        std::mt19937_64 random{seed};
        const std::array<City, numCities> cities
        {
            City(std::unordered_set<Cities>{Cities::chicago, Cities::washington, Cities::miami}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::atlanta, Cities::montreal, Cities::sanFrancisco, Cities::losAngeles, Cities::mexicoCity}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::london, Cities::milan, Cities::paris, Cities::stPetersburg}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::essen, Cities::madrid, Cities::newYork, Cities::paris}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::london, Cities::newYork, Cities::paris, Cities::saoPaulo, Cities::algiers}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::essen, Cities::paris, Cities::istanbul}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::chicago, Cities::newYork, Cities::washington}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::london, Cities::madrid, Cities::montreal, Cities::washington}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::essen, Cities::london, Cities::madrid, Cities::milan, Cities::algiers}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::chicago, Cities::manila, Cities::tokyo, Cities::losAngeles}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::essen, Cities::istanbul, Cities::moscow}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::atlanta, Cities::montreal, Cities::newYork, Cities::miami}, Color::blue),
	        City(std::unordered_set<Cities>{Cities::buenosAires, Cities::lima, Cities::mexicoCity, Cities::miami, Cities::saoPaulo}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::bogota, Cities::saoPaulo}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::khartoum, Cities::kinshasa}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::johannesburg, Cities::kinshasa, Cities::lagos, Cities::cairo}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::johannesburg, Cities::khartoum, Cities::lagos}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::khartoum, Cities::kinshasa, Cities::saoPaulo}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::bogota, Cities::mexicoCity, Cities::santiago}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::sanFrancisco, Cities::mexicoCity, Cities::sydney}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::chicago, Cities::bogota, Cities::lima, Cities::losAngeles, Cities::miami}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::atlanta, Cities::washington, Cities::bogota, Cities::mexicoCity}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::lima}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::madrid, Cities::bogota, Cities::buenosAires, Cities::lagos}, Color::yellow),
	        City(std::unordered_set<Cities>{Cities::madrid, Cities::paris, Cities::cairo, Cities::istanbul}, Color::black),
	        City(std::unordered_set<Cities>{Cities::cairo, Cities::istanbul, Cities::karachi, Cities::moscow, Cities::riyadh, Cities::tehran}, Color::black),
	        City(std::unordered_set<Cities>{Cities::khartoum, Cities::algiers, Cities::baghdad, Cities::istanbul, Cities::riyadh}, Color::black),
	        City(std::unordered_set<Cities>{Cities::delhi, Cities::kolkata, Cities::mumbai, Cities::bangkok, Cities::jakarta}, Color::black),
	        City(std::unordered_set<Cities>{Cities::chennai, Cities::karachi, Cities::kolkata, Cities::mumbai, Cities::tehran}, Color::black),
	        City(std::unordered_set<Cities>{Cities::milan, Cities::stPetersburg, Cities::algiers, Cities::baghdad, Cities::cairo, Cities::moscow}, Color::black),
	        City(std::unordered_set<Cities>{Cities::baghdad, Cities::delhi, Cities::mumbai, Cities::riyadh, Cities::tehran}, Color::black),
	        City(std::unordered_set<Cities>{Cities::chennai, Cities::delhi, Cities::bangkok, Cities::hongKong}, Color::black),
	        City(std::unordered_set<Cities>{Cities::stPetersburg, Cities::istanbul, Cities::tehran}, Color::black),
	        City(std::unordered_set<Cities>{Cities::chennai, Cities::delhi, Cities::karachi}, Color::black),
	        City(std::unordered_set<Cities>{Cities::baghdad, Cities::cairo, Cities::karachi}, Color::black),
	        City(std::unordered_set<Cities>{Cities::baghdad, Cities::delhi, Cities::karachi, Cities::moscow}, Color::black),
	        City(std::unordered_set<Cities>{Cities::chennai, Cities::kolkata, Cities::hoChiMinhCity, Cities::hongKong, Cities::jakarta}, Color::red),
	        City(std::unordered_set<Cities>{Cities::seoul, Cities::shanghai}, Color::red),
	        City(std::unordered_set<Cities>{Cities::bangkok, Cities::hoChiMinhCity, Cities::hongKong, Cities::manila}, Color::red),
	        City(std::unordered_set<Cities>{Cities::kolkata, Cities::bangkok, Cities::hoChiMinhCity, Cities::manila, Cities::shanghai, Cities::taipei}, Color::red),
	        City(std::unordered_set<Cities>{Cities::chennai, Cities::bangkok, Cities::hoChiMinhCity, Cities::sydney}, Color::red),
	        City(std::unordered_set<Cities>{Cities::sanFrancisco, Cities::hoChiMinhCity, Cities::hongKong, Cities::sydney, Cities::taipei}, Color::red),
	        City(std::unordered_set<Cities>{Cities::taipei, Cities::tokyo}, Color::red),
	        City(std::unordered_set<Cities>{Cities::beijing, Cities::shanghai, Cities::tokyo}, Color::red),
	        City(std::unordered_set<Cities>{Cities::beijing, Cities::hongKong, Cities::seoul, Cities::taipei, Cities::tokyo}, Color::red),
	        City(std::unordered_set<Cities>{Cities::losAngeles, Cities::jakarta, Cities::manila}, Color::red),
	        City(std::unordered_set<Cities>{Cities::hongKong, Cities::manila, Cities::osaka, Cities::shanghai}, Color::red),
	        City(std::unordered_set<Cities>{Cities::sanFrancisco, Cities::osaka, Cities::seoul, Cities::shanghai}, Color::red)
        };
        std::array<int_fast16_t, maxOutbreaks + gameDifficulty + 1> infectionRates;
        std::unordered_set<Cities> researchStations { Cities::atlanta };
        std::array<Disease, numDiseases> diseases{Color::blue, Color::yellow, Color::black, Color::red};
        playerDeck pDeck;
        infectionDeck iDeck;

        constexpr void initializeInfectionRates()
        {
            int_fast16_t currentRate = minInfectionRate;
            int_fast16_t indiciesToGo = firstRateIncreaseIndex;
            auto beginRange = infectionRates.begin();
            auto endRange = beginRange + indiciesToGo;
            auto end = infectionRates.end();
            while (currentRate < maxInfectionRate && beginRange < end && endRange < end)
            {
                std::fill(beginRange, endRange, currentRate);
                currentRate += infectionRateIncrease;
                beginRange = endRange;
                if (indiciesToGo - rateIncreaseRate > 0)
                {
                    indiciesToGo -= rateIncreaseRate;
                }
                endRange = beginRange + indiciesToGo;
            }
            std::fill(beginRange, end, currentRate);
        }

        template <class C>
        C createDeck()
        {
            C result{random};
            result.beginningShuffle();
            return result;
        }

        constexpr int cardsPerPlayer()
        {
            if (numPlayers == 2)
                return cardsIfTwo;
            else if  (numPlayers == 3)
                return cardsIfThree;
            else
                return cardsIfFour;
        }

        inline void dealPlayerCards()
        {
            for (int_fast16_t player = 0; player < numPlayers; ++player)
            {
                for (int_fast16_t cardsDealt = 0; cardsDealt < cardsPerPlayer(); cardsDealt++)
                {
                    pDeck.drawCard();
                }
            }
        }

        inline void initialInfections()
        {
            for (int_fast16_t wave = 0; wave < numWaves; ++wave)
            {
                for (int_fast16_t city = 0; city < citiesPerWave; ++city)
                {
                    const infectionCard& iCard = iDeck.drawCard();
                    City target = cities[iCard.getNumber<int_fast16_t>()];
                    target.addInfection(strongestWave - wave, target.color);
                }
            }
        }

    public:

        Game(const std::string& roleString)
            : pDeck{createDeck<playerDeck>()}, iDeck{createDeck<infectionDeck>()}
        {
            initializeInfectionRates();
            dealPlayerCards();
            pDeck.prepareDeck();
            initialInfections();
        }
};