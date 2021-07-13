#ifndef A_DARK_DISCOMFORT_STRINGHELPERS_H
#define A_DARK_DISCOMFORT_STRINGHELPERS_H

#include <raylib/Vector3.hpp>
#include <string>
#include <vector>

namespace StringHelpers
{
    /**
     * A helper function to convert a Vector3 to a string
     * @param vector - the vector to convert
     * @return a string representation of the Vector3
     */
    std::string VectorToString(raylib::Vector3 vector);

    /**
     * A helper function to convert a string to a Vector3
     * @param string - the string to convert
     * @return the resulting Vector3 from the string
     * @throws length_error - thrown when the number of
     *                        supplied vector components is
     *                        not equal to 3
     * @throws invalid_argument - thrown if a non-float
     *                            component is found in
     *                            the supplied vector
     */
    raylib::Vector3 StringToVector(std::string string);

    /**
     * A helper function to split a string into a vector of
     * strings by a specified delimiter character
     * @param string - the string to be split
     * @param delimiter - the character to use as the delimiter
     * @return a vector of string values
     */
    std::vector<std::string> SplitString(std::string string, char delimiter);

    /**
     * A helper function to lowercase a supplied string
     * @param string - the string to be lowercased
     * @return the lowercased string
     */
    std::string LowercaseString(std::string string);

    /**
     * A helper function to replace the first instance of a
     * supplied substring with the a supplied replacement
     * @param string - the string to modify
     * @param toReplace - the substring to replace
     * @param replacement - the string to replace it with
     * @return the modified string
     */
    std::string Replace(std::string string, const std::string& toReplace, const std::string& replacement);
}


#endif //A_DARK_DISCOMFORT_STRINGHELPERS_H
