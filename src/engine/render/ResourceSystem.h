#ifndef A_DARK_DISCOMFORT_RESOURCESYSTEM_H
#define A_DARK_DISCOMFORT_RESOURCESYSTEM_H

#include <utils/String.h>

#include <map>
#include <raylib/raylib-cpp.hpp>
#include <vector>

class ResourceSystem
{
public:

    // Public methods

    /**
     * Loads a resource from a path.
     * @tparam T - the type of resource being added
     * @param path - the path the resource will be loaded from
     */
    template<typename T>
    void Register(const String& path)
    {
        String fullPath = baseDir + path;
        if constexpr (std::is_same_v<T, Model>)
        {
            if (models.find(fullPath) != models.end()) return;
            models[fullPath] = LoadModel(fullPath);
        }
        else if constexpr (std::is_same_v<T, Texture>)
        {
            if (textures.find(fullPath) != textures.end()) return;
            textures[fullPath] = LoadTexture(fullPath);
        }
    }

    /**
     * Gets a reference to a resource by path.
     * @tparam T - the type of resource which needs to be returned
     * @param path - the path of the resource in the file system
     * @return a reference to the resource
     */
    template<typename T>
    T& Get(const String& path)
    {
        String fullPath = baseDir + path;
        if constexpr (std::is_same_v<T, Model>) return models.at(fullPath);
        else if constexpr (std::is_same_v<T, Texture>) return textures.at(fullPath);
    }

    /**
     * Queues all stored resources for removal.
     */
    void FreeResources();

    /**
     * Immediately clears all stored resources.
     */
    void ClearResources();

    /**
     * Sets the base directory used for resource path management.
     * @param dir - the directory to set as base
     */
    void SetBaseDirectory(const String& dir);

    /**
     * Gets the base directory used for resource path management.
     * @return the directory as a string
     */
    const String& GetBaseDirectory();

private:

    // Private fields

    /**
     * Loaded model resources.
     */
    std::map<String, Model> models;

    /**
     * Loaded texture resources.
     */
    std::map<String, Texture> textures;

    /**
     * Model resources queued for freeing.
     */
    std::vector<Model*> freedModels;

    /**
     * Texture resources queued for freeing.
     */
    std::vector<Texture*> freedTextures;

    /**
     * The default base directory for finding resources.
     */
    String baseDir;
};

#endif // A_DARK_DISCOMFORT_RESOURCESYSTEM_H
