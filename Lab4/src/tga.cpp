#include "tga.hpp"
#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>

TGA::TGA(std::string inFilename)
{
    std::ifstream input(inFilename, std::ios::binary);
    if (!input)
    {
        std::cerr << "No such file\n";
        exit(1);
    }
    bytes = std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    width = (bytes[13] << 8) + bytes[12]; // little endian
    height = (bytes[15] << 8) + bytes[14];

    // std::cout<<width << '\t' <<height <<'\n';

    parseBitmap();
}

void TGA::parseBitmap()
{
    bitMap.init(height, width);

    // start with 18 pos bcs of header
    // size_t prevSize = bytes.size();
    slice(bytes, 18, bytes.size());

    // std::cout << prevSize << " " << bytes.size() << " " << prevSize - bytes.size() << '\n';
    // std::cout << "SIZE: " << height * width << '\n';
    uint32_t rowPos;
    uint32_t colPos;

    for (uint32_t i = 0; i < height; i++)
    {
        rowPos = width * i;
        for (uint32_t j = 0; j < width; j++)
        {
            colPos = (rowPos + j) * 3;
            // rows are reversed
            bitMap[height - i - 1][j] = Pixel(bytes[colPos], bytes[colPos + 1], bytes[colPos + 2]);
        }
    }
    bytes.clear();
    bytes.shrink_to_fit();
}

void jpegLs(PixelArr &bitMap, PixelArr &predictionBitMap, const predictor predictor)
{
    predictionBitMap.init(bitMap.height, bitMap.width);

    Pixel emptyPixel(0, 0, 0);
    Pixel NW, W, N;

    for (uint32_t i = 0; i < bitMap.height; i++)
    {
        for (uint32_t j = 0; j < bitMap.width; j++)
        {

            if (i == 0)
                N = emptyPixel;
            else
                N = bitMap[i - 1][j];

            if (j == 0)
                W = emptyPixel;
            else
                W = bitMap[i][j - 1];

            if (i == 0 || j == 0)
                NW = emptyPixel;
            else
                NW = bitMap[i - 1][j - 1];

            predictionBitMap[i][j] = Pixel(bitMap[i][j] - predictor(NW, N, W));
        }
    }
}

double calculateEntropy(PixelArr &inBitMap, Colors color)
{
    double entropy;

    uint32_t occurrences[256] = {{0}};
    uint32_t counter = 0;

    for (uint32_t i = 0; i < inBitMap.height; i++)
    {
        for (uint32_t j = 0; j < inBitMap.width; j++)
        {
            switch (color)
            {
            case RED:
                occurrences[inBitMap[i][j].red]++;
                counter++;
                break;
            case GREEN:
                occurrences[inBitMap[i][j].green]++;
                counter++;
                break;
            case BLUE:
                occurrences[inBitMap[i][j].blue]++;
                counter++;
                break;
            default:
                occurrences[inBitMap[i][j].red]++;
                occurrences[inBitMap[i][j].green]++;
                occurrences[inBitMap[i][j].blue]++;
                counter += 3;
                break;
            }
        }
    }
    entropy = 0.0;
    double cbLog;
    double sizeLog = log2(counter);

    for (uint16_t currByte = 0; currByte < 256; currByte++)
    {
        if (occurrences[currByte] != 0)
        {
            cbLog = log2(occurrences[currByte]);
            entropy += (sizeLog - cbLog) * occurrences[currByte];
        }
    }
    entropy /= (double)counter;

    return entropy;
}

Entropies getEntropies(PixelArr &inBitMap)
{
    return {
        calculateEntropy(inBitMap, GENERAL),
        calculateEntropy(inBitMap, RED),
        calculateEntropy(inBitMap, GREEN),
        calculateEntropy(inBitMap, BLUE)};
}