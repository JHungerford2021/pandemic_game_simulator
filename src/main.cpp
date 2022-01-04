#include "game.h"

inline int getIntFromUser(const std::string& message) 
{
    bool validInput = 0;
    std::int_fast16_t result = 0;
    while (!validInput)
    {
        std::cout << message;
        std::string input{};
        std::cin >> input;
        std::stringstream convert{ input };
        if (convert >> result)
        {
            validInput = 1;
        }
    }
    return result;
}

int main(int argc, char *argv[]) 
{
    constexpr int_fast64_t roles = static_cast<int_fast64_t>('C' << 24) + static_cast<int_fast64_t>('C' << 16) + static_cast<int_fast64_t>('C' << 8) + static_cast<int_fast64_t>('C');
    std::vector<Game<roles>> games;
    constexpr int_fast16_t gamesAtOnce = 1000;
    games.reserve(gamesAtOnce);
    Timer t;
    for (int_fast16_t i = 0; i < gamesAtOnce; ++i)
    {
        games.emplace_back(Game<roles>{i});
    }
    std::cout << "Time elapsed: " << t.elapsed() << " seconds\n";
    return 0;
}