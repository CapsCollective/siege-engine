#include "SceneFile.h"
#include "../utils/StringHelpers.h"
#include "../entity/Entity.h"
#include "../utils/Logging.h"
#include <iostream>
#include <algorithm>
#include <fstream>

void SceneFile::RegisterSerialisable(
        const std::string& name,
        const Serialiser& serialise,
        const Deserialiser& deserialise)
{
    GetSerialisables().emplace(name, std::make_pair(serialise, deserialise));
}

bool SceneFile::Serialise(const std::vector<Entity*>& entities)
{
    // Open a new file stream, serialise the data to it and close it
    std::ofstream fileStream(MakeScenePath(sceneName));
    fileStream << SerialiseToString(entities);
    fileStream.close();
    return true;
}

std::string SceneFile::SerialiseToString(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    std::string fileData;
    auto& serialisables = GetSerialisables();
    for (auto& entity : entities)
    {
        // Only serialise entities that register a serialisable interface
        auto it = serialisables.find(entity->GetName());
        if (it == serialisables.end()) continue;

        // Serialise the general entity information
        fileData += entity->GetName() + SEP;
        fileData += DefineField("POSITION", entity->GetPosition().ToString());
        fileData += DefineField("ROTATION", std::to_string(entity->GetRotation()));
        fileData += DefineField("Z-INDEX", std::to_string(entity->GetZIndex()));

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
    // Try open the scene file for streaming
    std::ifstream file(MakeScenePath(sceneName));

    // Exit if scene is invalid
    if (!file.is_open()) return false;

    // Iterate over each line of the file
    std::string line;
    while (std::getline(file, line)) DeserialiseLine(line, entities);

    // Close the file stream
    file.close();
    return true;
}

void SceneFile::DeserialiseFromStrings(const std::vector<std::string>& lines, std::vector<Entity*>& entities)
{
    for (const std::string& line : lines) DeserialiseLine(line, entities);
}

void SceneFile::DeserialiseLine(const std::string& line, std::vector<Entity*>& entities)
{
    // Split the line into arguments and strip the labels from each item
    std::vector<std::string> args = StringHelpers::SplitString(line, SEP);
    for (std::string& arg : args) arg = arg.substr(arg.find(NAME_SEP) + 1, arg.size());

    // Get standard entity fields
    EntityData data = {
            StringHelpers::StringToVector(args[ENTITY_POS]),
            std::stof(args[ENTITY_ROT]),
            std::stoi(args[ENTITY_Z_IDX])
    };

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

std::string SceneFile::MakeScenePath(const std::string& sceneName)
{
    return ResourceManagerFacade::GetBaseDirectory() + sceneName + SCENE_FILE_EXT;
}