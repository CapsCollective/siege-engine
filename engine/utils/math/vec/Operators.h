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

// ------------------------------------------ Vec2 ------------------------------------------------

// Addition

/**
 * A += operator between two Vec2's. Adds the values of the right hand Vec2 to the left hand Vec2
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec2&, const Vec2&)
/**
 * A += operator between a Vec2 and a Vec3. Adds the values of the right hand Vec3 to the left
 * hand Vec2
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec2&, const Vec3&)
/**
 * A += operator between a Vec2 and a Vec4. Adds the values of the right hand Vec2 to the left
 * hand Vec4
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec2&, const Vec4&)

/**
 * A scalar += operator. Adds the scalar value to every dimension of the Vec2 (x and y)
 * @param lhs the Vec2 to add the scalar to
 * @param rhs the scalar value to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec2&, float)

/**
 * A + operator between two Vec2's. Returns a new Vec2 with the result of the addition
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec2 value
 * @return a new Vec2 with the result of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec2, const Vec2&, const Vec2&)
/**
 * A + operator between a Vec2 and a Vec3. Returns a new Vec2 with the result of the addition
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec3 value
 * @return a new Vec2 with the result of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec2, const Vec2&, const Vec3&)
/**
 * A + operator between a Vec2 and a Vec4. Returns a new Vec2 with the result of the addition
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec4 value
 * @return a new Vec2 with the result of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec2, const Vec2&, const Vec4&)

/**
 * A + scalar operator. Adds a scalar to all vector elements
 * @param lhs a 2D vector to be added to
 * @param rhs a scalar value to add to all elements
 * @return a new Vec2 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec2, const Vec2&, float)
/**
 * A + scalar operator. Adds a scalar to all vector elements
 * @param lhs a scalar value to add to all elements
 * @param rhs a 2D vector to be added to
 * @return a new Vec2 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec2, float, const Vec2&)

// Subtraction

/**
 * A -= operator between two Vec2's. Subtracts the values of the right hand Vec2 to the left hand
 * Vec2
 * @param lhs the vector to be subtracted to
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec2&, const Vec2&)
/**
 * A -= operator between a Vec2 and a Vec3. Subtracts the values of the right hand Vec3 to the left
 * hand Vec3
 * @param lhs the vector to be subtracted to
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec2&, const Vec3&)
/**
 * A -= operator between a Vec2 and a Vec4. Subtracts the values of the right hand Vec4 to the left
 * hand Vec4
 * @param lhs the vector to be subtracted to
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec2&, const Vec4&)
/**
 * A scalar -= operator. Subtracts the scalar value to every dimension of the Vec2 (x and y)
 * @param lhs the Vec2 to subtract the scalar to
 * @param rhs the scalar value to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec2&, float)
/**
 * A - operator between two Vec2's. Returns a new Vec2 with the result of the subtraction
 * @param lhs the left-hand Vec2 value
 * @param rhs the  right-hand Vec2 value
 * @return a new Vec2 with the result of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec2, const Vec2&, const Vec2&)
/**
 * A - operator between a Vec2 and a Vec3. Returns a new Vec2 with the result of the subtraction
 * @param lhs the left-hand Vec2 value
 * @param rhs the  right-hand Vec2 value
 * @return a new Vec2 with the result of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec2, const Vec2&, const Vec3&)
/**
 * A - operator between a Vec2 and a Vec4. Returns a new Vec2 with the result of the subtraction
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec2 value
 * @return a new Vec2 with the result of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec2, const Vec2&, const Vec4&)
/**
 * A - scalar operator. Subtracts a scalar to all vector elements
 * @param lhs a scalar value to subtract to all elements
 * @param rhs a 2D vector to be subtracted to
 * @return a new Vec2 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec2, const Vec2&, float)
/**
 * A - scalar operator. Subtracts a scalar to all vector elements
 * @param lhs a 2D vector to be subtracted to
 * @param rhs a scalar value to subtract to all elements
 * @return a new Vec2 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec2, float, const Vec2&)

// Multiplication

/**
 * A *= operator between two Vec2's. Multiplies the values of the right hand Vec2 with the left hand
 * Vec2
 * @param lhs the vector to be multiplied
 * @param rhs the vector to be multiplied with
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec2&, const Vec2&)
/**
 * A *= operator between a Vec2 and a Vec3. Multiplies the values of the right hand Vec2 with the
 * left hand Vec3
 * @param lhs the vector to be multiplied
 * @param rhs the vector to be multiplied with
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec2&, const Vec3&)
/**
 * A *= operator between a Vec2 and a Vec4. Multiplies the values of the right hand Vec2 to the left
 * hand Vec4
 * @param lhs the vector to be multiplied
 * @param rhs the vector to be multiplied with
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec2&, const Vec4&)
/**
 * A scalar *= operator. Multiplies the scalar value to every dimension of the Vec2 (x and y)
 * @param lhs the Vec2 to multiply the scalar to
 * @param rhs the scalar value to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec2&, float)
/**
 * A * operator between two Vec2's. Returns a new Vec2 with the result of the multiplication
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec2 value
 * @return a new Vec2 with the result of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec2, const Vec2&, const Vec2&)
/**
 * A * operator between a Vec2 and a Vec3. Returns a new Vec2 with the result of the multiplication
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec3 value
 * @return a new Vec2 with the result of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec2, const Vec2&, const Vec3&)
/**
 * A * operator between a Vec2 and a Vec4. Returns a new Vec2 with the result of the multiplication
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec4 value
 * @return a new Vec2 with the result of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec2, const Vec2&, const Vec4&)
/**
 * A * scalar operator. Multiplies a scalar to all vector elements
 * @param lhs a 2D vector to be multiplied
 * @param rhs a scalar value to multiply to all elements
 * @return a new Vec2 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec2, const Vec2&, float)
/**
 * A * scalar operator. Multiplies a scalar to all vector elements
 * @param lhs a scalar value to multiply to all elements
 * @param rhs a 2D vector to be multiplied
 * @return a new Vec2 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec2, float, const Vec2&)

// Division

/**
 * A /= operator between two Vec2's. Divides the values of the right hand Vec2 by the left hand Vec2
 * @param lhs the vector to be divided
 * @param rhs the vector to be divided by
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec2&, const Vec2&)
/**
 * A /= operator between a Vec2 and a Vec3. Divides the values of the right hand Vec2 by the left
 * hand Vec3
 * @param lhs the vector to be divided
 * @param rhs the vector to be divided by
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec2&, const Vec3&)
/**
 * A /= operator between a Vec2 and a Vec4. Divides the values of the right hand Vec2 by the left
 * hand Vec4
 * @param lhs the vector to be divided
 * @param rhs the vector to be divided by
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec2&, const Vec4&)
/**
 * A scalar /= operator. Divides the scalar value to every dimension of the Vec2 (x and y)
 * @param lhs the Vec2 to divide the scalar by
 * @param rhs the scalar value to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec2&, float)
/**
 * A / operator between two Vec2's. Returns a new Vec2 with the result of the division
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec2 value
 * @return a new Vec2 with the result of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec2, const Vec2&, const Vec2&)
/**
 * A / operator between a Vec2 and a Vec3. Returns a new Vec2 with the result of the division
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec3 value
 * @return a new Vec2 with the result of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec2, const Vec2&, const Vec3&)
/**
 * A / operator between a Vec2 and a Vec4. Returns a new Vec2 with the result of the division
 * @param lhs the left-hand Vec2 value
 * @param rhs the right-hand Vec4 value
 * @return a new Vec2 with the result of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec2, const Vec2&, const Vec4&)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a 2D vector to be divided
 * @param rhs a scalar value to divided by all elements
 * @return a new Vec2 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec2, const Vec2&, float)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a scalar value to divided by all elements
 * @param rhs a 2D vector to be divided
 * @return a new Vec2 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec2, float, const Vec2&)

// Boolean Operators

/**
 * An == operator. Returns true if the two vectors contain the same elements, otherwise returns
 * false
 * @param lhs the left hand side Vec2
 * @param rhs the right hand side Vec2
 * @return true if the two vectors contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(==, bool, const Vec2&, const Vec2&)
/**
 * A != operator. Returns true if the two vectors do not contain the same elements, otherwise
 * returns false
 * @param lhs the left hand side Vec2
 * @param rhs the right hand side Vec2
 * @return true if the two vectors do not contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(!=, bool, const Vec2&, const Vec2&)

// ------------------------------------------ Vec3 ------------------------------------------------

// Addition

/**
 * A += operator between a Vec3 and a Vec2. Adds the values of the right hand Vec2 to the left hand
 * Vec3
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec3&, const Vec2&)
/**
 * A += operator between two Vec3's. Adds the values of the right hand Vec3 to the left hand Vec3
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec3&, const Vec3&)
/**
 * A += operator between two Vec3's. Adds the values of the right hand Vec4 to the left hand Vec3
 * @param lhs the vector to be added to
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec3&, const Vec4&)
/**
 * A scalar += operator. Adds the scalar value to every dimension of the Vec3 (x, y, and z)
 * @param lhs the Vec3 to add the scalar by
 * @param rhs the scalar value to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec3&, float)
/**
 * A + operator between a Vec3 and a Vec2. Returns a new Vec3 with the result of the addition
 * @param lhs a 3D vector to be added to
 * @param rhs a 2D vector to be added
 * @return a new Vec3 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec3, const Vec3&, const Vec2&)
/**
 * A + operator between two Vec3s. Returns a new Vec3 with the result of the addition
 * @param lhs a 3D vector to be added to
 * @param rhs a 3D vector to be added
 * @return a new Vec3 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec3, const Vec3&, const Vec3&)
/**
 * A + operator between a Vec3 and a Vec4. Returns a new Vec3 with the result of the addition
 * @param lhs a 3D vector to be added to
 * @param rhs a 4D vector to be added
 * @return a new Vec3 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec3, const Vec3&, const Vec4&)
/**
 * A + scalar operator. Divides a scalar by all vector elements
 * @param lhs a 3D vector to be added to
 * @param rhs a scalar value to add to all elements
 * @return a new Vec3 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec3, const Vec3&, float)
/**
 * A + scalar operator. Divides a scalar by all vector elements
 * @param lhs a scalar value to add to all elements
 * @param rhs a 3D vector to be added to
 * @return a new Vec3 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec3, float, const Vec3&)

// Subtraction

/**
 * A -= operator between a Vec3 and a Vec2. Subtracts the values of the right hand Vec2 by the left
 * hand Vec3
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec3&, const Vec2&)
/**
 * A -= operator between two Vec3's. Subtracts the values of the right hand Vec3 by the left hand
 * Vec3
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec3&, const Vec3&)
/**
 * A -= operator between a Vec3 and a Vec4. Subtracts the values of the right hand Vec4 by the left
 * hand Vec3
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec3&, const Vec4&)
/**
 * A scalar -= operator. Subtracts the scalar value by every dimension of the Vec3 (x, y, and z)
 * @param lhs the Vec3 to subtract the scalar by
 * @param rhs the scalar value to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec3&, float)
/**
 * A - operator between a Vec3 and a Vec2. Returns a new Vec3 with the result of the subtraction
 * @param lhs a 3D vector to be subtracted by
 * @param rhs a 2D vector to be subtracted
 * @return a new Vec3 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec3, const Vec3&, const Vec2&)
/**
 * A - operator between two Vec3s. Returns a new Vec3 with the result of the subtraction
 * @param lhs a 3D vector to be subtracted by
 * @param rhs a 3D vector to be subtracted
 * @return a new Vec3 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec3, const Vec3&, const Vec3&)
/**
 * A - operator between a Vec3 and a Vec4. Returns a new Vec3 with the result of the subtraction
 * @param lhs a 3D vector to be subtracted by
 * @param rhs a 4D vector to be subtracted
 * @return a new Vec3 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec3, const Vec3&, const Vec4&)
/**
 * A - scalar operator. Subtracts a scalar by all vector elements
 * @param lhs a 3D vector to be subtracted by
 * @param rhs a scalar value to subtract by all elements
 * @return a new Vec3 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec3, const Vec3&, float)
/**
 * A - scalar operator. Subtracts a scalar by all vector elements
 * @param lhs a scalar value to subtract by all elements
 * @param rhs a 3D vector to be subtracted by
 * @return a new Vec3 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec3, float, const Vec3&)

// Multiplication

/**
 * A *= operator between a Vec3 and a Vec2. Multiplies the values of the right hand Vec2 by the left
 * hand Vec3
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec3&, const Vec2&)
/**
 * A *= operator between two Vec3s. Multiplies the values of the right hand Vec3 by the left hand
 * Vec3
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec3&, const Vec3&)
/**
 * A *= operator between a Vec3 and a Vec2. Multiplies the values of the right hand Vec4 by the left
 * hand Vec3
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec3&, const Vec4&)
/**
 * A scalar *= operator. Multiplies the scalar value by every dimension of the Vec3 (x, y, and z)
 * @param lhs the Vec3 to multiply the scalar by
 * @param rhs the scalar value to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec3&, float)
/**
 * A * operator between a Vec3 and a Vec2. Returns a new Vec3 with the result of the multiplication
 * @param lhs a 3D vector to be multiplied by
 * @param rhs a 2D vector to be multiplied
 * @return a new Vec3 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec3, const Vec3&, const Vec2&)
/**
 * A * operator between two Vec3s. Returns a new Vec3 with the result of the multiplication
 * @param lhs a 3D vector to be multiplied by
 * @param rhs a 3D vector to be multiplied
 * @return a new Vec3 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec3, const Vec3&, const Vec3&)
/**
 * A * operator between two Vec3s. Returns a new Vec3 with the result of the multiplication
 * @param lhs a 3D vector to be multiplied by
 * @param rhs a 4D vector to be multiplied
 * @return a new Vec3 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec3, const Vec3&, const Vec4&)
/**
 * A * scalar operator. Multiplies a scalar by all vector elements
 * @param lhs a 3D vector to be multiplied by
 * @param rhs a scalar value to multiply by all elements
 * @return a new Vec3 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec3, const Vec3&, float)
/**
 * A * scalar operator. Multiplies a scalar by all vector elements
 * @param lhs a scalar value to multiply by all elements
 * @param rhs a 3D vector to be multiplied by
 * @return a new Vec3 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec3, float, const Vec3&)

// Division

/**
 * A /= operator between a Vec3 and a Vec2. Divides the values of the right hand Vec2 by the left
 * hand Vec3
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec3&, const Vec2&)
/**
 * A /= operator between two Vec3s. Divides the values of the right hand Vec3 by the left hand Vec3
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec3&, const Vec3&)
/**
 * A /= operator between a Vec3 and a Vec4. Divides the values of the right hand Vec4 by the left
 * hand Vec3
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec3&, const Vec4&)
/**
 * A scalar /= operator. Divides the scalar value by every dimension of the Vec3 (x, y, and z)
 * @param lhs the Vec3 to divide the scalar by
 * @param rhs the scalar value to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec3&, float)
/**
 * A / operator between a Vec3 and a Vec2. Returns a new Vec3 with the result of the division
 * @param lhs a 3D vector to be divided by
 * @param rhs a 2D vector to be divided
 * @return a new Vec3 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec3, const Vec3&, const Vec2&)
/**
 * A / operator between two Vec3s. Returns a new Vec3 with the result of the division
 * @param lhs a 3D vector to be divided by
 * @param rhs a 3D vector to be divided
 * @return a new Vec3 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec3, const Vec3&, const Vec3&)
/**
 * A / operator between a Vec3 and a Vec4. Returns a new Vec3 with the result of the division
 * @param lhs a 3D vector to be divided by
 * @param rhs a 4D vector to be divided
 * @return a new Vec3 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec3, const Vec3&, const Vec4&)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a 3D vector to be divided by
 * @param rhs a scalar value to divide by all elements
 * @return a new Vec3 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec3, const Vec3&, float)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a scalar value to divide by all elements
 * @param rhs a 3D vector to be divided by
 * @return a new Vec3 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec3, float, const Vec3&)

// Boolean Operators

/**
 * An == operator. Returns true if the two vectors contain the same elements, otherwise returns
 * false
 * @param lhs the left hand side Vec3
 * @param rhs the right hand side Vec3
 * @return true if the two vectors contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(==, bool, const Vec3&, const Vec3&)
/**
 * An != operator. Returns true if the two vectors do not contain the same elements, otherwise
 * returns false
 * @param lhs the left hand side Vec3
 * @param rhs the right hand side Vec3
 * @return true if the two vectors do not contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(!=, bool, const Vec3&, const Vec3&)

// ------------------------------------------ Vec4 ------------------------------------------------

// Addition

/**
 * A += operator between a Vec4 and a Vec2. Adds the values of the right hand Vec2 by the left
 * hand Vec4
 * @param lhs the vector to be added by
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec4&, const Vec2&)
/**
 * A += operator between a Vec4 and a Vec3. Adds the values of the right hand Vec3 by the left
 * hand Vec4
 * @param lhs the vector to be added by
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec4&, const Vec3&)
/**
 * A += operator between two Vec4s. Adds the values of the right hand Vec4 by the left
 * hand Vec4
 * @param lhs the vector to be added by
 * @param rhs the vector to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec4&, const Vec4&)
/**
 * A scalar += operator. Adds the scalar value to every dimension of the Vec4 (x, y, z, and w)
 * @param lhs the Vec4 to add the scalar to
 * @param rhs the scalar value to be added
 */
