#ifndef A_DARK_DISCOMFORT_HELPERFUNCS_H
#define A_DARK_DISCOMFORT_HELPERFUNCS_H

#include <string>
#include <Vector3.hpp>
#include <vector>

namespace HelperFuncs
{
    std::string VectorToString(raylib::Vector3 vector);

    raylib::Vector3 StringToVector(std::string string);

    std::vector<std::string> SplitString(std::string string, char delimiter);
};


#endif //A_DARK_DISCOMFORT_HELPERFUNCS_H
