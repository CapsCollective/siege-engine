#include "StringHelpers.h"
#include <algorithm>
#include <stdexcept>

std::string StringHelpers::VectorToString(raylib::Vector3 vector)
{
    return std::to_string(vector.x) + "," + std::to_string(vector.y) + "," + std::to_string(vector.z);
}

raylib::Vector3 StringHelpers::StringToVector(std::string string)
{
    // Split the string at comma values and check the number of components
    std::vector<std::string> components = SplitString(std::move(string), ',');
    if (components.size() != 3) throw std::length_error("Received incorrect number of vector components");

    // Try convert the components to float values and return them as a Vector3
    try
    {
        return {std::stof(components[0]), std::stof(components[1]), std::stof(components[2])};
    }
    catch (const std::invalid_argument& err)
    {
        throw std::invalid_argument("Received non-float vector components");
    }
}

std::vector<std::string> StringHelpers::SplitString(std::string string, char delimiter)
{
    // Iterate over the string while there is still a delimiter
    size_t delimiterPos;
    std::vector<std::string> args;
    while ((delimiterPos = string.find(delimiter)) != std::string::npos)
    {
        // Get up to the next delimiter, add it to the vector, and erase it
        args.push_back(string.substr(0, delimiterPos));
        string.erase(0, args.back().size()+1);
    }
    args.push_back(string);
    return args;
}

std::string StringHelpers::LowercaseString(std::string string)
{
    std::for_each(string.begin(), string.end(), [](char& c){c = (char) std::tolower(c);});
    return string;
}

std::string StringHelpers::Replace(std::string string, const std::string& toReplace, const std::string& replacement)
{
    // Try find the position of the supplied substring and replace the substring at position
    size_t pos = string.find(toReplace);
    if (pos == std::string::npos) return string;
    string.replace(pos, toReplace.length(), replacement);
    return string;
}