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
    auto it = serialisables.find(entity->GetTypeName());
    if (it == serialisables.end()) return false;

    // Serialise the general entity information
    fileData += DefineField("TYPE", entity->GetTypeName());
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

    std::string str(fileData);
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.cend());
    str.erase(std::remove(str.begin(), str.end(), '\r'), str.cend());
    String sanitisedFileData = str.c_str();

    // Split the file into attributes
    std::map<String, String> attributes;
    for (const String& line : sanitisedFileData.Split(LINE_SEP))
    {
        std::vector<String> attribute = line.Split(NAME_SEP);
        attributes[attribute[0]] = attribute[1];
    }

    // Check if the entity has a relevant serialisable interface registered
    auto& serialisables = GetSerialisables();
    auto it = serialisables.find(attributes["TYPE"]);
    if (it != serialisables.end())
    {
        // Apply its deserialiser
        Deserialiser deserialiser = it->second.second;
        if (deserialiser) return deserialiser(attributes);
    }
    else CC_LOG_WARNING("\"{}\" has no deserialisation protocols defined", attributes["TYPE"]);
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

EntityData SceneFile::GetBaseEntityData(const std::map<String, String>& attributes)
{
    EntityData data;
    auto it = attributes.find("ROTATION");
    if (it == attributes.end() || !it->second.GetFloat(data.rotation))
    {
        CC_LOG_WARNING("Failed to deserialise ROTATION field for entity attributes");
    }

    it = attributes.find("Z-INDEX");
    if (it == attributes.end() || !it->second.GetInt(data.zIndex))
    {
        CC_LOG_WARNING("Failed to deserialise Z-INDEX field for entity attributes");
    }

    it = attributes.find("POSITION");
    if (it == attributes.end() || !FromString(data.position, it->second))
    {
        CC_LOG_WARNING("Failed to deserialise POSITION field for entity attributes");
    }
    return data;
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
    return MakeScenePath(sceneName) + '/' + entity->GetTypeName() + '.' + index + ENTITY_FILE_EXT;
}

String SceneFile::MakeScenePath(const String& sceneName)
{
    return sceneName + SCENE_FILE_EXT;
}
} // namespace Siege