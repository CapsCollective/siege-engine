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
    static void RegisterModel(const std::string& path);

    static void RegisterTexture(const std::string& path);

    /**
     * Returns a reference to a resource
     * @param path - the path of the resource in the file system
     * @return a reference to the designated resource
     */
    static Model& GetModel(const std::string& path);

    static Texture& GetTexture(const std::string& path);

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
    static std::map<std::string, Model> models;
    static std::map<std::string, Texture> textures;

    /**
     * All resources to free at the end of the frame - limited to textures and models
     */
    static std::vector<Model*> freedModels;
    static std::vector<Texture*> freedTextures;
};

#endif //A_DARK_DISCOMFORT_RESOURCEMANAGER_H
