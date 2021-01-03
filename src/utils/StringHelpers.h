#ifndef A_DARK_DISCOMFORT_STRINGHELPERS_H
#define A_DARK_DISCOMFORT_STRINGHELPERS_H

#include <Vector3.hpp>
#include <string>
#include <vector>

namespace StringHelpers
{
    /**
     * A helper function to convert a Vector3 to a string
     * @param vector - the vector to convert
     * @return A string representation of the Vector3
     */
    std::string VectorToString(raylib::Vector3 vector);

    /**
     * A helper function to convert a string to a Vector3
     * @param string - the string to convert
     * @return The resulting Vector3 from the string
     */
    raylib::Vector3 StringToVector(std::string string);

    /**
     * A helper function to split a string into a vector of
     * strings by a specified delimiter character
     * @param string - the string to be split
     * @param delimiter - the character to use as the delimiter
     * @return A vector of string values
     */
    std::vector<std::string> SplitString(std::string string, char delimiter);

    /**
     * A helper function to lowercase a supplied string
     * @param string - the string to be lowercased
     * @return The lowercased string
     */
    std::string LowercaseString(std::string string);
}


#endif //A_DARK_DISCOMFORT_STRINGHELPERS_H