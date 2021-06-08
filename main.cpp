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
    Game g{0, "CMQR", 6, 4};
    return 0;
}