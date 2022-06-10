#ifndef _TYPES
#define _TYPES

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <map>
#include <bitset>
#include <algorithm>

struct Stats{
    size_t originalSize;
    size_t compressedSize;
    double originalEntropy;
    double compressedEntropy;
};

double calculateEntropy(const std::vector<uint8_t> &bytes);
void writeBoolsToFile(const std::string outFilename, std::vector<bool> &bitString, struct Stats &stats, bool paddingByte);
void printStats(struct Stats &stats);
void compress(const std::string inFilename, const std::string outFilename, const char *encoding);
void decompress(const std::string inFilename, const std::string outFilename, const char *encoding);

void lzwEncode(const std::vector<uint8_t> &bytes, std::vector<uint32_t> &result);
void lzwDecode(const std::vector<uint32_t> &codes, std::string &result);

void eliasGammaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector);
void eliasDeltaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector);
void eliasOmegaEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector);

void eliasGammaDecode(std::string &inBits, std::vector<uint32_t> &codes);
void eliasDeltaDecode(std::string &inBits, std::vector<uint32_t> &codes);
void eliasOmegaDecode(std::string &inBits, std::vector<uint32_t> &codes);

void fibonacciEncode(const std::vector<uint32_t> &codes, std::vector<bool> &outVector);
std::string fibonacciSymbolEncode(uint32_t n, const std::vector<uint32_t> &fib);

void fibonacciDecode(std::string &inBits, std::vector<uint32_t> &codes);

#endif