#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib-cpp.hpp>
#include <map>
#include <variant>
#include <vector>

// Define types
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
        // If the resource already exists then exit the function
        if (resources.find(path) != resources.end()) return;

        // Define an empty variant to hold resource
        std::variant<Model, Texture> resource = std::variant<Model, Texture2D>();

        // Check what type of resource is being registered
        // In this case, check that the resource is a Model
        if (std::is_same<T, Model>::value)
        {
            // If it is a model, load the model and store the struct in the variant
            resource = LoadModel(path.c_str());
        }
        // If the resource is a Texture2D
        else if (std::is_same<T, Texture2D>::value)
        {
            // Load the TEXTURE and store the struct in the variant
            resource = LoadTexture(path.c_str());
        }

        // Add the variant to the resources map
        resources.insert({path, resource});
    };

    /**
     * Returns a reference to a resource
     * @tparam T - the type of resource which needs to be returned
     * @param path - the path of the resource in the file system
     * @return A reference to the designated resource
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
     * @return A pointer to the designated resource
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
     * All resources in the system
     */
    static ResourceMap resources;

    /**
     * All resources to free at the end of the frame
     */
    static ResourceList freedResources;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
