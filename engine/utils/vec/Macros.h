//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#ifndef SIEGE_ENGINE_VEC_MACROS_H
#define SIEGE_ENGINE_VEC_MACROS_H

#include "../Macros.h"

namespace Siege
{
typedef struct Vec2 Vec2;
typedef struct Vec3 Vec3;
typedef struct Vec4 Vec4;
} // namespace Siege

// Define macros

#define DECL_VEC_BINARY_OP_NO_IMP(op, returnType, lhsType, rhsType) \
    returnType operator op(lhsType lhs, rhsType rhs) _SEMICOLON

#define DEFINE_VEC_BINARY_OP_IMP(op, returnType, lhsType, rhsType, ...) \
    returnType operator op(lhsType lhs, rhsType rhs)                    \
    {                                                                   \
        __VA_ARGS__                                                     \
    }

#define VEC2_OPERATOR_BODY_VEC(op, sep) lhs.x op rhs.x sep lhs.y op rhs.y sep

#define VEC2_OPERATOR_BODY_FLOAT(op, sep) lhs.x op rhs sep lhs.y op rhs sep

#endif // SIEGE_ENGINE_VEC_MACROS_H
