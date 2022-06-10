#ifndef _header
#define _header

#include <iostream>
#include <vector>
#include <stdarg.h>

#define DEBUG

#ifdef DEBUG
#define LOG(format, ...) fprintf(stderr, format, __VA_ARGS__)
#else
#define LOG(format, ...)
#endif

#define BytesVec std::vector<uint8_t>

void encode(const std::string &inFilename, const std::string &outFilename);
void noise(const std::string &inFilename, const std::string &outFilename, uint8_t p);
void decode(const std::string &inFilename, const std::string &outFilename);
void compare(const std::string &inFilename1, const std::string &inFilename2);

/* utilities */
void readFileToBitString(const std::string &inFilename, std::string &bitString);
void writeBytesToFile(const size_t actualSize, const size_t outSize,
                      const std::string outFilename, const BytesVec &bytes);

#endif