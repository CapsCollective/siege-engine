#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib/raylib-cpp.hpp>
#include <map>
#include <vector>

class ResourceManager
{
public:

    // Public functions

    /**
     * Loads a resource from a path and adds it to the resource map
     * @param path - the path the resource will be loaded from
     */
    template<typename T>
    static void Register(const std::string& path)
    {
        if constexpr (std::is_same_v<T, Model>)
        {
            if (models.find(path) != models.end()) return;
            models[path] = LoadModel(path.c_str());
        }
        else if constexpr (std::is_same_v<T, Texture>)
        {
            if (textures.find(path) != textures.end()) return;
            textures[path] = LoadTexture(path.c_str());
        }
    }

    /**
     * Returns a reference to a resource
     * @param path - the path of the resource in the file system
     * @return a reference to the designated resource
     */
    template<typename T>
    static T& Get(const std::string& path)
    {
        if constexpr (std::is_same_v<T, Model>) return models.at(path);
        else if constexpr (std::is_same_v<T, Texture>) return textures.at(path);
    }

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
    static std::map<std::string, Model> models;
    static std::map<std::string, Texture> textures;

    /**
     * All resources to free at the end of the frame
     */
    static std::vector<Model*> freedModels;
    static std::vector<Texture*> freedTextures;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
