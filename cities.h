#include "gameConstants.h"
#include <sstream>
#include <array>
#include <unordered_set>

#ifndef CITIES
#define CITIES

std::ostream& operator << (std::ostream& lhs, Cities rhs)
{
    return lhs << static_cast<int_fast16_t>(rhs);
}

class City
{
    friend std::ostream& operator << (std::ostream& lhs, const City& rhs)
    {
        std::stringstream result{};
        result << "Color: " << rhs.color << '\n';
        result << "Infection Counts: ";
        for (int_fast16_t disease = 0; disease < numDiseases; ++disease)
        {
            result << rhs.infectionCounts[disease] << ' ';
        }
        result << std::endl;
        return lhs << result.str();
    }

    private:
        std::array<int_fast16_t, numDiseases> infectionCounts{0, 0, 0, 0};
        const std::unordered_set<Cities>& adjacentCities;

    public:

        const Color color;

        constexpr City(std::unordered_set<Cities>&& cities, const Color c = Color::blue)
            :adjacentCities {cities}, color {c}
        {}

        const std::unordered_set<Cities>& getAdjacentCities() const
        {
            return adjacentCities;
        }

        bool isAdjacent(const Cities city) const
        {
            return adjacentCities.find(city) != adjacentCities.end();
        }

        bool addInfection(const std::int_fast16_t count, const Color c)
        {
            infectionCounts[static_cast<int_fast16_t>(c)] += count;
            if (infectionCounts[static_cast<int_fast16_t>(c)] > 3)
            {
                infectionCounts[static_cast<int_fast16_t>(c)] = 3;
                return 1;
            }
            return 0;
        }
};
#endif