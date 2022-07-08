//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MATHS_H
#define SIEGE_ENGINE_MATHS_H

#include <cmath>

#include "Macros.h"
#include "String.h"

// Define macros

#define DEFINE_OPERATOR_NO_IMP(op, lhsType, rhsType) \
   lhsType& operator op(const rhsType& rhs);

#define DEFINE_CONST_OPERATOR_NO_IMP(op, lhsType, rhsType) \
   lhsType operator op(const rhsType& rhs) const;

#define DEFINE_OPERATOR_IMP(op, lhsType, rhsType, body) \
    lhsType& lhsType::operator op(const rhsType& rhs) \
    {                                               \
        body(op, _SEMICOLON) return *this;          \
    }

#define DEFINE_CONST_OPERATOR_IMP(op, lhsType, rhsType, body) \
    lhsType lhsType::operator op(const rhsType& rhs) const         \
    {                                                     \
        return {body(op, _COMMA)};                        \
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

#define VEC_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep z op rhs.z sep

#define VEC_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep z op rhs sep

#define VEC2_OPERATOR_BODY_VEC(op, sep) x op rhs.x sep y op rhs.y sep

#define VEC2_OPERATOR_BODY_FLOAT(op, sep) x op rhs sep y op rhs sep

typedef struct Vec2 Vec2;
typedef struct Vec3 Vec3;

// ---------------------------------------- Vec2 --------------------------------------------------

struct Vec2
{
    // Public constants

    static const Vec2 Zero;
    static const Vec2 One;
    static const Vec2 Up;

    // 'Structors

    Vec2() : Vec2(0.f, 0.f) {}
    Vec2(const float& x, const float& y) : x(x), y(y) {}
    Vec2(const Vec3& other);

    // Operator overloads

    Vec2& operator=(const Vec2& rhs) = default;

    DEFINE_OPERATOR(+=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    DEFINE_OPERATOR_NO_IMP(+=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(-=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(*=, Vec2, Vec3)
    DEFINE_OPERATOR_NO_IMP(/=, Vec2, Vec3)

    bool operator==(const Vec2& other);
    bool operator!=(const Vec2& other);

    DEFINE_OPERATOR(+=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(-=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(*=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(/=, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    DEFINE_CONST_OPERATOR(+, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(-, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(*, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(/, Vec2, Vec2, VEC2_OPERATOR_BODY_VEC)

    DEFINE_CONST_OPERATOR_NO_IMP(+, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(-, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(*, Vec2, Vec3)
    DEFINE_CONST_OPERATOR_NO_IMP(/, Vec2, Vec3)

    DEFINE_CONST_OPERATOR(+, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(-, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(*, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(/, Vec2, float, VEC2_OPERATOR_BODY_FLOAT)

    // Static methods

    static bool FromString(OUT Vec2& vec, const String& string);

    // Public methods

    String ToString() const;

    float Length() const
    {
        return sqrtf(x * x + y * y);
    }

    Vec2 XComp() const
    {
        return {x, 0.f,};
    }

    Vec2 YComp() const
    {
        return {0.f, y};
    }

    float x, y;
};

// ---------------------------------------- Vec3 --------------------------------------------------

struct Vec3
{
    // Public constants

    static const Vec3 Zero;

    static const Vec3 One;

    static const Vec3 Up;

    // 'Structors

    Vec3() : Vec3(0.f, 0.f, 0.f) {}

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3& operator=(const Vec3& rhs) = default;

    // Operator overloads

    DEFINE_OPERATOR(+=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    bool operator==(const Vec3& other);

    DEFINE_OPERATOR(+=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(-=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(*=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_OPERATOR(/=, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    DEFINE_CONST_OPERATOR(+, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(-, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(*, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_CONST_OPERATOR(/, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

    DEFINE_CONST_OPERATOR(+, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(-, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(*, Vec3, float, VEC_OPERATOR_BODY_FLOAT)
    DEFINE_CONST_OPERATOR(/, Vec3, float, VEC_OPERATOR_BODY_FLOAT)

    // Static methods

    static bool FromString(OUT Vec3& vec, const String& string);

    // Public methods

    String ToString() const;

    Vec3 Normalise() const;

    float Length() const
    {
        return sqrtf(x * x + y * y + z * z);
    }

    Vec3 XComp() const
    {
        return {x, 0.f, 0.f};
    }

    Vec3 YComp() const
    {
        return {0.f, y, 0.f};
    }

    Vec3 ZComp() const
    {
        return {0.f, 0.f, z};
    }

    // Public members

    float x;
    float y;
    float z;
};

struct Xform
{
public:

    Xform() : Xform(Vec3::Zero) {}

    explicit Xform(Vec3 position) : Xform(position, 0.f) {}

    Xform(Vec3 position, float rotation) : Xform(position, rotation, Vec3::One) {}

    Xform(Vec3 position, float rotation, Vec3 scale) :
        position(position),
        rotation(rotation),
        scale(scale)
    {}

    const Vec3& GetPosition() const
    {
        return position;
    }

    float GetRotation() const
    {
        return rotation;
    }

    const Vec3& GetScale() const
    {
        return scale;
    }

    void SetPosition(const Vec3& newPosition)
    {
        position = newPosition;
    }

    void SetRotation(float newRotation)
    {
        rotation = fmod(newRotation, 360.f);
    }

    void SetScale(const Vec3& newScale)
    {
        scale = newScale;
    }

private:

    Vec3 position;
    float rotation;
    Vec3 scale;
};

struct BoundedBox
{
    // 'Structors

    BoundedBox() : BoundedBox(Vec3(), Vec3()) {}

    BoundedBox(Vec3 min, Vec3 max) : min(min), max(max) {}

    // Public methods

    bool Intersects(const BoundedBox& other) const;

    bool Intersects(const struct RayCast& ray) const;

    // Public members

    Vec3 min;
    Vec3 max;
};

struct RayCast
{
    // 'Structors

    RayCast() : RayCast(Vec3(), Vec3()) {}

    RayCast(Vec3 position, Vec3 direction) : position(position), direction(direction) {}

    // Public members

    Vec3 position;
    Vec3 direction;
};

#endif // SIEGE_ENGINE_MATHS_H
