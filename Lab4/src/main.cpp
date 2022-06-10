#include "tga.hpp"
#include <iostream>

#define MAX_DOUBLE 1.7976931348623157E+308

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Please enter a file name\n";
        return 1;
    }

    TGA tgaImage(argv[1]);
    Entropies entropies;

    std::cout << "------ ENTROPY FOR "
              << "\033[35m" << argv[1] << "\033[0m ----------\n"
              << getEntropies(tgaImage.bitMap) << '\n';

    double bestGeneral = MAX_DOUBLE;
    double bestRed = MAX_DOUBLE;
    double bestGreen = MAX_DOUBLE;
    double bestBlue = MAX_DOUBLE;

    uint8_t bestGeneralNo = numOfPredictors;
    uint8_t bestRedNo = numOfPredictors;
    uint8_t bestGreenNo = numOfPredictors;
    uint8_t bestBlueNo = numOfPredictors;

    PixelArr encodedMap;

    for (uint8_t i = 0; i < numOfPredictors; i++)
    {
        jpegLs(tgaImage.bitMap, encodedMap, predictors[i]);
        entropies = getEntropies(encodedMap);

        //std::cout << "Scheme[" << int(i + 1) << "]\n"
        //          << entropies << '\n';

        if (entropies.general < bestGeneral)
        {
            bestGeneral = entropies.general;
            bestGeneralNo = i;
        }

        if (entropies.red < bestRed)
        {
            bestRed = entropies.red;
            bestRedNo = i;
        }

        if (entropies.green < bestGreen)
        {
            bestGreen = entropies.green;
            bestGreenNo = i;
        }

        if (entropies.blue < bestBlue)
        {
            bestBlue = entropies.blue;
            bestBlueNo = i;
        }
    }

    std::cout << "------ BEST RESULTS FOR PREDICITONS ----------\n"
              << "in square bracket no. of predictor (starting with 1)\n"
              << "general "
              << "[" << int(bestGeneralNo + 1) << "] "
              << "\033[97m" << bestGeneral << "\033[0m\n"
              << "red "
              << "[" << int(bestRedNo + 1) << "] "
              << "\033[31m" << bestRed << "\033[0m\n"
              << "green "
              << "[" << int(bestGreenNo + 1) << "]"
              << "\033[32m" << bestGreen << "\033[0m\n"
              << "blue "
              << "[" << int(bestBlueNo + 1) << "] "
              << "\033[34m" << bestBlue << "\033[0m\n";

    return 0;
}
