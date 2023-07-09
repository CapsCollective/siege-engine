//
//  Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
//  This code is released under an unmodified zlib license.
//  For conditions of distribution and use, please see:
//      https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_UTILS_MATH_VEC_FORMAT_H
#define SIEGE_ENGINE_UTILS_MATH_VEC_FORMAT_H

#include "../../String.h"
#include "Vec.h"

namespace Siege
{
/**
 * Converts a 4D vector to a string
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector to convert to a string
 * @return a string representation of the vector
 */
template<typename T>
inline String ToString(const Vec<T, 4>& vec)
{
    return String("%.2f,%.2f,%.2f,%.2f").Formatted(vec.x, vec.y, vec.z, vec.w);
}

/**
 * Converts a 3D vector to a string
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector to convert to a string
 * @return a string representation of the vector
 */
template<typename T>
inline String ToString(const Vec<T, 3>& vec)
{
    return String("%.2f,%.2f,%.2f").Formatted(vec.x, vec.y, vec.z);
}

/**
 * Converts a 2D vector to a string
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector to convert to a string
 * @return a string representation of the vector
 */
template<typename T>
inline String ToString(const Vec<T, 2>& vec)
{
    return String("%.2f,%.2f").Formatted(vec.x, vec.y);
}

/**
 * Creates a 4D vector from a string. Typically used for deserialisation
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector which will be populated by the string
 * @param str the string to convert from
 * @return a boolean representing if the conversion was successful
 */
template<typename T>
inline bool FromString(Vec<T, 4>& vec, const String& str)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = str.Split(',');
    if (components.size() != 4) return false;

    // Try to convert the components to float values and return them as a Vector4
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y) &&
                components[2].GetFloat(vec.z),
            components[3].GetFloat(vec.w));
}

/**
 * Creates a 3D vector from a string. Typically used for deserialisation
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector which will be populated by the string
 * @param str the string to convert from
 * @return a boolean representing if the conversion was successful
 */
template<typename T>
inline bool FromString(Vec<T, 3>& vec, const String& str)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = str.Split(',');
    if (components.size() != 3) return false;

    // Try to convert the components to float values and return them as a Vector3
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y) &&
            components[2].GetFloat(vec.z));
}

/**
 * Creates a 2D vector from a string. Typically used for deserialisation
 * @tparam T the type of numerical value stored by the vector
 * @param vec the vector which will be populated by the string
 * @param str the string to convert from
 * @return a boolean representing if the conversion was successful
 */
template<typename T>
inline bool FromString(Vec<T, 2>& vec, const String& str)
{
    // Split the string at comma values and check the number of components
    const std::vector<String>& components = str.Split(',');
    if (components.size() != 2) return false;

    // Try to convert the components to float values and return them as a Vector3
    return (components[0].GetFloat(vec.x) && components[1].GetFloat(vec.y));
}
} // namespace Siege

#endif // SIEGE_ENGINE_UTILS_MATH_VEC_FORMAT_H
