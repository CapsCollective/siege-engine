#include "StringHelpers.h"

std::string StringHelpers::VectorToString(raylib::Vector3 vector)
{
    return std::to_string(vector.x) + "," +
           std::to_string(vector.y) + "," +
           std::to_string(vector.z) + ",";
}

raylib::Vector3 StringHelpers::StringToVector(std::string string)
{
    std::vector<std::string> components = SplitString(std::move(string), ',');
    return {
            std::stof(components[0]),
            std::stof(components[1]),
            std::stof(components[2]),
    };
}

std::vector<std::string> StringHelpers::SplitString(std::string string, char delimiter)
{
    // Iterate over the string while there is still a delimiter
    int delimiterPos;
    std::vector<std::string> args;
    while ((delimiterPos = string.find(delimiter)) != std::string::npos)
    {
        // Get up to the next delimiter, add it to the vector, and erase it
        args.push_back(string.substr(0, delimiterPos));
        string.erase(0, args.back().size()+1);
    }
    return args;
}

std::string StringHelpers::LowercaseString(std::string string)
{
    std::for_each(string.begin(), string.end(), [](char& c){c = (char) std::tolower(c);});
    return string;
}