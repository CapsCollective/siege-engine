#ifndef A_DARK_DISCOMFORT_CUSTOMTOOLS_H
#define A_DARK_DISCOMFORT_CUSTOMTOOLS_H

#include <string>
#include <Vector3.hpp>
#include <vector>

class CustomTools
{
public:

    // Private methods

    static std::string VectorToString(raylib::Vector3 vector);

    static raylib::Vector3 StringToVector(std::string string);

    static std::vector<std::string> SplitString(std::string string, char delimiter);
};


#endif //A_DARK_DISCOMFORT_CUSTOMTOOLS_H
