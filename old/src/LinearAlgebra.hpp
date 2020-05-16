#ifndef PHOTIC_LINEAR_ALGEBRA_HPP
#define PHOTIC_LINEAR_ALGEBRA_HPP

#include "Types.hpp"

#define ELEM_COUNT T_Rows * T_Cols

template <typename T, Dim_t T_Rows, Dim_t T_Cols>
struct Matrix
{
    T mData[ELEM_COUNT];
    union
    {
        T mData[ELEM_COUNT];
    };

};