DECL_BINARY_OP_NO_IMPL(+=, void, Vec4&, float)
/**
 * A + operator between a Vec4 and a Vec2. Returns a new Vec4 with the result of the Addition
 * @param lhs a 4D vector to be added to
 * @param rhs a 2D vector to be added
 * @return a new Vec3 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec4, const Vec4&, const Vec2&)
/**
 * A + operator between a Vec4 and a Vec3. Returns a new Vec4 with the result of the Addition
 * @param lhs a 4D vector to be added to
 * @param rhs a 3D vector to be added
 * @return a new Vec4 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec4, const Vec4&, const Vec3&)
/**
 * A + operator between two Vec4s. Returns a new Vec4 with the result of the Addition
 * @param lhs a 4D vector to be added to
 * @param rhs a 4D vector to be added
 * @return a new Vec4 with the results of the addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec4, const Vec4&, const Vec4&)
/**
 * A + scalar operator. Adds a scalar to all vector elements
 * @param lhs a 4D vector to be added to
 * @param rhs a scalar value to add to all elements
 * @return a new Vec4 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec4, const Vec4&, float)
/**
 * A + scalar operator. Adds a scalar to all vector elements
 * @param lhs a scalar value to add to all elements
 * @param rhs a 4D vector to be added to
 * @return a new Vec4 with the results of the scalar addition
 */
