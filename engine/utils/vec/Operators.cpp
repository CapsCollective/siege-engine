//
//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#include "Operators.h"

#define DEF_BIN_OP_IMP(op, returnType, lhsType, rhsType, ...) \
    returnType operator op(lhsType lhs, rhsType rhs)          \
    {                                                         \
        __VA_ARGS__                                           \
    }

#define FORCE_EVAL(...) __VA_ARGS__

#define RETURN(body) return body _SEMICOLON

#define RETURN_VEC(body) FORCE_EVAL(return {body} _SEMICOLON)

#define VEC2_OP_BODY_VEC(op, sep) lhs.x op rhs.x sep lhs.y op rhs.y sep

#define VEC2_OP_BODY_PARAMS(op) lhs.x op rhs.x _COMMA lhs.y op rhs.y

#define VEC2_OP_BODY_PARAMS_F(op) lhs.x op rhs _COMMA lhs.y op rhs

#define VEC2_OP_BODY_PARAMS_F_REV(op) lhs op rhs.x _COMMA lhs op rhs.y

#define VEC2_OP_BODY_F(op, sep) lhs.x op rhs sep lhs.y op rhs sep

#define VEC2_OP_BODY_F_REV(op, sep) lhs op rhs.x sep lhs op rhs.y sep

#define VEC2_LOGIC_OP(op, logic, sep) lhs.x op rhs.x logic lhs.y op rhs.y

#define VEC3_OP_BODY_VEC(op, sep) lhs.x op rhs.x sep lhs.y op rhs.y sep lhs.z op rhs.z sep

#define VEC3_OP_BODY_PARAMS(op) VEC2_OP_BODY_PARAMS(op) _COMMA lhs.z op rhs.z

#define VEC3_OP_BODY_PARAMS_F(op) VEC2_OP_BODY_PARAMS_F(op) _COMMA lhs.z op rhs

#define VEC3_OP_BODY_PARAMS_F_REV(op) VEC2_OP_BODY_PARAMS_F_REV(op) _COMMA lhs op rhs.z

#define VEC3_OP_BODY_F(op, sep) VEC2_OP_BODY_F(op, sep) lhs.z op rhs sep

#define VEC3_LOGIC_OP(op, logic, sep) VEC2_LOGIC_OP(op, logic, sep) logic lhs.z op rhs.z

#define VEC4_OP_BODY_VEC(op, sep) \
    lhs.x op rhs.x sep lhs.y op rhs.y sep lhs.z op rhs.z sep lhs.w op rhs.w sep

#define VEC4_OP_BODY_PARAMS(op) VEC3_OP_BODY_PARAMS(op) _COMMA lhs.w op rhs.w

#define VEC4_OP_BODY_PARAMS_F(op) VEC3_OP_BODY_PARAMS_F(op) _COMMA lhs.w op rhs

#define VEC4_OP_BODY_PARAMS_F_REV(op) VEC3_OP_BODY_PARAMS_F_REV(op) _COMMA lhs op rhs.w

#define VEC4_OP_BODY_F(op, sep) VEC3_OP_BODY_F(op, sep) lhs.w op rhs sep

#define VEC4_LOGIC_OP(op, logic, sep) VEC3_LOGIC_OP(op, logic, sep) logic lhs.w op rhs.w

