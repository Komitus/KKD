#include "header.hpp"
#include <assert.h>
#include <cstring>

double calculateEntropy(const std::vector<uint8_t> &bytes)
{
    double entropy;

    if (bytes.size() == 0)
    {
        std::cerr << "Length of file == 0\n";
        exit(1);
    }

    uint32_t occurrences[256] = {{0}};

    for (auto it = bytes.begin(); it != bytes.end(); ++it)
    {
        occurrences[*it]++;
    }

    entropy = 0.0;
    double cbLog;
    double sizeLog = log2(bytes.size());

    for (uint16_t currByte = 0; currByte < 256; currByte++)
    {
        if (occurrences[currByte] != 0)
        {
            cbLog = log2(occurrences[currByte]);
            entropy += (sizeLog - cbLog) * occurrences[currByte];
        }
    }
    entropy /= (double)bytes.size();

    return entropy;
}

void printStats(struct Stats &stats)
{
    std::cout << "Size of original file (in bytes): " << stats.originalSize << '\n';
    std::cout << "Size of compressed file (in bytes): " << stats.compressedSize << '\n';
    std::cout << "##################################################################\n";
    std::cout << "# Entropy of original: " << stats.originalEntropy << '\n';
    std::cout << "# Entropy of compressed: " << stats.compressedEntropy << '\n';
    std::cout << "# Compression ratio (original / compressed): " << (double)stats.originalSize / (double)stats.compressedSize << '\n';
}

void compress(const std::string inFilename, const std::string outFilename, const char *encoding)
{
    struct Stats stats;
    bool paddingByte = false;
    std::ifstream input(inFilename, std::ios::binary);

    std::vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    std::vector<uint32_t> codes;
    std::vector<bool> encoded;

    lzwEncode(bytes, codes);

    stats.originalSize = bytes.size();
    stats.originalEntropy = calculateEntropy(bytes);

    encoded.reserve(codes.size() * 2);

    if (strcmp(encoding, "delta") == 0)
    {
        eliasDeltaEncode(codes, encoded);
    }
    else if (strcmp(encoding, "omega") == 0)
    {
        eliasOmegaEncode(codes, encoded);
        paddingByte = true;
    }
    else if (strcmp(encoding, "fib") == 0)
    {
        fibonacciEncode(codes, encoded);
    }
    else
    {
        eliasGammaEncode(codes, encoded);
    }

    codes.clear();
    codes.shrink_to_fit();

    writeBoolsToFile(outFilename, encoded, stats, paddingByte);
    printStats(stats);

    encoded.clear();
    encoded.shrink_to_fit();
}

void decompress(const std::string inFilename, const std::string outFilename, const char *encoding)
{
    std::ifstream input(inFilename, std::ios::binary);

    std::vector<uint8_t> bytes(
        (std::istreambuf_iterator<char>(input)),
        (std::istreambuf_iterator<char>()));
    input.close();

    size_t bytesSize = bytes.size();

    std::vector<uint32_t> codes;
    std::string encoded;
    std::string result;

    encoded.reserve(bytesSize);

    uint8_t toRemove = 0;

    if (strcmp(encoding, "omega") == 0){
        std::cout<<"\033[0;33mI am removing one byte that informs about padded zero bits\033[0m\n";
        toRemove = bytes[bytesSize - 1];
        bytes.pop_back();
    }
 
    for (auto it = bytes.begin(); it != bytes.end(); it++)
    {
        // std::cout<<static_cast<int>(*it) << ": " << std::bitset<8>(*it).to_string() << '\n';
        encoded.append(std::bitset<8>(*it).to_string());
    }

    for(uint8_t i = 0; i < toRemove; i++){
        encoded.pop_back();
    }

    bytes.clear();
    bytes.shrink_to_fit();

    codes.reserve(bytesSize);

    if (strcmp(encoding, "delta") == 0)
    {
        eliasDeltaDecode(encoded, codes);
    }
    else if (strcmp(encoding, "omega") == 0)
    {
        eliasOmegaDecode(encoded, codes);
    }
    else if (strcmp(encoding, "fib") == 0)
    {
        fibonacciDecode(encoded, codes);
    }
    else
    {
        eliasGammaDecode(encoded, codes);
    }

    lzwDecode(codes, result);

    std::ofstream outFile(outFilename, std::ios::binary);
    outFile << result;
}

void writeBoolsToFile(const std::string outFilename, std::vector<bool> &bitString, struct Stats &stats, bool paddingByte)
{
    std::vector<uint8_t> bytes;
    bytes.reserve(bitString.size() / 8);

    uint8_t ret = 0;
    uint8_t tmp;
    size_t counter = 0;

    uint8_t toAdd = (8 - bitString.size() % 8) % 8;

    for (uint8_t i = 0; i < toAdd; i++)
    {
        bitString.push_back(0);
    }

    for (size_t i = 0; i < bitString.size(); i += 8)
    {
        for (uint8_t j = 0; j < 8; j++)
        {
            tmp = bitString.at(i + j);
            // std::cout<<static_cast<int>(tmp);
            ret |= tmp << (8 - j - 1);
        }
        // std::cout<<" ret: "<<static_cast<int>(ret)<<'\n';
        bytes.push_back(ret);
        counter++;
        ret = 0;
    }

    assert(bitString.size() / 8 == counter);

    if(paddingByte){
        std::cout<<"\033[0;33mI am adding one byte that informs about padded zero bits\033[0m\n";
        bytes.push_back(toAdd);
    }
    stats.compressedSize = bytes.size(); //bcs last informs about padding
    stats.compressedEntropy = calculateEntropy(bytes);

    bitString.clear();
    bitString.shrink_to_fit();

    std::ofstream outFile(outFilename, std::ios::binary);
    outFile.write((const char *)&bytes[0], bytes.size());
    outFile.close();

    bytes.clear();
    bytes.shrink_to_fit();
}
