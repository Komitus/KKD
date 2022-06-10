#include "header.hpp"
#include <fstream>
#include <cassert>

inline const std::string byteToBinStr(uint8_t a)
{
    std::string binary = "";
    uint8_t mask = 1;
    for (uint8_t i = 0; i < 8 * sizeof(uint8_t); i++)
    {
        if ((mask & a) >= 1)
            binary = "1" + binary;
        else
            binary = "0" + binary;
        mask <<= 1;
    }

    return binary;
}

void readFileToBitString(const std::string &inFilename, std::string &bitString)
{
    std::ifstream input(inFilename, std::ios::binary);
    if (!input)
    {
        std::cerr << "No such file\n";
        exit(1);
    }

    BytesVec bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();
    LOG("%s %ld %s", "inFile size:", bytes.size(), "in bytes\n");

    bitString.reserve(bytes.size() * 8);

    for (auto byte : bytes)
        bitString.append(byteToBinStr(byte));
}

void writeBytesToFile(const size_t actualSize, const size_t outSize,
                      const std::string outFilename, const BytesVec &bytes)
{
    assert(actualSize == outSize);
    std::ofstream outFile(outFilename, std::ios::binary);
    outFile.write((const char *)&bytes[0], outSize);
    outFile.close();
}