namespace Siege
{

// Vec2

// Addition

DEF_BIN_OP_IMP(+=, void, Vec2&, const Vec2&, VEC2_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec2&, const Vec3&, VEC2_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec2&, const Vec4&, VEC2_OP_BODY_VEC(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+=, void, Vec2&, float, VEC2_OP_BODY_F(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+, Vec2, const Vec2&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_VEC(+, _COMMA)))
DEF_BIN_OP_IMP(+, Vec2, const Vec2&, const Vec3&, RETURN_VEC(VEC2_OP_BODY_VEC(+, _COMMA)))
DEF_BIN_OP_IMP(+, Vec2, const Vec2&, const Vec4&, RETURN_VEC(VEC2_OP_BODY_VEC(+, _COMMA)))

DEF_BIN_OP_IMP(+, Vec2, const Vec2&, float, RETURN_VEC(VEC2_OP_BODY_F(+, _COMMA)))
DEF_BIN_OP_IMP(+, Vec2, float, const Vec2&, RETURN_VEC(VEC2_OP_BODY_F_REV(+, _COMMA)))

// Subtraction

DEF_BIN_OP_IMP(-=, void, Vec2&, const Vec2&, VEC2_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec2&, const Vec3&, VEC2_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec2&, const Vec4&, VEC2_OP_BODY_VEC(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-=, void, Vec2&, float, VEC2_OP_BODY_F(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-, Vec2, const Vec2&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_VEC(-, _COMMA)))
DEF_BIN_OP_IMP(-, Vec2, const Vec2&, const Vec3&, RETURN_VEC(VEC2_OP_BODY_VEC(-, _COMMA)))
DEF_BIN_OP_IMP(-, Vec2, const Vec2&, const Vec4&, RETURN_VEC(VEC2_OP_BODY_VEC(-, _COMMA)))

DEF_BIN_OP_IMP(-, Vec2, const Vec2&, float, RETURN_VEC(VEC2_OP_BODY_F(-, _COMMA)))
DEF_BIN_OP_IMP(-, Vec2, float, const Vec2&, RETURN_VEC(VEC2_OP_BODY_F_REV(-, _COMMA)))

// Multiplication

DEF_BIN_OP_IMP(*=, void, Vec2&, const Vec2&, VEC2_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec2&, const Vec3&, VEC2_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec2&, const Vec4&, VEC2_OP_BODY_VEC(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*=, void, Vec2&, float, VEC2_OP_BODY_F(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*, Vec2, const Vec2&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_VEC(*, _COMMA)))
DEF_BIN_OP_IMP(*, Vec2, const Vec2&, const Vec3&, RETURN_VEC(VEC2_OP_BODY_VEC(*, _COMMA)))
DEF_BIN_OP_IMP(*, Vec2, const Vec2&, const Vec4&, RETURN_VEC(VEC2_OP_BODY_VEC(*, _COMMA)))

DEF_BIN_OP_IMP(*, Vec2, const Vec2&, float, RETURN_VEC(VEC2_OP_BODY_F(*, _COMMA)))
DEF_BIN_OP_IMP(*, Vec2, float, const Vec2&, RETURN_VEC(VEC2_OP_BODY_F_REV(*, _COMMA)))

// Division

DEF_BIN_OP_IMP(/=, void, Vec2&, const Vec2&, VEC2_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec2&, const Vec3&, VEC2_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec2&, const Vec4&, VEC2_OP_BODY_VEC(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/=, void, Vec2&, float, VEC2_OP_BODY_F(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/, Vec2, const Vec2&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_VEC(/, _COMMA)))
DEF_BIN_OP_IMP(/, Vec2, const Vec2&, const Vec3&, RETURN_VEC(VEC2_OP_BODY_VEC(/, _COMMA)))
DEF_BIN_OP_IMP(/, Vec2, const Vec2&, const Vec4&, RETURN_VEC(VEC2_OP_BODY_VEC(/, _COMMA)))

DEF_BIN_OP_IMP(/, Vec2, const Vec2&, float, RETURN_VEC(VEC2_OP_BODY_F(/, _COMMA)))
DEF_BIN_OP_IMP(/, Vec2, float, const Vec2&, RETURN_VEC(VEC2_OP_BODY_F_REV(/, _COMMA)))

// Boolean Operators

DEF_BIN_OP_IMP(==, bool, const Vec2&, const Vec2&, RETURN(VEC2_LOGIC_OP(==, &&, _SEMICOLON)))
DEF_BIN_OP_IMP(!=, bool, const Vec2&, const Vec2&, RETURN(VEC2_LOGIC_OP(!=, ||, _SEMICOLON)))

// Vec3

// Addition

DEF_BIN_OP_IMP(+=, void, Vec3&, const Vec2&, VEC2_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec3&, const Vec3&, VEC3_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec3&, const Vec4&, VEC3_OP_BODY_VEC(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+=, void, Vec3&, float, VEC3_OP_BODY_F(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+, Vec3, const Vec3&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_PARAMS(+) _COMMA lhs.z))
DEF_BIN_OP_IMP(+, Vec3, const Vec3&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(+)))
DEF_BIN_OP_IMP(+, Vec3, const Vec3&, const Vec4&, RETURN_VEC(VEC3_OP_BODY_PARAMS(+)))

DEF_BIN_OP_IMP(+, Vec3, const Vec3&, float, RETURN_VEC(VEC3_OP_BODY_PARAMS_F(+)))
DEF_BIN_OP_IMP(+, Vec3, float, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS_F_REV(+)))

// Subtraction

DEF_BIN_OP_IMP(-=, void, Vec3&, const Vec2&, VEC2_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec3&, const Vec3&, VEC3_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec3&, const Vec4&, VEC3_OP_BODY_VEC(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-=, void, Vec3&, float, VEC3_OP_BODY_F(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-, Vec3, const Vec3&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_PARAMS(-) _COMMA lhs.z))
DEF_BIN_OP_IMP(-, Vec3, const Vec3&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(-)))
DEF_BIN_OP_IMP(-, Vec3, const Vec3&, const Vec4&, RETURN_VEC(VEC3_OP_BODY_PARAMS(-)))

DEF_BIN_OP_IMP(-, Vec3, const Vec3&, float, RETURN_VEC(VEC3_OP_BODY_PARAMS_F(-)))
DEF_BIN_OP_IMP(-, Vec3, float, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS_F_REV(-)))

// Multiplication

DEF_BIN_OP_IMP(*=, void, Vec3&, const Vec2&, VEC2_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec3&, const Vec3&, VEC3_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec3&, const Vec4&, VEC3_OP_BODY_VEC(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*=, void, Vec3&, float, VEC3_OP_BODY_F(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*, Vec3, const Vec3&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_PARAMS(*) _COMMA lhs.z))
DEF_BIN_OP_IMP(*, Vec3, const Vec3&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(*)))
DEF_BIN_OP_IMP(*, Vec3, const Vec3&, const Vec4&, RETURN_VEC(VEC3_OP_BODY_PARAMS(*)))

DEF_BIN_OP_IMP(*, Vec3, const Vec3&, float, RETURN_VEC(VEC3_OP_BODY_PARAMS_F(*)))
DEF_BIN_OP_IMP(*, Vec3, float, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS_F_REV(*)))

// Division

DEF_BIN_OP_IMP(/=, void, Vec3&, const Vec2&, VEC2_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec3&, const Vec3&, VEC3_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec3&, const Vec4&, VEC3_OP_BODY_VEC(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/=, void, Vec3&, float, VEC3_OP_BODY_F(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/, Vec3, const Vec3&, const Vec2&, RETURN_VEC(VEC2_OP_BODY_PARAMS(/) _COMMA lhs.z))
DEF_BIN_OP_IMP(/, Vec3, const Vec3&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(/)))
DEF_BIN_OP_IMP(/, Vec3, const Vec3&, const Vec4&, RETURN_VEC(VEC3_OP_BODY_PARAMS(/)))

DEF_BIN_OP_IMP(/, Vec3, const Vec3&, float, RETURN_VEC(VEC3_OP_BODY_PARAMS_F(/)))
DEF_BIN_OP_IMP(/, Vec3, float, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS_F_REV(/)))

// Boolean Operators

DEF_BIN_OP_IMP(==, bool, const Vec3&, const Vec3&, RETURN(VEC3_LOGIC_OP(==, &&, _SEMICOLON)))
DEF_BIN_OP_IMP(!=, bool, const Vec3&, const Vec3&, RETURN(VEC3_LOGIC_OP(!=, ||, _SEMICOLON)))

// Vec4

// Addition

// static Vec4 operator+(const Vec4& lhs, float rhs);

DEF_BIN_OP_IMP(+=, void, Vec4&, const Vec2&, VEC2_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec4&, const Vec3&, VEC3_OP_BODY_VEC(+=, _SEMICOLON))
DEF_BIN_OP_IMP(+=, void, Vec4&, const Vec4&, VEC4_OP_BODY_VEC(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+=, void, Vec4&, float, VEC4_OP_BODY_F(+=, _SEMICOLON))

DEF_BIN_OP_IMP(+,
               Vec4,
               const Vec4&,
               const Vec2&,
               RETURN_VEC(VEC2_OP_BODY_PARAMS(+) _COMMA lhs.z _COMMA lhs.w))
DEF_BIN_OP_IMP(+, Vec4, const Vec4&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(+) _COMMA lhs.w))
DEF_BIN_OP_IMP(+, Vec4, const Vec4&, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS(+)))

DEF_BIN_OP_IMP(+, Vec4, const Vec4&, float, RETURN_VEC(VEC4_OP_BODY_PARAMS_F(+)))
DEF_BIN_OP_IMP(+, Vec4, float, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS_F_REV(+)))

