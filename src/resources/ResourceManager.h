#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <vector>

// Define macros

// A macro which defines what a resource map actually looks like
#define RESOURCE_MAP std::map<std::string, std::variant<Model, Texture2D>>

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

    /**
     * Returns a reference to a resource.
     * @tparam T - the type of resource which needs to be returned.
     * @param path - the path of the resource in the file system.
     * @return - returns a reference to the designated resource.
     */
    template<typename T>
    static T& Get(const std::string& path)
    {
        // Unwraps the variant in the given path
        return std::get<T>(resources.at(path));
    };

    /**
     * Returns a pointer to a resource
     * @tparam T - the type of resource which needs to be returned.
     * @param path - the path of the resource in the file system.
     * @return - returns a pointer to the designated resource.
     */
    template<typename T>
    static T* GetRef(const std::string& path)
    {
        // Upwraps the variant in the given path and returns a pointer.
        return &std::get<T>(resources[path]);
    };

    /**
     * Frees all resources in the resource map.
     */
    static void FreeAllResources();

    /**
     * Adds all resources to a veector which is cleared at the end of the frame.
     */
    static void ClearResources();

private:

    // Private fields

    /**
     * A static map for all our resources.
     */
    static RESOURCE_MAP resources;

    /**
     * A vector of all resources which we want to free at the end of the frame.
     */
    static std::vector<std::variant<Model, Texture2D>*> freedResources;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
