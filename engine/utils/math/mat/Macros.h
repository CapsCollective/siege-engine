//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_MAT_MACROS_H
#define SIEGE_ENGINE_MAT_MACROS_H

#define X(idx, size) idx* size
#define Y(idx, size) (idx * size) + 1
#define Z(idx, size) (idx * size) + 2
#define W(idx, size) (idx * size) + 3

#define LHS(dimension, idx, size) values[dimension(idx, size)]
#define RHS(dimension, idx, size) other[dimension(idx, size)]

#define DEFINE_MAT_VEC_OP(op, sep, idx, element, rowSize) \
    LHS(element, idx, rowSize) op RHS(element, idx, rowSize)

#define DEFINE_SCALAR_OP(op, sep, idx, element, rowSize) LHS(element, idx, rowSize) op scalar

// ------------------------------------ MAT2 MACROS -----------------------------------------------

#define DOT(m0x, m0y, m1x, m1y) (m0x * m1x) + (m0y * m1y)

#define MAT2_MATH_OP(op, body) body(op, _SEMICOLON, 0) _SEMICOLON body(op, _SEMICOLON, 1)

#define MAT2_LOGIC_OP(op, logic, bLogic, body) body(op, logic, 0) bLogic body(op, logic, 1)

#define MAT2_BODY_FUNC(op, sep, idx) \
    DEFINE_MAT_VEC_OP(op, sep, idx, X, 2) sep DEFINE_MAT_VEC_OP(op, sep, idx, Y, 2)

#define MAT2_BODY_SCALAR_FUNC(op, sep, idx) \
    DEFINE_SCALAR_OP(op, sep, idx, X, 2) sep DEFINE_SCALAR_OP(op, sep, idx, Y, 2)

#define GET_ROW2(varX, varY, idx) varX = LHS(X, idx, 2), varY = LHS(Y, idx, 2);

#define GET_COLUMN2(varX, varY, vals, idx) varX = vals[idx], varY = vals[idx + 2];

// ------------------------------------ MAT3 MACROS -----------------------------------------------

#define MAT3_MATH_OP(op, body) MAT2_MATH_OP(op, body) _SEMICOLON body(op, _SEMICOLON, 2)

#define MAT3_LOGIC_OP(op, logic, bLogic, body) \
    MAT2_LOGIC_OP(op, logic, bLogic, body) bLogic body(op, logic, 2)

#define MAT3_BODY_FUNC(op, sep, idx)      \
    DEFINE_MAT_VEC_OP(op, sep, idx, X, 3) \
    sep DEFINE_MAT_VEC_OP(op, sep, idx, Y, 3) sep DEFINE_MAT_VEC_OP(op, sep, idx, Z, 3)

#define MAT3_BODY_SCALAR_FUNC(op, sep, idx) \
    DEFINE_SCALAR_OP(op, sep, idx, X, 3)    \
    sep DEFINE_SCALAR_OP(op, sep, idx, Y, 3) sep DEFINE_SCALAR_OP(op, sep, idx, Z, 3)

#define GET_ROW3(varX, varY, varZ, idx) \
    varX = LHS(X, idx, 3), varY = LHS(Y, idx, 3), varZ = LHS(Z, idx, 3);

#define GET_COLUMN3(varX, varY, varZ, vals, idx) \
    varX = vals[idx], varY = vals[idx + 3], varZ = vals[idx + 6];

#define DOT3(m0x, m0y, m0z, m1x, m1y, m1z) DOT(m0x, m0y, m1x, m1y) + (m0z * m1z)

// ------------------------------------ MAT4 MACROS -----------------------------------------------

#define MAT4_MATH_OP(op, body) MAT3_MATH_OP(op, body) _SEMICOLON body(op, _SEMICOLON, 3)

#define MAT4_LOGIC_OP(op, logic, bLogic, body) \
    MAT3_LOGIC_OP(op, logic, bLogic, body) bLogic body(op, logic, 3)

#define MAT4_BODY_FUNC(op, sep, idx)                                                        \
    DEFINE_MAT_VEC_OP(op, sep, idx, X, 4)                                                   \
    sep DEFINE_MAT_VEC_OP(op, sep, idx, Y, 4) sep DEFINE_MAT_VEC_OP(op, sep, idx, Z, 4) sep \
    DEFINE_MAT_VEC_OP(op, sep, idx, W, 4)

#define MAT4_BODY_SCALAR_FUNC(op, sep, idx)                                               \
    DEFINE_SCALAR_OP(op, sep, idx, X, 4)                                                  \
    sep DEFINE_SCALAR_OP(op, sep, idx, Y, 4) sep DEFINE_SCALAR_OP(op, sep, idx, Z, 4) sep \
    DEFINE_SCALAR_OP(op, sep, idx, W, 4)

#define GET_ROW4(varX, varY, varZ, varW, idx) \
    varX = LHS(X, idx, 4), varY = LHS(Y, idx, 4), varZ = LHS(Z, idx, 4), varW = LHS(W, idx, 4);

#define GET_COLUMN4(varX, varY, varZ, varW, vals, idx) \
    varX = vals[idx], varY = vals[idx + 4], varZ = vals[idx + 8], varW = vals[idx + 12];

#define DOT4(m0x, m0y, m0z, m0w, m1x, m1y, m1z, m1w) \
    DOT3(m0x, m0y, m0z, m1x, m1y, m1z) + (m0w * m1w)

#endif // SIEGE_ENGINE_MAT_MACROS_H