// Subtraction

DEF_BIN_OP_IMP(-=, void, Vec4&, const Vec2&, VEC2_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec4&, const Vec3&, VEC3_OP_BODY_VEC(-=, _SEMICOLON))
DEF_BIN_OP_IMP(-=, void, Vec4&, const Vec4&, VEC4_OP_BODY_VEC(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-=, void, Vec4&, float, VEC4_OP_BODY_F(-=, _SEMICOLON))

DEF_BIN_OP_IMP(-,
               Vec4,
               const Vec4&,
               const Vec2&,
               RETURN_VEC(VEC2_OP_BODY_PARAMS(-) _COMMA lhs.z _COMMA lhs.w))
DEF_BIN_OP_IMP(-, Vec4, const Vec4&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(-) _COMMA lhs.w))
DEF_BIN_OP_IMP(-, Vec4, const Vec4&, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS(-)))

DEF_BIN_OP_IMP(-, Vec4, const Vec4&, float, RETURN_VEC(VEC4_OP_BODY_PARAMS_F(-)))
DEF_BIN_OP_IMP(-, Vec4, float, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS_F_REV(-)))

// Multiplication

DEF_BIN_OP_IMP(*=, void, Vec4&, const Vec2&, VEC2_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec4&, const Vec3&, VEC3_OP_BODY_VEC(*=, _SEMICOLON))
DEF_BIN_OP_IMP(*=, void, Vec4&, const Vec4&, VEC4_OP_BODY_VEC(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*=, void, Vec4&, float, VEC4_OP_BODY_F(*=, _SEMICOLON))

DEF_BIN_OP_IMP(*,
               Vec4,
               const Vec4&,
               const Vec2&,
               RETURN_VEC(VEC2_OP_BODY_PARAMS(*) _COMMA lhs.z _COMMA lhs.w))
DEF_BIN_OP_IMP(*, Vec4, const Vec4&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(*) _COMMA lhs.w))
DEF_BIN_OP_IMP(*, Vec4, const Vec4&, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS(*)))

