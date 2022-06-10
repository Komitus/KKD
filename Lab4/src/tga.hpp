#ifndef _tga
#define _tga

#include "pixel.hpp"
#include "2d_array.hpp"
#include <string>
#include <vector>

typedef array_2d<Pixel> PixelArr;

enum Colors
{
    GENERAL,
    RED,
    GREEN,
    BLUE
};

/**
 * tga has bitmap written in LSF (least significant bit)
 * to read RGB we read BGR
 */

struct TGA
{
    uint32_t width;
    uint32_t height;
    std::vector<uint8_t> bytes;
    PixelArr bitMap;
    TGA(std::string filename);
    void parseBitmap();
};

void jpegLs(PixelArr &bitMap, PixelArr &predictionBitMap, const predictor predictor);

struct Entropies
{
    double general;
    double red;
    double green;
    double blue;

    friend std::ostream &operator<<(std::ostream &os, const Entropies &obj)
    {
        return os << "general: \033[97m" << obj.general << "\033[0m\n"
                  << "red: \033[31m" << obj.red << "\033[0m\n"
                  << "green: \033[32m" << obj.green << "\033[0m\n"
                  << "blue: \033[34m" << obj.blue << "\033[0m\n";
    }
};

double calculateEntropy(PixelArr &inBitMap, Colors color);
Entropies getEntropies(PixelArr &inBitMap);

template <typename T>
void slice(std::vector<T> &v, size_t m, int n)
{
    v.erase(v.cbegin(), v.cbegin() + m);
    if (n > 0)
    {
        v.erase(v.cbegin() - m + n, v.cend());
    }
    else
    {
        v.erase(v.cend() + n, v.cend());
    }
}

#endif