#ifndef _filter
#define _filter

#include "tga.hpp"

/* STATS */
void stats(std::string originalFilename, std::string editedFilename);
uint32_t *calcErrors(BitMap &original, BitMap &edited);
void displayStats(uint32_t *errors, BitMap &original);

/* FILTERS */
void filters(BitMap &bitMap, BitMap &yBitMap, BitMap &zBitMap);
inline void quantizeVal(uint8_t step, BitMap &inBitMap, BitMap &outValBitMap);
void diffCode(uint8_t step, BitMap &yBitMap, BitMap &rBitMap);

/* BYTES ENCODING */
void encode(std::string inFileName, std::string outFilename, uint8_t qSize);
void toBytes(uint8_t qSize, BitMap &rIdxs, BitMap &zIdxs, BytesVec &outBytes);
inline void quantizeIndex(uint8_t step, BitMap &inBitMap, BitMap &outIdxBitMap);

/* BYTES DECODING */
void decode(std::string inFilename, std::string outFilename);
void idxsExtractor(BytesVec &bytes, BytesVec &outIdxs);
void generateBitMaps(uint8_t step, BytesVec &outIdxs, BitMap &outBitMap);
void calculateXBitMap(BitMap &r_qBitMap, BitMap &z_qBitMap, BitMap &xBitMap);

#endif