DEF_BIN_OP_IMP(*, Vec4, const Vec4&, float, RETURN_VEC(VEC4_OP_BODY_PARAMS_F(*)))
DEF_BIN_OP_IMP(*, Vec4, float, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS_F_REV(*)))

// Division

DEF_BIN_OP_IMP(/=, void, Vec4&, const Vec2&, VEC2_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec4&, const Vec3&, VEC3_OP_BODY_VEC(/=, _SEMICOLON))
DEF_BIN_OP_IMP(/=, void, Vec4&, const Vec4&, VEC4_OP_BODY_VEC(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/=, void, Vec4&, float, VEC4_OP_BODY_F(/=, _SEMICOLON))

DEF_BIN_OP_IMP(/,
               Vec4,
               const Vec4&,
               const Vec2&,
               RETURN_VEC(VEC2_OP_BODY_PARAMS(/) _COMMA lhs.z _COMMA lhs.w))
DEF_BIN_OP_IMP(/, Vec4, const Vec4&, const Vec3&, RETURN_VEC(VEC3_OP_BODY_PARAMS(/) _COMMA lhs.w))
DEF_BIN_OP_IMP(/, Vec4, const Vec4&, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS(/)))

DEF_BIN_OP_IMP(/, Vec4, const Vec4&, float, RETURN_VEC(VEC4_OP_BODY_PARAMS_F(/)))
DEF_BIN_OP_IMP(/, Vec4, float, const Vec4&, RETURN_VEC(VEC4_OP_BODY_PARAMS_F_REV(/)))

// Boolean Operators

DEF_BIN_OP_IMP(==, bool, const Vec4&, const Vec4&, RETURN(VEC4_LOGIC_OP(==, &&, _SEMICOLON)))
DEF_BIN_OP_IMP(!=, bool, const Vec4&, const Vec4&, RETURN(VEC4_LOGIC_OP(!=, ||, _SEMICOLON)))

} // namespace Siege