DECL_BINARY_OP_NO_IMPL(+, Vec4, float, const Vec4&)

// Subtraction

/**
 * A -= operator between a Vec4 and a Vec2. Subtracts the values of the right hand Vec2 by the left
 * hand Vec4
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec4&, const Vec2&)
/**
 * A -= operator between a Vec4 and a Vec3. Subtracts the values of the right hand Vec3 by the left
 * hand Vec4
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec4&, const Vec3&)
/**
 * A -= operator between two Vec4s. Subtracts the values of the right hand Vec4 by the left
 * hand Vec4
 * @param lhs the vector to be subtracted by
 * @param rhs the vector to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec4&, const Vec4&)
/**
 * A scalar -= operator. Subtracts the scalar value from every dimension of the Vec4 (x, y, z, and
 * w)
 * @param lhs the Vec4 to subtract the scalar from
 * @param rhs the scalar value to be subtracted
 */
DECL_BINARY_OP_NO_IMPL(-=, void, Vec4&, float)
/**
 * A - operator between a Vec4 and a Vec2. Returns a new Vec4 with the result of the subtraction
 * @param lhs a 4D vector to be subtracted from
 * @param rhs a 2D vector to be subtracted
 * @return a new Vec4 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec4, const Vec4&, const Vec2&)
/**
 * A - operator between a Vec4 and a Vec3. Returns a new Vec4 with the result of the subtraction
 * @param lhs a 4D vector to be subtracted from
 * @param rhs a 3D vector to be subtracted
 * @return a new Vec4 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec4, const Vec4&, const Vec3&)
/**
 * A - operator between two Vec4s. Returns a new Vec4 with the result of the subtraction
 * @param lhs a 4D vector to be subtracted from
 * @param rhs a 4D vector to be subtracted
 * @return a new Vec4 with the results of the subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec4, const Vec4&, const Vec4&)
/**
 * A - scalar operator. Subtracts a scalar by all vector elements
 * @param lhs a 4D vector to be subtracted by
 * @param rhs a scalar value to subtract by all elements
 * @return a new Vec4 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec4, const Vec4&, float)
/**
 * A - scalar operator. Subtracts a scalar by all vector elements
 * @param lhs a scalar value to subtract by all elements
 * @param rhs a 4D vector to be subtracted by
 * @return a new Vec4 with the results of the scalar subtraction
 */
