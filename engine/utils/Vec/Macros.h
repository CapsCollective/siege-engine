//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib

#ifndef SIEGE_ENGINE_VEC_MACROS_H
#define SIEGE_ENGINE_VEC_MACROS_H

#include "../Macros.h"

// Define macros

#define DEFINE_OPERATOR_NO_IMP(op, lhsType, rhsType) lhsType& operator op(const rhsType& rhs);

#define DEFINE_CONST_OPERATOR_NO_IMP(op, lhsType, rhsType) \
    lhsType operator op(const rhsType& rhs) const;

#define DEFINE_OPERATOR_IMP(op, lhsType, rhsType, body) \
    lhsType& lhsType::operator op(const rhsType& rhs)   \
    {                                                   \
        body(op, _SEMICOLON) return *this;              \
    }

#define DEFINE_CONST_OPERATOR_IMP(op, lhsType, rhsType, body) \
    lhsType lhsType::operator op(const rhsType& rhs) const    \
    {                                                         \
        return {body(op, _COMMA)};                            \
    }

#define DEFINE_OPERATOR(op, lhsType, rhsType, body) \
    lhsType& operator op(const rhsType& rhs)        \
    {                                               \
        body(op, _SEMICOLON) return *this;          \
    }

#define DEFINE_CONST_OPERATOR(op, lhsType, rhsType, body) \
    lhsType operator op(const rhsType& rhs) const         \
    {                                                     \
        return {body(op, _COMMA)};                        \
    }

#endif // SIEGE_ENGINE_VEC_MACROS_H
