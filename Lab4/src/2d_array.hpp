#ifndef _2d_array
#define _2d_array

/* TODO: reimplement this to work on 1 dimension by calculating indexes */
#include <iostream>

template <typename t>
struct array_2d
{
    size_t height = 0;
    size_t width = 0;
    struct array_1d
    {
        t *array;
        array_1d(void) { array = 0; }
        ~array_1d()
        {
            if (array)
            {
                delete[] array;
                array = 0;
            }
        }
        t &operator[](size_t index) { return array[index]; }
    } * array;
    array_2d(void) { array = 0; }
    array_2d(array_2d<t> *a)
    {
        array = a->array;
        a->array = 0;
    }
    void init(size_t a, size_t b)
    {
        if (array)
        {
            delete[] array;
            array = 0;
        }
        height = a;
        width = b;
        array = new array_1d[a];
        for (size_t i = 0; i < a; i++)
        {
            array[i].array = new t[b];
        }
    }
    ~array_2d()
    {
        if (array)
        {
            delete[] array;
            array = 0;
        }
    }
    array_1d &operator[](size_t index) { return array[index]; }
};

#endif