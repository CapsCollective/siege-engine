//
// Copyright (c) 2020-present Caps Collective & contributors
// Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#include "SceneFile.h"

#include <resources/PackFile.h>
#include <resources/ResourceSystem.h>
#include <resources/SceneData.h>
#include <utils/FileSystem.h>
#include <utils/Logging.h>

#include <algorithm>

#include "SceneSystem.h"
#include "resources/GenericFileData.h"

namespace Siege
{
void SceneFile::RegisterSerialisable(const String& name,
                                     const Serialiser& serialise,
                                     const Deserialiser& deserialise)
{
    GetSerialisables().emplace(name, std::make_pair(serialise, deserialise));
}

bool SceneFile::Serialise(const std::vector<Entity*>& entities)
{
    // Create the required scene file directory
    FileSystem::CreateDirectoryRecursive(MakeScenePath(sceneName));

    // Flush any removed entities from disk
    for (const auto& pair : entityPaths)
    {
        if (!pair.first) FileSystem::Remove(pair.second);
    }

    bool succeeded = true;
    for (auto& entity : entities)
    {
        String fileData;
        bool result = SerialiseToString(entity, fileData);

        if (!result)
        {
            succeeded = false;
            continue;
        }

        String filepath = GetOrCreateEntityFilepath(entity);
        FileSystem::Save(filepath, fileData);
        entityPaths[EntityPtr(entity)] = filepath;
    }
    return succeeded;
}

bool SceneFile::SerialiseToString(Entity* entity, String& fileData)
{
    // Iterate over each entity in the scene
    auto& serialisables = GetSerialisables();

    // Only serialise entities that register a serialisable interface
    auto it = serialisables.find(entity->GetName());
    if (it == serialisables.end()) return false;

    // Serialise the general entity information
    fileData += (entity->GetName() + LINE_SEP);
    fileData += DefineField("POSITION", ToString(entity->GetPosition()));
    fileData += DefineField("ROTATION", String::FromFloat(entity->GetRotation().y));
    fileData += DefineField("Z-INDEX", String::FromInt(entity->GetZIndex()));

    // Apply its serialiser if it
    Serialiser serialiser = it->second.first;
    if (serialiser) fileData += serialiser(entity);

    return true;
}

bool SceneFile::Deserialise(std::vector<Entity*>& entities)
{
    // Clear out the held entity paths before repopulating
    entityPaths.clear();

    bool succeeded = true;
    auto deserialiseEntityString =
        [this, &entities, &succeeded](const String& entityData, const std::filesystem::path& path) {
            Entity* newEntity = DeserialiseFromString(entityData);

            if (!newEntity)
            {
                succeeded = false;
                return;
            }

            entities.push_back(newEntity);
            entityPaths[EntityPtr(newEntity)] = path.c_str();
        };

    // TODO: Implement proper write-mode handling for scene system
    String ScenePath = MakeScenePath(sceneName);
    if (!SceneSystem::GetBaseDirectory().IsEmpty())
    {
        bool result = FileSystem::ForEachFileInDir(
            ScenePath,
            [&deserialiseEntityString](const std::filesystem::path& path) {
                if (path.extension() != ENTITY_FILE_EXT) return;
                String entityData = FileSystem::Read(path.c_str());
                deserialiseEntityString(entityData, path);
            });
        if (!result)
        {
            CC_LOG_ERROR("Failed to read scene file at path \"{}\"", ScenePath)
            return false;
        }
    }
    else
    {
        PackFile* packFile = ResourceSystem::GetInstance().GetPackFile();

        SceneData* sceneData = packFile->FindData<SceneData>(ScenePath);
        if (!sceneData)
        {
            CC_LOG_WARNING("Failed to find scene \"{}\" in pack file", ScenePath)
            return false;
        }

        String sceneString(sceneData->data);
        for (const String& entityData : sceneString.Split('|'))
        {
            deserialiseEntityString(entityData.Str(), "");
        }
    }

    return succeeded;
}

Entity* SceneFile::DeserialiseFromString(const String& fileData)
{
    if (fileData.IsEmpty())
    {
        CC_LOG_WARNING("Found empty entity during deserialisation");
        return nullptr;
    }

    // Split the file into arguments and strip the labels from each item
    std::vector<String> args = fileData.Split(LINE_SEP);
    for (String& arg : args) arg = arg.SubString((int) arg.Find(NAME_SEP) + 1);

    // Get the standard entity fields
    EntityData data;
    if (!(args.size() >= 4 && args[ENTITY_ROT].GetFloat(data.rotation) &&
          args[ENTITY_Z_IDX].GetInt(data.zIndex) && FromString(data.position, args[ENTITY_POS])))
    {
        CC_LOG_WARNING("Failed to deserialise fields for entity \"{}\"", args[ENTITY_NAME]);
    }

    // Check if the entity has a relevant serialisable interface registered
    auto& serialisables = GetSerialisables();
    auto it = serialisables.find(args[ENTITY_NAME]);
    if (it != serialisables.end())
    {
        // Apply its deserialiser
        Deserialiser deserialiser = it->second.second;
        if (deserialiser) return deserialiser(data, args);
    }
    else CC_LOG_WARNING("\"{}\" has no deserialisation protocols defined", args[ENTITY_NAME]);
    return nullptr;
}

const String& SceneFile::GetSceneName()
{
    return sceneName;
}

void SceneFile::SetSceneName(const String& name)
{
    sceneName = name;
}

void SceneFile::InitialiseEntityPathMappings()
{
    for (std::pair<EntityPtr<Entity>, String> pair : entityPaths)
    {
        pair.first.InitialiseIndex();
    }
}

String SceneFile::GetOrCreateEntityFilepath(Entity* entity)
{
    // Try to find the entity path amongst the deserialised
    auto it = entityPaths.find(EntityPtr(entity));
    if (it != entityPaths.end()) return it->second;

    // Search through the scene file for the next available file index
    int newFileIndex = 1;
    auto findNextFileIndex = [&newFileIndex](const std::filesystem::path& path) {
        if (path.extension() != ENTITY_FILE_EXT) return;
        std::vector<String> filename = String(path.filename().c_str()).Split('.');

        int nameIndex;
        if (filename[1].GetInt(nameIndex) && nameIndex >= newFileIndex)
        {
            newFileIndex = nameIndex + 1;
        }
    };
    bool result = FileSystem::ForEachFileInDir(MakeScenePath(sceneName), findNextFileIndex);

    // Failed attempts to find a file index are serialised as 0
    String index = result ? String::FromInt(newFileIndex) : "0";
    return MakeScenePath(sceneName) + '/' + entity->GetName() + '.' + index + ENTITY_FILE_EXT;
}

String SceneFile::MakeScenePath(const String& sceneName)
{
    return sceneName + SCENE_FILE_EXT;
}
} // namespace Siege