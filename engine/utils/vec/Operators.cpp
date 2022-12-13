//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Operators.h"

#define DECL_VEC_BINARY_OP_NO_IMP(op, returnType, lhsType, rhsType) \
    returnType operator op(lhsType lhs, rhsType rhs) _SEMICOLON

#define DEFINE_VEC_BINARY_OP_IMP(op, returnType, lhsType, rhsType, ...) \
    returnType operator op(lhsType lhs, rhsType rhs)                    \
    {                                                                   \
        __VA_ARGS__                                                     \
    }

#define VEC2_OPERATOR_BODY_VEC(op, sep) lhs.x op rhs.x sep lhs.y op rhs.y sep

#define VEC2_OPERATOR_BODY_FLOAT(op, sep) lhs.x op rhs sep lhs.y op rhs sep

#define VEC3_OPERATOR_BODY_VEC(op, sep) VEC2_OPERATOR_BODY_VEC(op, sep) lhs.z op rhs.z sep

#define VEC3_OPERATOR_BODY_FLOAT(op, sep) VEC2_OPERATOR_BODY_FLOAT(op, sep) lhs.z op rhs sep

#define VEC4_OPERATOR_BODY_VEC(op, sep) VEC3_OPERATOR_BODY_VEC(op, sep) lhs.w op rhs.w sep

#define VEC4_OPERATOR_BODY_FLOAT(op, sep) VEC3_OPERATOR_BODY_FLOAT(op, sep) lhs.w op rhs sep

namespace Siege
{

// Vec2

// Addition

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec2&, const Vec2&, VEC2_OPERATOR_BODY_VEC(+=, _SEMICOLON))
DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec2&, const Vec3&, VEC2_OPERATOR_BODY_VEC(+=, _SEMICOLON))
DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec2&, const Vec4&, VEC2_OPERATOR_BODY_VEC(+=, _SEMICOLON))

DEFINE_VEC_BINARY_OP_IMP(+=, void, Vec2&, float, VEC2_OPERATOR_BODY_FLOAT(+=, _SEMICOLON))

DEFINE_VEC_BINARY_OP_IMP(+, Vec2, const Vec2&, const Vec2&, return {lhs.x + rhs.x, lhs.y + rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(+, Vec2, const Vec2&, const Vec3&, return {lhs.x + rhs.x, lhs.y + rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(+, Vec2, const Vec2&, const Vec4&, return {lhs.x + rhs.x, lhs.y + rhs.y};)

DEFINE_VEC_BINARY_OP_IMP(+, Vec2, const Vec2&, float, return {lhs.x + rhs, lhs.y + rhs};)
DEFINE_VEC_BINARY_OP_IMP(+, Vec2, float, const Vec2&, return {lhs + rhs.x, lhs + rhs.y};)

// Subtraction

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec2&, const Vec2&, lhs.x -= rhs.x; lhs.y -= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec2&, const Vec3&, lhs.x -= rhs.x; lhs.y -= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec2&, const Vec4&, lhs.x -= rhs.x; lhs.y -= rhs.y;)

DEFINE_VEC_BINARY_OP_IMP(-=, void, Vec2&, float, lhs.x -= rhs; lhs.y -= rhs;)

DEFINE_VEC_BINARY_OP_IMP(-, Vec2, const Vec2&, const Vec2&, return {lhs.x - rhs.x, lhs.y - rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(-, Vec2, const Vec2&, const Vec3&, return {lhs.x - rhs.x, lhs.y - rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(-, Vec2, const Vec2&, const Vec4&, return {lhs.x - rhs.x, lhs.y - rhs.y};)

DEFINE_VEC_BINARY_OP_IMP(-, Vec2, const Vec2&, float, return {lhs.x - rhs, lhs.y - rhs};)
DEFINE_VEC_BINARY_OP_IMP(-, Vec2, float, const Vec2&, return {lhs - rhs.x, lhs - rhs.y};)

// Multiplication

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec2&, const Vec2&, lhs.x *= rhs.x; lhs.y *= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec2&, const Vec3&, lhs.x *= rhs.x; lhs.y *= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec2&, const Vec4&, lhs.x *= rhs.x; lhs.y *= rhs.y;)

DEFINE_VEC_BINARY_OP_IMP(*=, void, Vec2&, float, lhs.x *= rhs; lhs.y *= rhs;)

DEFINE_VEC_BINARY_OP_IMP(*, Vec2, const Vec2&, const Vec2&, return {lhs.x * rhs.x, lhs.y* rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(*, Vec2, const Vec2&, const Vec3&, return {lhs.x * rhs.x, lhs.y* rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(*, Vec2, const Vec2&, const Vec4&, return {lhs.x * rhs.x, lhs.y* rhs.y};)

DEFINE_VEC_BINARY_OP_IMP(*, Vec2, const Vec2&, float, return {lhs.x * rhs, lhs.y* rhs};)
DEFINE_VEC_BINARY_OP_IMP(*, Vec2, float, const Vec2&, return {lhs * rhs.x, lhs* rhs.y};)

// Division

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec2&, const Vec2&, lhs.x /= rhs.x; lhs.y /= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec2&, const Vec3&, lhs.x /= rhs.x; lhs.y /= rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec2&, const Vec4&, lhs.x /= rhs.x; lhs.y /= rhs.y;)

DEFINE_VEC_BINARY_OP_IMP(/=, void, Vec2&, float, lhs.x /= rhs; lhs.y /= rhs;)

DEFINE_VEC_BINARY_OP_IMP(/, Vec2, const Vec2&, const Vec2&, return {lhs.x / rhs.x, lhs.y / rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(/, Vec2, const Vec2&, const Vec3&, return {lhs.x / rhs.x, lhs.y / rhs.y};)
DEFINE_VEC_BINARY_OP_IMP(/, Vec2, const Vec2&, const Vec4&, return {lhs.x / rhs.x, lhs.y / rhs.y};)

DEFINE_VEC_BINARY_OP_IMP(/, Vec2, const Vec2&, float, return {lhs.x / rhs, lhs.y / rhs};)
DEFINE_VEC_BINARY_OP_IMP(/, Vec2, float, const Vec2&, return {lhs / rhs.x, lhs / rhs.y};)

// Boolean Operators

DEFINE_VEC_BINARY_OP_IMP(
    ==, bool, const Vec2&, const Vec2&, return lhs.x == rhs.x && lhs.y == rhs.y;)
DEFINE_VEC_BINARY_OP_IMP(
    !=, bool, const Vec2&, const Vec2&, return lhs.x != rhs.x || lhs.y != rhs.y;)
} // namespace Siege
