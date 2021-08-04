#ifndef A_DARK_DISCOMFORT_MATHS_H
#define A_DARK_DISCOMFORT_MATHS_H

#include "Macros.h"
#include <string>
#include <raylib/Vector3.hpp>

// Define macros

#define DEFINE_OPERATOR(op, lhsType, rhsType, body) \
    lhsType& operator op(const rhsType& rhs) \
    { \
        body(op, SEMICOLON) \
        return *this; \
    }

#define DEFINE_CONST_OPERATOR(op, lhsType, rhsType, body) \
    lhsType operator op(const rhsType& rhs) const \
    { \
        return { \
            body(op, COMMA) \
        }; \
    }

#define VEC_OPERATOR_BODY_VEC(op, sep) \
    x op rhs.x sep \
    y op rhs.y sep \
    z op rhs.z sep

#define VEC_OPERATOR_BODY_FLOAT(op, sep) \
    x op rhs sep \
    y op rhs sep \
    z op rhs sep

struct Vec3
{
    // Public constants

    static const Vec3 Zero;

    static const Vec3 One;

    static const Vec3 Up;

    // 'Structors

    Vec3() :
        Vec3(0.f, 0.f, 0.f)
    {}

    Vec3(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}

    // Raylib conversions

    Vec3(raylib::Vector3 vector) :
            Vec3(vector.x, vector.y, vector.z)
    {}

    operator raylib::Vector3() const
    {
        return {x, y, z};
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

    // Public methods

    std::string ToString() const;

    Vec3 Normalise() const;

    float Length() const
    {
        return sqrtf(x*x + y*y + z*z);
    }

    Vec3 XComp() const
    {
        return {x,0.f,0.f};
    }

    Vec3 YComp() const
    {
        return {0.f, y,0.f};
    }

    Vec3 ZComp() const
    {
        return {0.f,0.f,z};
    }

    // Public members

    float x;
    float y;
    float z;
};

struct BoundedBox
{
    // 'Structors

    BoundedBox() :
        BoundedBox(Vec3(), Vec3())
    {}

    BoundedBox(Vec3 min, Vec3 max) :
        min(min),
        max(max)
    {}

    // Raylib conversions

    operator BoundingBox() const
    {
        return {min, max};
    }

    // Public methods

    bool Intersects(const BoundedBox& other) const;

    // Public members

    Vec3 min;
    Vec3 max;
};

#endif //A_DARK_DISCOMFORT_MATHS_H
