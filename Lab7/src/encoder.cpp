#include "header.hpp"
#include <algorithm>
#include <unordered_map>

inline const std::string toHamming(const std::string &bits);
const std::unordered_map<std::string, std::string> HammingTable{
    {"1110", "00101101"},
    {"1101", "10101010"},
    {"0000", "00000000"},
    {"0001", "11010010"},
    {"0010", "01010101"},
    {"0100", "10011001"},
    {"1011", "01100110"},
    {"1111", "11111111"},
    {"0101", "01001011"},
    {"0110", "11001100"},
    {"1100", "01111000"},
    {"0111", "00011110"},
    {"0011", "10000111"},
    {"1000", "11100001"},
    {"1001", "00110011"},
    {"1010", "10110100"}};

void encode(const std::string &inFilename, const std::string &outFilename)
{
    std::string bitString;
    readFileToBitString(inFilename, bitString);
    const size_t outSize = bitString.size() * 2 / 8;
    BytesVec bytes(outSize);
    LOG("%s %ld\n%s %ld\n", "Encode:\nBitstring size:", bitString.size(), "bytes size:", outSize);

    const static uint8_t step = 4;
    size_t idx = 0;
    for (size_t i = 0; i < bitString.size(); i += step)
    {
        // bytes[idx] = (uint8_t)std::stoi(toHamming(bitString.substr(i, step)), 0, 2);
        bytes[idx] = (uint8_t)std::stoi(HammingTable.at(bitString.substr(i, step)), 0, 2);
        idx++;
    }

    writeBytesToFile(idx, outSize, outFilename, bytes);
}

const auto calcParity = [](const std::string &bits, const std::vector<uint8_t> &idxs)
{
    char result = 0;
    for (auto idx : idxs)
        result += (bits[idx] == '1' ? 1 : 0);

    return result % 2 + '0';
};

inline const std::string toHamming(const std::string &bits)
{
    char p1 = calcParity(bits, {0, 1, 3});
    char p2 = calcParity(bits, {0, 2, 3});
    char p3 = calcParity(bits, {1, 2, 3});

    std::string codeWord = "";
    codeWord.push_back(p1);
    codeWord.push_back(p2);
    codeWord.push_back(bits[0]);
    codeWord.push_back(p3);
    codeWord.append(bits.begin() + 1, bits.end());

    char parity = calcParity(codeWord, {0, 1, 2, 3, 4, 5, 6, 7});
    codeWord.push_back(parity);

    return codeWord;
}

/* 
functions to generate const HashTable above
void getAllKLengthStrsRec(char set[], std::string prefix,
                          int n, int k, std::vector<std::string> &outVec)
{
    if (k == 0)
    {
        outVec.push_back(prefix);
        return;
    }

    for (int i = 0; i < n; i++)
    {
        std::string newPrefix;
        newPrefix = prefix + set[i];
        getAllKLengthStrsRec(set, newPrefix, n, k - 1, outVec);
    }
}

void getHashMap()
{
    char set[] = {'0', '1'};
    int k = 4, n = 2;
    std::vector<std::string> outVec;
    std::unordered_map<std::string, std::string> hashMap;
    getAllKLengthStrsRec(set, "", n, k, outVec);

    for (auto str : outVec)
        hashMap.emplace(std::make_pair(str, toHamming(str)));

    std::cout << "{\n";

    std::for_each(hashMap.begin(), hashMap.end(),
                  [](std::pair<std::string, std::string> key_value)
                  {
                      std::string str = key_value.first;
                      std::string code = key_value.second;
                      std::cout << "{"
                                << "\"" << str << "\", "
                                << "\"" << code << "\" }, \n";
                  });

    std::cout << "}\n";
}
*/