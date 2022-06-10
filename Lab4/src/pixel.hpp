#ifndef _pixel
#define _pixel

#include <ostream>
#include <algorithm>

struct Pixel
{
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;

    Pixel(){};
    Pixel(uint8_t blue, uint8_t green, uint8_t red) : blue(blue), green(green), red(red){};

    /* operators */
    Pixel &operator=(const Pixel &obj)
    {
        red = obj.red;
        green = obj.green;
        blue = obj.blue;
        return *this;
    }
    friend Pixel operator+(Pixel lhs, const Pixel &rhs)
    {
        lhs.red += rhs.red;
        lhs.green += rhs.green;
        lhs.blue += rhs.blue;
        return lhs;
    }
    friend Pixel operator-(Pixel lhs, const Pixel &rhs)
    {
        lhs.red -= rhs.red;
        lhs.green -= rhs.green;
        lhs.blue -= rhs.blue;
        return lhs;
    }
    friend Pixel operator%(Pixel lhs, const int rhs)
    {
        lhs.red %= rhs;
        lhs.green %= rhs;
        lhs.blue %= rhs;
        return lhs;
    }
    friend Pixel operator/(Pixel lhs, const int rhs)
    {
        lhs.red /= rhs;
        lhs.green /= rhs;
        lhs.blue /= rhs;
        return lhs;
    }
    friend std::ostream &operator<<(std::ostream &os, const Pixel &obj)
    {
        return os << "\033[31m" << int(obj.red)
                  << "\t\033[32m" << int(obj.green)
                  << "\t\033[34m" << int(obj.blue) << "\033[0m";
    }
};
typedef struct Pixel Pixel;

/**
 * Predictors
 */

/**
 * [NW][N]
 * [W][X]
 */

inline uint8_t forColor(uint8_t NW, uint8_t N, uint8_t W)
{
    if (NW >= std::max(W, N))
        return std::max(W, N);
    if (NW <= std::min(W, N))
        return std::min(W, N);
    return W + N - NW;
}

inline Pixel newStandard(Pixel NW, Pixel N, Pixel W)
{
    return Pixel(
        forColor(NW.blue, N.blue, W.blue),
        forColor(NW.green, N.green, W.green),
        forColor(NW.red, N.red, W.red));
}

typedef Pixel (*predictor)(Pixel, Pixel, Pixel);

#define numOfPredictors 8
static const predictor predictors[numOfPredictors] = {
    [](Pixel NW, Pixel N, Pixel W)
    { return W; },
    [](Pixel NW, Pixel N, Pixel W)
    { return N; },
    [](Pixel NW, Pixel N, Pixel W)
    { return NW; },
    [](Pixel NW, Pixel N, Pixel W)
    { return N + W - NW; },
    [](Pixel NW, Pixel N, Pixel W)
    { return N + (W - NW) / 2; },
    [](Pixel NW, Pixel N, Pixel W)
    { return W + (N - NW) / 2; },
    [](Pixel NW, Pixel N, Pixel W)
    { return (N + W) / 2; },
    [](Pixel NW, Pixel N, Pixel W)
    { return newStandard(NW, N, W); } // this make no sens bcs inline is too long and have multiple rets
};

#endif