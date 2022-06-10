#include "header.hpp"

#define numOfCodes 16
const std::string hammingCodes[numOfCodes] = {
    "00000000",
    "11010010",
    "01010101",
    "10000111",
    "10011001",
    "01001011",
    "11001100",
    "00011110",
    "11100001",
    "00110011",
    "10110100",
    "01100110",
    "01111000",
    "10101010",
    "00101101",
    "11111111",
};

inline const std::string fromHamming(const std::string &byte);

void decode(const std::string &inFilename, const std::string &outFilename)
{
    const static size_t step = 8;
    std::string bitString, decodedBitString = "";
    readFileToBitString(inFilename, bitString);
    const size_t decodedBitStringSize = bitString.size() / 2;
    const size_t outSize = decodedBitStringSize / step;
    decodedBitString.reserve(decodedBitStringSize);
    BytesVec bytes(outSize);
    size_t withMoreErrs = 0;

    LOG("%s %ld\n%s %ld\n", "Decode:\nBitstring size:", bitString.size(), "resByte size:", outSize);

    for (size_t i = 0; i < bitString.size(); i += step)
    {
        const std::string codeWord = fromHamming(bitString.substr(i, step));

        if (codeWord.empty())
        {   
            withMoreErrs++;
            decodedBitString.append("0000");
        }
        else
        {
            decodedBitString.append(codeWord);
        }
    }

    size_t idx = 0;
    for (size_t i = 0; i < decodedBitString.size(); i += step)
    {
        bytes[idx] = (uint8_t)std::stoi(decodedBitString.substr(i, step), 0, 2);
        idx++;
    }

    writeBytesToFile(idx, outSize, outFilename, bytes);

    std::cout<<"Blocks with >= 2 errors: " << withMoreErrs << '\n';
}

inline const std::string fromHamming(const std::string &byte)
{
    std::string toRet = "";
    for (uint8_t i = 0; i < numOfCodes; i++)
    {
        uint8_t errors = 0;
        std::string codeWord = hammingCodes[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            if (byte[j] != codeWord[j])
                errors++;
        }

        if (errors == 0)
        {
            toRet.push_back(byte[2]);
            toRet.push_back(byte[4]);
            toRet.push_back(byte[5]);
            toRet.push_back(byte[6]);
            return toRet;
        }

        if (errors == 1)
        {
            toRet.push_back(codeWord[2]);
            toRet.push_back(codeWord[4]);
            toRet.push_back(codeWord[5]);
            toRet.push_back(codeWord[6]);
            return toRet;
        }
    }
    return toRet;
}
