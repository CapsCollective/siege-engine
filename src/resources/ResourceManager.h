#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <vector>

// Define macros
#define RESOURCE_MAP std::map<std::string, std::variant<Model, Texture2D>>

class ResourceManager
{
public:

    // Public functions
    template<typename T>
    static void Register(const std::string& path)
    {
        // TODO comment Register
        if (resources.find(path) != resources.end()) return;

        std::variant<Model, Texture> resource = std::variant<Model, Texture2D>();

        if (std::is_same<T, Model>::value)
        {
            resource = LoadModel(path.c_str());
        }
        else if (std::is_same<T, Texture2D>::value)
        {
            resource = LoadTexture(path.c_str());
        }

        resources.insert({path, resource});
    };

    template<typename T>
    static T& Get(const std::string& path)
    {
        return std::get<T>(resources.at(path));
    };

    template<typename T>
    static T* GetRef(const std::string& path)
    {
        return &std::get<T>(resources[path]);
    };

    static void FreeAllResources();

    static void ClearResources();

private:

    // Private fields

    static RESOURCE_MAP resources;

    static std::vector<std::variant<Model, Texture2D>*> freedResources;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
