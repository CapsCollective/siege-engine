//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_OPERATORS_H
#define SIEGE_ENGINE_OPERATORS_H

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

namespace Siege
{
// Vec2

// Addition

DECL_VEC_BINARY_OP_NO_IMP(+=, void, Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(+=, void, Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(+=, void, Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(+=, void, Vec2&, float)

DECL_VEC_BINARY_OP_NO_IMP(+, Vec2, const Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(+, Vec2, const Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(+, Vec2, const Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(+, Vec2, const Vec2&, float)
DECL_VEC_BINARY_OP_NO_IMP(+, Vec2, float, const Vec2&)

// Subtraction

DECL_VEC_BINARY_OP_NO_IMP(-=, void, Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(-=, void, Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(-=, void, Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(-=, void, Vec2&, float)

DECL_VEC_BINARY_OP_NO_IMP(-, Vec2, const Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(-, Vec2, const Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(-, Vec2, const Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(-, Vec2, const Vec2&, float)
DECL_VEC_BINARY_OP_NO_IMP(-, Vec2, float, const Vec2&)

// Multiplication

DECL_VEC_BINARY_OP_NO_IMP(*=, void, Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(*=, void, Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(*=, void, Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(*=, void, Vec2&, float)

DECL_VEC_BINARY_OP_NO_IMP(*, Vec2, const Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(*, Vec2, const Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(*, Vec2, const Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(*, Vec2, const Vec2&, float)
DECL_VEC_BINARY_OP_NO_IMP(*, Vec2, float, const Vec2&)

// Division

DECL_VEC_BINARY_OP_NO_IMP(/=, void, Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(/=, void, Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(/=, void, Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(/=, void, Vec2&, float)

DECL_VEC_BINARY_OP_NO_IMP(/, Vec2, const Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(/, Vec2, const Vec2&, const Vec3&)
DECL_VEC_BINARY_OP_NO_IMP(/, Vec2, const Vec2&, const Vec4&)

DECL_VEC_BINARY_OP_NO_IMP(/, Vec2, const Vec2&, float)
DECL_VEC_BINARY_OP_NO_IMP(/, Vec2, float, const Vec2&)

// Boolean Operators

DECL_VEC_BINARY_OP_NO_IMP(==, bool, const Vec2&, const Vec2&)
DECL_VEC_BINARY_OP_NO_IMP(!=, bool, const Vec2&, const Vec2&)

// Vec3

} // namespace Siege

#endif // SIEGE_ENGINE_OPERATORS_H
