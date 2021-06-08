#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>

#ifndef GAME_CONSTANTS
#define GAME_CONSTANTS

enum class Color : int_fast16_t
{
    blue,
    yellow,
    black,
    red
};

enum class Cities : int_fast16_t
{
    // blue cities
    atlanta,
    chicago,
    essen,
    london,
    madrid,
    milan,
    montreal,
    newYork,
    paris,
    sanFrancisco,
    stPetersburg,
    washington,
    // yellow cities
    bogota,
    buenosAires,
    johannesburg,
    khartoum,
    kinshasa,
    lagos,
    lima,
    losAngeles,
    mexicoCity,
    miami,
    santiago,
    saoPaulo,
    // black cities
    algiers,
    baghdad,
    cairo,
    chennai,
    delhi,
    istanbul,
    karachi,
    kolkata,
    moscow,
    mumbai,
    riyadh,
    tehran,
    // red cities
    bangkok,
    beijing,
    hoChiMinhCity,
    hongKong,
    jakarta,
    manila,
    osaka,
    seoul,
    shanghai,
    sydney,
    taipei,
    tokyo   
};

std::ostream& operator << (std::ostream& lhs, Color rhs)
{
    return lhs << static_cast<int_fast16_t>(rhs);
}

// Gameplay Constants
inline constexpr std::int_fast16_t numCities = 48;
inline constexpr std::int_fast16_t numResearchStations = 6;
inline constexpr std::int_fast16_t alive = 0;
inline constexpr std::int_fast16_t cured = 1;
inline constexpr std::int_fast16_t eradicated = 2;
inline constexpr std::int_fast16_t numDiseases = 4;
inline constexpr std::int_fast16_t diseaseCubesPerColor = 24;
inline constexpr std::int_fast16_t maxOutbreaks = 7;
inline constexpr std::int_fast16_t maxInfection = 4;

// Infection Rate Constants
inline constexpr std::int_fast16_t minInfectionRate = 2;
inline constexpr std::int_fast16_t maxInfectionRate = 4;
inline constexpr std::int_fast16_t firstRateIncreaseIndex = 3;
inline constexpr std::int_fast16_t rateIncreaseRate = 1;
inline constexpr std::int_fast16_t infectionRateIncrease = 1;

// Deck Constants
inline constexpr std::int_fast16_t numEventCards = 5;
inline constexpr std::int_fast16_t numCityCards = numCities;
inline constexpr std::int_fast16_t numPlayerCards = numEventCards + numCityCards;
inline constexpr std::int_fast16_t epidemicCard = -1;
inline constexpr std::int_fast16_t cardsIfTwo = 4;
inline constexpr std::int_fast16_t cardsIfThree = 3;
inline constexpr std::int_fast16_t cardsIfFour = 2;
inline constexpr std::int_fast16_t numWaves = 3;
inline constexpr std::int_fast16_t strongestWave = 3;
inline constexpr std::int_fast16_t citiesPerWave = 3;

// City Constants
inline constexpr std::array<int_fast32_t, numCities> cityPopulations = 
{
	// blue cities
	4715000,
	9121000,
	575000,
	8586000,
	5427000,
	5232000,
	3429000,
	20464000,
	10755000,
	5864000,
	4879000,
	4679000,
	// yellow cities
	8702000,
	13639000,
	3888000,
	4887000,
	9046000,
	11547000,
	9121000,
	14900000,
	19463000,
	5582000,
	6015000,
	20186000,
	// black cities
	2946000,
	6204000,
	14718000,
	8865000,
	22242000,
	13576000,
	20711000,
	14374000,
	15512000,
	16910000,
	5037000,
	7419000,
	// red cities
	17311000,
	7151000,
	8314000,
	7106000,
	26063000,
	20767000,
	2871000,
	22547000,
	13482000,
	3785000,
	8338000,
	13189000
};

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
#endif