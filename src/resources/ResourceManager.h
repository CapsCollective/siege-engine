#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <vector>

// Define macros

// A macro which defines what a resource map actually looks like
#define RESOURCE_MAP std::map<std::string, std::variant<Model, Texture2D>>

// TODO comment API functions
class ResourceManager
{
public:

    // Public functions

    /**
     * Loads a resource from a path and adds it to the resource map.
     * @tparam T - defines the type of resource being added (Model or Texture2D).
     * @param path - the path the resource will be loaded from.
     */
    template<typename T>
    static void Register(const std::string& path)
    {
        // If the resource already exists then exit the function.
        if (resources.find(path) != resources.end()) return;

        // Define an empty variant to hold our resource.
        std::variant<Model, Texture> resource = std::variant<Model, Texture2D>();

        // Check what type of resource is being registered.
        // In this case, check that the resource is a Model
        if (std::is_same<T, Model>::value)
        {
            // If it is a model, load the model and store the struct in our variant.
            resource = LoadModel(path.c_str());
        }
        // If the resource is a Texture2D
        else if (std::is_same<T, Texture2D>::value)
        {
            // Load the TEXTURE and store the struct in our variant.
            resource = LoadTexture(path.c_str());
        }

        // Add the variant to our resources map.
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
