#ifndef _lbg
#define _lbg

#include "tga.hpp"
#include <cmath>

double MSE(const PixelVec &bitMap1, const PixelVec &bitMap2);
double SNR(const PixelVec &bitMap, double mse);
void quantify(PixelVec &outBitmap, const PixelVec &inBitmap, uint32_t colors);
CodeBook *generate(const PixelVec &data, size_t desiredSize, double eps);

CodeBook *_split(const PixelVec &data, CodeBook *codeBook, double eps, double &initialAvgDist);
Pixel _avgPixOfPixs(const PixelVec &pixels);
Pixel _newCodePixel(Pixel c, double e);
double _avgDistortionC0(Pixel c0, const PixelVec &data);
double _avgDistortionCList(const PixelVec &c_list, const PixelVec &data);
double euclidSquared(Pixel px1, Pixel px2);


#endif