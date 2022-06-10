#include "encoding.hpp"
#include <fstream>
#include <bitset>

void decode(std::string inFilename, std::string outFilename)
{
    std::ifstream input(inFilename, std::ios::binary);
    if (!input)
    {
        std::cerr << "No such file\n";
        exit(1);
    }

    std::cout << "\nDECODING\n";
    BytesVec bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    BytesVec header(bytes.begin(), bytes.begin() + 18);
    BytesVec footer(bytes.end() - 26, bytes.end());
    uint8_t step = 256 / (1 << bytes[18]);
    BytesVec outIdxs;

    idxsExtractor(bytes, outIdxs);
    bytes.clear();
    bytes.shrink_to_fit();

    BitMap outBitMap;
    generateBitMaps(step, outIdxs, outBitMap);
    std::cout << "outBitMap size: " << outBitMap.size() << '\n';

    for (auto pix : outBitMap)
    {
        header.push_back(pix.blue);
        header.push_back(pix.green);
        header.push_back(pix.red);
    }
    header.insert(header.end(), footer.begin(), footer.end());

    std::cout << "NEW BYTES: " << header.size() - 18 - 26 << '\n';

    std::ofstream outFile(outFilename, std::ios::binary | std::ios::trunc);
    outFile.write((const char *)&header[0], header.size());
    outFile.close();
}

void idxsExtractor(BytesVec &bytes, BytesVec &outIdxs)
{
    uint32_t width = (bytes[13] << 8) + bytes[12]; // little endian
    uint32_t height = (bytes[15] << 8) + bytes[14];
    uint32_t numOfidxs = width * height;

    uint8_t qSize = bytes[18]; // just before bitmap
    std::cout << "\nqSize: " << (int)qSize << '\n';
    outIdxs.reserve(numOfidxs);

    std::string bitString = "";

    size_t numOfBits = qSize * numOfidxs * pixelSize;
    uint8_t toAdd = (ByteSize - numOfBits % ByteSize) % ByteSize;

    size_t numOfBytes = numOfBits / ByteSize;
    if (toAdd > 0)
    {
        numOfBytes++;
        numOfBits += toAdd;
    }

    bitString.reserve(numOfBits);

    for (size_t i = 0; i < numOfBytes; i++)
    {
        std::string binString = std::bitset<8>(bytes[i + 19]).to_string();
        bitString.append(binString);
        // std::cout << "NUM: " << (int)bytes[i] << '\n';
        // std::cout << binString << '\n';
    }

    std::cout << "Num of Bytes: " << numOfBytes << '\n';
    std::cout << "Num of bits: " << numOfBits << '\n';

    for (size_t i = 0; i < numOfBits - 3; i += qSize)
    {
        std::string subStr = bitString.substr(i, qSize);
        // int val = std::bitset<ByteSize>(subStr).to_ulong();
        uint8_t val = std::stoi(subStr, 0, 2);
        outIdxs.push_back(val);

        // std::cout<< i << '\n';
        // std::cout<<"VAL: " << (int)val << "\nSTR: "<< subStr << '\n';
    }
}

void generateBitMaps(uint8_t step, BytesVec &outIdxs, BitMap &outBitMap)
{

    std::cout << "STEP: " << (int)step << '\n';
    uint32_t untilZ = outIdxs.size() / 2;
    BitMap r_qBitMap, z_qBitMap;
    r_qBitMap.reserve(untilZ);
    r_qBitMap.reserve(untilZ);

    for (uint32_t i = 0; i < untilZ; i += pixelSize)
    {
        r_qBitMap.emplace_back(
            outIdxs[i] * step,
            outIdxs[i + 1] * step,
            outIdxs[i + 2] * step);
        z_qBitMap.emplace_back(
            outIdxs[i + untilZ] * step,
            outIdxs[i + untilZ + 1] * step,
            outIdxs[i + untilZ + 2] * step);
    }
    /*
    std::cout << "LAST r: " << r_qBitMap.back()
              << "\nLast z: " << z_qBitMap.back() << '\n';
    */
    calculateXBitMap(r_qBitMap, z_qBitMap, outBitMap);
}

void calculateXBitMap(BitMap &r_qBitMap, BitMap &z_qBitMap, BitMap &xBitMap)
{
    BitMap yBitMap;
    yBitMap.reserve(z_qBitMap.size() + 1);
    xBitMap.reserve(z_qBitMap.size() * 2 + 2);

    std::cout << "xBitMap size : " << xBitMap.size() << '\n';
    std::cout << "rSeq size : " << r_qBitMap.size() << '\n';

    yBitMap.push_back(r_qBitMap[0]);
    for (size_t i = 1; i < r_qBitMap.size(); i++)
    {
        yBitMap.push_back(yBitMap[i - 1] + r_qBitMap[i]);
    }

    std::cout << "y size : " << yBitMap.size() << "\nz_qSeq " << z_qBitMap.size() << '\n';
    std::cout << "Sum : " << yBitMap.size() + z_qBitMap.size() << '\n';

    // its zero but its x_1
    // x_2n = y_2n + z_2n
    // x_2n−1 = y_2n − z_2n

    for (size_t i = 0; i < z_qBitMap.size(); i++)
    {
        xBitMap.push_back(yBitMap[i] + z_qBitMap[i]);
        xBitMap.push_back(yBitMap[i] - z_qBitMap[i]);
    }

    std::cout << "New Size : " << xBitMap.size() << '\n';
}