DECL_BINARY_OP_NO_IMPL(-, Vec4, float, const Vec4&)

// Multiplication

/**
 * A *= operator between a Vec4 and a Vec2. Multiplies the values of the right hand Vec2 by the left
 * hand Vec4
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec4&, const Vec2&)
/**
 * A *= operator between a Vec4 and a Vec3. Multiplies the values of the right hand Vec3 by the left
 * hand Vec4
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec4&, const Vec3&)
/**
 * A *= operator between ttwo Vec4s. Multiplies the values of the right hand Vec4 by the left
 * hand Vec4
 * @param lhs the vector to be multiplied by
 * @param rhs the vector to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec4&, const Vec4&)
/**
 * A scalar *= operator. Multiplies the scalar value by every dimension of the Vec4 (x, y, z, and w)
 * @param lhs the Vec4 to multiply the scalar by
 * @param rhs the scalar value to be multiplied
 */
DECL_BINARY_OP_NO_IMPL(*=, void, Vec4&, float)
/**
 * A * operator between a Vec4 and a Vec2. Returns a new Vec4 with the result of the multiplication
 * @param lhs a 4D vector to be multiplied by
 * @param rhs a 2D vector to be multiplied
 * @return a new Vec4 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec4, const Vec4&, const Vec2&)
/**
 * A * operator between a Vec4 and a Vec3. Returns a new Vec4 with the result of the multiplication
 * @param lhs a 4D vector to be multiplied by
 * @param rhs a 3D vector to be multiplied
 * @return a new Vec4 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec4, const Vec4&, const Vec3&)
/**
 * A * operator between two Vec4s. Returns a new Vec4 with the result of the multiplication
 * @param lhs a 4D vector to be multiplied by
 * @param rhs a 4D vector to be multiplied
 * @return a new Vec4 with the results of the multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec4, const Vec4&, const Vec4&)
/**
 * A * scalar operator. Multiplies a scalar by all vector elements
 * @param lhs a 4D vector to be multiplied by
 * @param rhs a scalar value to multiply by all elements
 * @return a new Vec4 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec4, const Vec4&, float)
/**
 * A * scalar operator. Multiplies a scalar by all vector elements
 * @param lhs a scalar value to multiply by all elements
 * @param rhs a 4D vector to be multiplied by
 * @return a new Vec4 with the results of the scalar multiplication
 */
