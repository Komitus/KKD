#include <cmath>
#include "encoding.hpp"

#define numOfStats 4

void stats(std::string originalFilename, std::string editedFilename)
{
    TGA original(originalFilename);
    TGA edited(editedFilename);

    uint32_t *errors = calcErrors(original.bitMap, edited.bitMap);
    displayStats(errors, original.bitMap);
}

void displayStats(uint32_t *errors, BitMap &original)
{
    double size = original.size();
    double mses[] = {
        errors[0] / size,
        errors[1] / size,
        errors[2] / size,
        errors[3] / (size * pixelSize)};

    std::cout << "\nmse: " << mses[3]
              << "\nmse(r): " << mses[2]
              << "\nmse(g): " << mses[1]
              << "\nmse(b): " << mses[0] << '\n';

    double snrs[4] = {{0}};
    double sums[4] = {{0}};

    for (auto pixel : original)
    {
        for (int j = 0; j < numOfStats - 1; j++)
        {
            uint32_t tmp = pow(pixel[j], 2);
            sums[j] += tmp;
            sums[numOfStats - 1] += tmp;
        }
    }

    for (int i = 0; i < numOfStats - 1; i++)
    {
        snrs[i] = sums[i] / errors[i];
    }
    snrs[numOfStats - 1] = sums[numOfStats - 1] / errors[numOfStats - 1];

    std::cout << "\nsnr: " << snrs[3]
              << "\nsnr(r): " << snrs[2]
              << "\nsnr(g): " << snrs[1]
              << "\nsnr(b): " << snrs[0] << '\n';
}

uint32_t *calcErrors(BitMap &original, BitMap &edited)
{
    uint32_t *errors = new uint32_t[numOfStats];
    for (uint8_t i = 0; i < numOfStats; i++)
        errors[i] = 0;

    for (size_t i = 0; i < original.size(); i++)
    {   
        for (int j = 0; j < numOfStats - 1; j++)
        {       
            uint32_t tmp = pow((original[i][j] - edited[i][j]), 2);
            errors[j] += tmp;
            errors[numOfStats - 1] += tmp;
        }
    }
    return errors;
}
