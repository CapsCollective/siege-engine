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
#include "Hash.h"

// Define macros

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

class Float
{
public:

    static float Lerp(const float& origin, const float& destination, const float& time);

    static constexpr float Epsilon()
    {
        return std::numeric_limits<float>::epsilon();
    }

    static float Normalise(const float& value);

    static float Normalise(const float& value, const float& min, const float& max);

    static float Abs(const float& value);

    static float Max(const float& lhs, const float& rhs);

    static float Min(const float& lhs, const float& rhs);

    static float Clamp(const float& value, const float& min, const float& max);
};

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

    bool operator==(const Vec3& rhs) const
    {
        return x == rhs.x && y == rhs.y && z == rhs.z;
    }

    // Operator overloads

    DEFINE_OPERATOR(+=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(-=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(*=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)
    DEFINE_OPERATOR(/=, Vec3, Vec3, VEC_OPERATOR_BODY_VEC)

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

    static Vec3 Lerp(const Vec3& origin, const Vec3& destination, const float& time)
    {
        return {Float::Lerp(origin.x, destination.x, time),
                Float::Lerp(origin.y, destination.y, time),
                Float::Lerp(origin.z, destination.z, time)};
    }

    static float Dot(const Vec3& vecA, const Vec3& vecB)
    {
        return (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);
    }

    // Public members

    float x;
    float y;
    float z;
};

// Math object hashes
template<>
struct std::hash<Vec3>
{
    size_t operator()(const Vec3& vec) const
    {
        size_t seed = 0;
        Siege::Utils::HashCombine(seed, vec.x, vec.y, vec.z);
        return seed;
    };
};

Vec3 operator*(const float& lhs, const Vec3&);

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
