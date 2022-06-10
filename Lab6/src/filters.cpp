#include "encoding.hpp"

void filters(BitMap &bitMap, BitMap &yBitMap, BitMap &zBitMap)
{
    yBitMap.clear();
    yBitMap.reserve(bitMap.size() / 2);

    yBitMap.emplace_back(0, 0, 0);
    zBitMap.push_back(bitMap[0]);

    for (size_t n = 2; n < bitMap.size(); n += 2)
    {
        Pixel xn = bitMap[n];
        Pixel yn = (xn + bitMap[n - 1]) / 2;
        yBitMap.push_back(yn);
        zBitMap.push_back(xn - yn);
    }
}

inline void quantizeVal(uint8_t step, BitMap &inBitMap, BitMap &outValBitMap)
{
    outValBitMap.clear();
    outValBitMap.reserve(inBitMap.size());

    for (auto pixel : inBitMap)
        outValBitMap.push_back((pixel / step) * step);
}

void diffCode(uint8_t step, BitMap &yBitMap, BitMap &rBitMap)
{
    BitMap y_qSequence;
    quantizeVal(step, yBitMap, y_qSequence);

    rBitMap.emplace_back(0, 0, 0);

    for (size_t i = 1; i < y_qSequence.size(); i++)
        rBitMap.push_back(yBitMap[i] - y_qSequence[i - 1]);
}
