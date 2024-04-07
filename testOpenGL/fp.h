#pragma once

#include <cmath> // sin, cos
#include <cstdint>
#include <initializer_list>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <format>

#include "vector.h"

template <template <typename T, size_t N, size_t Alignment> class vec, typename T, size_t N, size_t Alignment>
struct functor1
{

};

// partial specialization on size_t N
template <template <typename T, size_t N, size_t Alignment> class vec, typename T, size_t Alignment>
struct functor1<vec, T, 1, Alignment>
{
    inline static vec<T, 1, Alignment> call(T (*Func)(T x), vec<T, 1, Alignment> const &v)
    {
        return vec<T, 1, Alignment>(Func(v[COMPONENT::X]));
    }
};

template <template <typename T, size_t N, size_t Alignment> class vec, size_t Alignment, typename T>
struct functor1<vec, T, 2, Alignment>
{
    inline constexpr static vec<T, 2, Alignment> call(T (*Func)(T x), vec<T, 2, Alignment> const &v)
    {
        return vec<T, 2, Alignment>(Func(v[COMPONENT::X]), Func(v[COMPONENT::Y]));
    }
};

template <template <typename T, size_t N, size_t Alignment> class vec, typename T>
struct functor1<vec, T, 3, sizeof(T) * 4>
{
    inline constexpr static vec<T, 3, sizeof(T) * 4> call(T (*Func)(T x), vec<T, 3, sizeof(T) * 4> const &v)
    {
        return vec<T, 3, sizeof(T) * 4>(std::array<T, 3>{Func(v[COMPONENT::X]), Func(v[COMPONENT::Y]), Func(v[COMPONENT::Z])});
    }
};