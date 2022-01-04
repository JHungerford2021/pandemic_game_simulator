#include "gameConstants.h"

class Disease
{
    private:

        int_fast16_t cubesLeft = diseaseCubesPerColor;
        int_fast16_t status = alive;
        const Color color;

    public:

        constexpr Disease(Color c = Color::blue)
            :color {c}
        {}

        bool adjustCubes(const int_fast16_t count)
        {
            cubesLeft += count;
            if (cubesLeft == diseaseCubesPerColor && status == cured)
            {
                this->changeStatus(eradicated);
            }
            return cubesLeft >= 0;
        }

        bool isCured()
        {
            return status == cured;
        }

        bool isEradicated()
        {
            return status == eradicated;
        }

        void changeStatus(const int_fast16_t status)
        {
            this->status = status; 
        }
};