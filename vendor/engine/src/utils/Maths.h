#ifndef A_DARK_DISCOMFORT_MATHS_H
#define A_DARK_DISCOMFORT_MATHS_H

#include "Macros.h"
#include <string>
#include <cmath>

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

struct Xform
{
public:

    Xform() :
        Xform(Vec3::Zero)
    {}

    explicit Xform(Vec3 position) :
        Xform(position, 0.f)
    {}

    Xform(Vec3 position, float rotation) :
        Xform(position, rotation, Vec3::One)
    {}

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

    BoundedBox() :
        BoundedBox(Vec3(), Vec3())
    {}

    BoundedBox(Vec3 min, Vec3 max) :
        min(min),
        max(max)
    {}

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

    RayCast() :
        RayCast(Vec3(), Vec3())
    {}

    RayCast(Vec3 position, Vec3 direction) :
        position(position),
        direction(direction)
    {}

    // Public members

    Vec3 position;
    Vec3 direction;
};

struct Colour
{
    // Public constants

    static const Colour Black;

    static const Colour White;

    static const Colour Red;

    static const Colour Green;

    static const Colour Blue;

    // 'Structors

    Colour() :
        Colour(0, 0, 0, 0)
    {}

    Colour(int r, int g, int b) :
        r(r),
        g(g),
        b(b),
        a(255)
    {}

    Colour(int r, int g, int b, int a) :
        r(r),
        g(g),
        b(b),
        a(a)
    {}

    // Public members

    int r;
    int g;
    int b;
    int a;
};

#endif //A_DARK_DISCOMFORT_MATHS_H
