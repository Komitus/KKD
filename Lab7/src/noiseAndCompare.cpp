#include "header.hpp"
#include <random>

std::random_device randDev;
std::mt19937 twister(randDev());
std::uniform_int_distribution<uint8_t> dist;

const auto getRand = []() -> uint8_t
{
    dist.param(std::uniform_int_distribution<uint8_t>::param_type(1, 100));
    return dist(twister);
};

void noise(const std::string &inFilename, const std::string &outFilename, uint8_t p)
{   
    static const size_t step = 8;
    std::string bitString;
    readFileToBitString(inFilename, bitString);
    const size_t outSize = bitString.size() / 8;
    BytesVec bytes(outSize);
    
    LOG("%s %ld\n%s %ld\n", "Noise:\nBitstring size:", bitString.size(), "resByte size:", outSize);

    for (size_t i = 0; i < bitString.size(); i++)
    {
        if (getRand() < p)
        {
            bitString[i] = !(bitString[i] - '0') + '0';
        }
    }

    size_t idx = 0;
    for (size_t i = 0; i < bitString.size(); i += step)
    {
        bytes[idx] = (uint8_t)std::stoi(bitString.substr(i, step), 0, 2);
        idx++;
    }

    writeBytesToFile(idx, outSize, outFilename, bytes);
}

void compare(const std::string &inFilename1, const std::string &inFilename2)
{   
    const uint8_t step = 8;
    std::string bitString1, bitString2;
    readFileToBitString(inFilename1, bitString1);
    readFileToBitString(inFilename2, bitString2);

    size_t diffs = abs(bitString1.size() - bitString2.size()) / 4;
    size_t size = std::min(bitString1.size(), bitString2.size());
    
    for(size_t i = 0; i < size; i+=step){
        if(bitString1.substr(i, step).compare(bitString2.substr(i, step)) != 0) {
            diffs++;
        }
    }
    std::cout<<"Number of different 4-bit blocks: " << diffs << '\n';
}
