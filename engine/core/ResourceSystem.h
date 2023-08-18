//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_RESOURCESYSTEM_H
#define SIEGE_ENGINE_RESOURCESYSTEM_H

#include <utils/String.h>

#include <map>
#include <vector>

struct Model
{};

struct Texture
{};

namespace Siege
{
class ResourceSystem
{
public:

    // Public methods

    /**
     * Loads a model resource from a path.
     * @param path - the path the resource will be loaded from
     */
    void RegisterModel(const String& path);

    /**
     * Loads a texture resource from a path.
     * @param path - the path the resource will be loaded from
     */
    void RegisterTexture(const String& path);

    /**
     * Gets a reference to a model resource by path.
     * @param path - the path of the resource in the file system
     * @return a reference to the resource
     */
    void* GetModel(const String& path);

    /**
     * Gets a reference to a texture resource by path.
     * @param path - the path of the resource in the file system
     * @return a reference to the resource
     */
    void* GetTexture(const String& path);

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
} // namespace Siege

#endif // SIEGE_ENGINE_RESOURCESYSTEM_H
