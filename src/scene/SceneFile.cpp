#include "SceneFile.h"

#include <algorithm>

#include "../entity/Entity.h"
#include "../utils/FileSystem.h"
#include "../utils/Logging.h"

void SceneFile::RegisterSerialisable(const String& name,
                                     const Serialiser& serialise,
                                     const Deserialiser& deserialise)
{
    GetSerialisables().emplace(name, std::make_pair(serialise, deserialise));
}

bool SceneFile::Serialise(const std::vector<Entity*>& entities)
{
    return FileSystem::Save(MakeScenePath(sceneName), SerialiseToString(entities));
}

String SceneFile::SerialiseToString(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    String fileData;
    auto& serialisables = GetSerialisables();
    for (auto& entity : entities)
    {
        // Only serialise entities that register a serialisable interface
        auto it = serialisables.find(entity->GetName());
        if (it == serialisables.end()) continue;

        // Serialise the general entity information
        fileData += (entity->GetName() + SEP);
        fileData += DefineField("POSITION", entity->GetPosition().ToString());
        fileData += DefineField("ROTATION", String::FromFloat(entity->GetRotation()));
        fileData += DefineField("Z-INDEX", String::FromInt(entity->GetZIndex()));

        // Apply its serialiser if it
        Serialiser serialiser = it->second.first;
        if (serialiser) fileData += serialiser(entity);

        // End the serialisation entry
        fileData += "\n";
    }
    return fileData;
}

bool SceneFile::Deserialise(std::vector<Entity*>& entities)
{
    // Iterate over each line of the file
    String lines = FileSystem::Read(MakeScenePath(sceneName));
    if (!lines) return false;
    DeserialiseLines(lines.Split('\n'), entities);
    return true;
}

void SceneFile::DeserialiseLines(const std::vector<String>& lines, std::vector<Entity*>& entities)
{
    for (const String& line : lines)
    {
        // Split the line into arguments and strip the labels from each item
        std::vector<String> args = line.Split(SEP);
        for (String& arg : args) arg = arg.SubString((int) arg.Find(NAME_SEP) + 1);

        // Get the standard entity fields
        EntityData data;
        if (!(args.size() >= 4 && args[ENTITY_ROT].GetFloat(data.rotation) &&
              args[ENTITY_Z_IDX].GetInt(data.zIndex) &&
              Vec3::FromString(data.position, args[ENTITY_POS])))
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
            if (deserialiser) entities.push_back(deserialiser(data, args));
        }
        else CC_LOG_WARNING("\"{}\" has no deserialisation protocols defined", args[ENTITY_NAME]);
    }
}

String SceneFile::MakeScenePath(const String& sceneName)
{
    return ResourceManagerFacade::GetBaseDirectory() + sceneName + SCENE_FILE_EXT;
}