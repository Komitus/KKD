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
    std::vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    width = (bytes[13] << 8) + bytes[12]; // little endian
    height = (bytes[15] << 8) + bytes[14];

    // std::cout<<width << '\t' <<height <<'\n';

    bitMap.reserve(height * width);
    header = std::vector<uint8_t>(bytes.begin(), bytes.begin() + 18);
    footer = std::vector<uint8_t>(bytes.end() - 26, bytes.end());

    //std::cout<<"Header: "<<header.size() << "\nFooter: " << footer.size() << '\n';
    uint32_t idx;
    bytes.erase(bytes.begin(), bytes.begin() + 18);

    for (uint32_t i = 0; i < (height * width); i++)
    {    
        idx=i*3;
        bitMap.emplace_back(bytes[idx], bytes[idx + 1], bytes[idx + 2]);
    }
}

void TGA::newImage(PixelVec &inBitmap, std::string fileName)
{
    std::vector<uint8_t> bytes(header);
    bytes.reserve(header.size() + inBitmap.size() * 3 + footer.size());
    
    for (auto it = inBitmap.begin(); it != inBitmap.end(); ++it)
    {   
        bytes.push_back((uint8_t)std::round(std::get<0>(*it)));
        bytes.push_back((uint8_t)std::round(std::get<1>(*it)));
        bytes.push_back((uint8_t)std::round(std::get<2>(*it)));
    }
    bytes.insert(bytes.end(), footer.begin(), footer.end());

    std::ofstream outFile(fileName, std::ios::binary);
    outFile.write((const char *)&bytes[0], bytes.size());
    outFile.close();
}