#ifndef A_DARK_DISCOMFORT_RESOURCEMANAGER_H
#define A_DARK_DISCOMFORT_RESOURCEMANAGER_H

#include <raylib/raylib-cpp.hpp>
#include <map>
#include <vector>

class ResourceManager
{
public:

    // Public methods

    /**
     * Loads a resource from a path.
     * @tparam T - the type of resource being added
     * @param path - the path the resource will be loaded from
     */
    template<typename T>
    static void Register(const std::string& path)
    {
        std::string fullPath = baseDir + path;
        if constexpr (std::is_same_v<T, Model>)
        {
            if (models.find(fullPath) != models.end()) return;
            models[fullPath] = LoadModel(fullPath.c_str());
        }
        else if constexpr (std::is_same_v<T, Texture>)
        {
            if (textures.find(fullPath) != textures.end()) return;
            textures[fullPath] = LoadTexture(fullPath.c_str());
        }
    }

    /**
     * Gets a reference to a resource by path.
     * @tparam T - the type of resource which needs to be returned
     * @param path - the path of the resource in the file system
     * @return a reference to the resource
     */
    template<typename T>
    static T& Get(const std::string& path)
    {
        std::string fullPath = baseDir + path;
        if constexpr (std::is_same_v<T, Model>) return models.at(fullPath);
        else if constexpr (std::is_same_v<T, Texture>) return textures.at(fullPath);
    }

    /**
     * Queues all stored resources for removal.
     */
    static void FreeResources();

    /**
     * Immediately clears all stored resources.
     */
    static void ClearResources();

    static void SetBaseDirectory(const std::string& dir);

private:

    // Private fields

    /**
     * Loaded model resources.
     */
    static std::map<std::string, Model> models;

    /**
     * Loaded texture resources.
     */
    static std::map<std::string, Texture> textures;

    /**
     * Model resources queued for freeing.
     */
    static std::vector<Model*> freedModels;

    /**
     * Texture resources queued for freeing.
     */
    static std::vector<Texture*> freedTextures;

    /**
     * The default base directory for finding resources.
     */
    static std::string baseDir;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
