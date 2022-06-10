#ifndef _tga
#define _tga

#include "pixel.hpp"
#include <string>
#include <vector>

#define BytesVec std::vector<uint8_t>
#define pixelSize 3
#define BitMap std::vector<Pixel>
#define ByteSize 8

struct TGA
{
    uint32_t width;
    uint32_t height;
    BytesVec header;
    BytesVec footer;
    BitMap bitMap;
    TGA(std::string filename);
    void newImage(BitMap &inBitmap, std::string fileName);
    void newImage(BytesVec &inBytesVec, std::string fileName);
};

#endif