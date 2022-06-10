#include "encoding.hpp"
#include <bitset>

void encode(std::string inFileName, std::string outFilename, uint8_t qSize)
{
    std::cout << "\nENCODING: \n";
    TGA img(inFileName);

    BytesVec outBytes;
    BitMap yBitMap, zBitMap, rBitMap, rIdxs, zIdxs;

    uint8_t step = 256 / (1 << qSize);

    filters(img.bitMap, yBitMap, zBitMap);
    diffCode(step, yBitMap, rBitMap);

    quantizeIndex(step, rBitMap, rIdxs);
    quantizeIndex(step, zBitMap, zIdxs);

    yBitMap.clear();
    yBitMap.shrink_to_fit();
    zBitMap.clear();
    zBitMap.shrink_to_fit();
    rBitMap.clear();
    rBitMap.shrink_to_fit();

    /*
    std::cout << "LAST r: " << rIdxs.back() * step
              << "\nLast z: " << zIdxs.back() * step << '\n';
    */
    toBytes(qSize, rIdxs, zIdxs, outBytes);

    img.newImage(outBytes, outFilename);
}

inline std::string decToBinStr(int a, uint8_t size)
{
    std::string binary = "";
    int mask = 1;
    for (int i = 0; i < size; i++)
    {
        if ((mask & a) >= 1)
            binary = "1" + binary;
        else
            binary = "0" + binary;
        mask <<= 1;
    }

    return binary;
}

void toBytes(uint8_t qSize, BitMap &rIdxs, BitMap &zIdxs, BytesVec &outBytes)
{
    size_t numOfBits = qSize * (rIdxs.size() + zIdxs.size()) * pixelSize;

    uint8_t toAdd = (ByteSize - numOfBits % ByteSize) % ByteSize;
    size_t toReserve = numOfBits / ByteSize + 2; // toAdd and info about qSize
    outBytes.reserve(toReserve + 1);

    outBytes.push_back(qSize);

    std::string bitString = "";

    for (auto rIdx : rIdxs)
    {
        bitString.append(decToBinStr(rIdx.blue, qSize));
        bitString.append(decToBinStr(rIdx.green, qSize));
        bitString.append(decToBinStr(rIdx.red, qSize));
    }

    for (auto zIdx : zIdxs)
    {
        bitString.append(decToBinStr(zIdx.blue, qSize));
        bitString.append(decToBinStr(zIdx.green, qSize));
        bitString.append(decToBinStr(zIdx.red, qSize));
    }

    for (uint8_t i = 0; i < toAdd; i++)
    {
        bitString.push_back('0');
    }

    for (size_t i = 0; i < bitString.size(); i += ByteSize)
    {
        std::string subStr = bitString.substr(i, ByteSize);
        std::bitset<ByteSize> b(subStr);
        uint8_t c = (b.to_ulong() & 0xFF);
        outBytes.push_back(c);

        /*
        std::cout << "NUMB: " << static_cast<int>(c) << '\n';
        for (uint8_t j = 0; j < ByteSize; j++)
            std::cout << (int)subStr[j] - '0';
        std::cout << '\n';
        */
    }

    std::cout << "\nRsize : " << rIdxs.size()
              << "\nZSize: " << zIdxs.size()
              << "\nzeros was added: " << (int)toAdd
              << "\nNum of Bytes: " << toReserve - 2
              << "\nNum of bits: " << numOfBits
              << "\nDivided by 8: " << (double)numOfBits / (double)ByteSize << '\n';
}

inline void quantizeIndex(uint8_t step, BitMap &inBitMap, BitMap &outIdxBitMap)
{
    outIdxBitMap.clear();
    outIdxBitMap.reserve(inBitMap.size());
    // std::cout << "step : " << (int)step << '\n';
    for (auto pixel : inBitMap)
        outIdxBitMap.push_back(pixel / step);
}