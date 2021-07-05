#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <vector>

// Define types

// Resource map stores a union (std::variant) of either a model or a Texture2D
typedef std::map<std::string, std::variant<Model, Texture2D>> ResourceMap;
typedef std::vector<std::variant<Model, Texture2D>*> ResourceList;

class ResourceManager
{
public:

    // Public functions

    /**
     * Loads a resource from a path and adds it to the resource map
     * @tparam T - defines the type of resource being added (Model or Texture2D)
     * @param path - the path the resource will be loaded from
     */
    template<typename T>
    static void Register(const std::string& path)
    {
        if (resources.find(path) != resources.end()) return;

        // Define an empty variant to hold resource
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

    /**
     * Returns a reference to a resource
     * @tparam T - the type of resource which needs to be returned
     * @param path - the path of the resource in the file system
     * @return a reference to the designated resource
     */
    template<typename T>
    static T& Get(const std::string& path)
    {
        // Unwraps the variant in the given path
        return std::get<T>(resources.at(path));
    };

    /**
     * Returns a pointer to a resource
     * @tparam T - the type of resource which needs to be returned
     * @param path - the path of the resource in the file system
     * @return a pointer to the designated resource
     */
    template<typename T>
    static T* GetRef(const std::string& path)
    {
        // Unwraps the variant in the given path and returns a pointer
        return &std::get<T>(resources[path]);
    };

    /**
    * Clears all stored resources at the end of the frame
    */
    static void FreeResources();

    /**
     * Immediately clears all stored resources
     */
    static void ClearResources();

private:

    // Private fields

    /**
     * All resources in the system - currently limited to textures and models
     */
    static ResourceMap resources;

    /**
     * All resources to free at the end of the frame - limited to Textures and Models
     */
    static ResourceList freedResources;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
