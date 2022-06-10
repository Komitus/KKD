#ifndef _tga
#define _tga

#include "tuple"
#include <string>
#include <vector>

typedef std::tuple<double, double, double> Pixel;
typedef std::vector<Pixel> PixelVec;
typedef std::vector<Pixel> CodeBook;

struct TGA
{
    uint32_t width;
    uint32_t height;
    PixelVec bitMap;
    std::vector<uint8_t> header;
    std::vector<uint8_t> footer;
    TGA(std::string filename);
    void newImage(PixelVec &inBitmap, std::string fileName);
};

#endif