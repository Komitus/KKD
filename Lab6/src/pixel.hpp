#ifndef _pixel
#define _pixel

#include <iostream>

struct Pixel
{
    uint8_t blue = 0;
    uint8_t green = 0;
    uint8_t red = 0;

    Pixel(){};
    Pixel(uint8_t blue, uint8_t green, uint8_t red) : blue(blue), green(green), red(red){};

    /* operators */

    uint8_t &operator[](int idx){
        if(idx == 1){
            return green;
        } else if(idx == 2) {
            return red;
        } else {
            return blue;
        }
    }

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
    friend Pixel operator*(Pixel lhs, const int rhs)
    {
        lhs.red *= rhs;
        lhs.green *= rhs;
        lhs.blue *= rhs;
        return lhs;
    }
    friend Pixel operator%(Pixel lhs, const int rhs)
    {
        lhs.red = (rhs + (lhs.red % rhs)) % rhs;
        lhs.green = (rhs + (lhs.green % rhs)) % rhs;
        lhs.blue = (rhs + (lhs.blue % rhs)) % rhs;
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

#endif