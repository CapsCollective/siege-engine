#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <string>
#include <iostream>

#define RESOURCEMAP std::map<std::string, std::variant<raylib::Model*, raylib::Texture2D*>>

class ResourceManager
{

public:

    template<typename T>
    static void Register(std::string path)
    {
        if (resources.find(path) != resources.end()) return;

        resources.insert({path, new T(path)});
    };

    template<typename T>
    static T& Get(const std::string& path)
    {
        return *std::get<T*>(resources[path]);
    };

    template<typename T>
    static T* GetRef(const std::string& path)
    {
        return std::get<T*>(resources[path]);
    };

private:

    static RESOURCEMAP resources;
};


#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
