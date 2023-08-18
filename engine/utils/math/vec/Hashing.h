//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_VEC_HASHING_H
#define SIEGE_ENGINE_UTILS_MATH_VEC_HASHING_H

#include "../../Hash.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace std
{
template<typename T>
struct hash<Siege::Vec<T, 3>>
{
    size_t operator()(const Siege::Vec<T, 3>& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y, vertex.z);
        return seed;
    };
};

template<typename T>
struct hash<Siege::Vec<T, 2>>
{
    size_t operator()(const Siege::Vec<T, 2>& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y);
        return seed;
    };
};

template<typename T>
struct hash<Siege::Vec<T, 4>>
{
    size_t operator()(const Siege::Vec<T, 4>& vertex) const
    {
        size_t seed = 0;
        Siege::Hash::HashCombine(seed, vertex.x, vertex.y, vertex.z, vertex.w);
        return seed;
    };
};
} // namespace std

#endif // SIEGE_ENGINE_UTILS_MATH_VEC_HASHING_H