DECL_BINARY_OP_NO_IMPL(*, Vec4, float, const Vec4&)

// Division

/**
 * A /= operator between a Vec4 and a Vec2. Divides the values of the right hand Vec2 by the left
 * hand Vec4
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec4&, const Vec2&)
/**
 * A /= operator between a Vec4 and a Vec3. Divides the values of the right hand Vec3 by the left
 * hand Vec4
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec4&, const Vec3&)
/**
 * A /= operator between two Vec4s. Divides the values of the right hand Vec4 by the left
 * hand Vec4
 * @param lhs the vector to be divided by
 * @param rhs the vector to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec4&, const Vec4&)
/**
 * A scalar /= operator. Divides the scalar value by every dimension of the Vec4 (x, y, z, and w)
 * @param lhs the Vec4 to divide the scalar by
 * @param rhs the scalar value to be divided
 */
DECL_BINARY_OP_NO_IMPL(/=, void, Vec4&, float)
/**
 * A / operator between a Vec4 and a Vec2. Returns a new Vec4 with the result of the division
 * @param lhs a 4D vector to be divided by
 * @param rhs a 2D vector to be divided
 * @return a new Vec4 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec4, const Vec4&, const Vec2&)
/**
 * A / operator between a Vec4 and a Vec3. Returns a new Vec4 with the result of the division
 * @param lhs a 4D vector to be divided by
 * @param rhs a 3D vector to be divided
 * @return a new Vec4 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec4, const Vec4&, const Vec3&)
/**
 * A / operator between two Vec4s. Returns a new Vec4 with the result of the division
 * @param lhs a 4D vector to be divided by
 * @param rhs a 3D vector to be divided
 * @return a new Vec4 with the results of the division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec4, const Vec4&, const Vec4&)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a 4D vector to be divided by
 * @param rhs a scalar value to divide by all elements
 * @return a new Vec4 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec4, const Vec4&, float)
/**
 * A / scalar operator. Divides a scalar by all vector elements
 * @param lhs a scalar value to divide by all elements
 * @param rhs a 4D vector to be divided by
 * @return a new Vec4 with the results of the scalar division
 */
DECL_BINARY_OP_NO_IMPL(/, Vec4, float, const Vec4&)

// Boolean Operators

/**
 * An == operator. Returns true if the two vectors contain the same elements, otherwise returns
 * false
 * @param lhs the left hand side Vec4
 * @param rhs the right hand side Vec4
 * @return true if the two vectors contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(==, bool, const Vec4&, const Vec4&)
/**
 * An != operator. Returns true if the two vectors do not contain the same elements, otherwise
 * returns false
 * @param lhs the left hand side Vec4
 * @param rhs the right hand side Vec4
 * @return true if the two vectors do not contain the same elements, false otherwise
 */
DECL_BINARY_OP_NO_IMPL(!=, bool, const Vec4&, const Vec4&)

} // namespace Siege

#endif // SIEGE_ENGINE_OPERATORS_H
