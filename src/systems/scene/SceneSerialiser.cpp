#include "SceneSerialiser.h"
#include "../../utils/StringHelpers.h"
#include "../entity/Entity.h"
#include <iostream>
#include <algorithm>

void SceneSerialiser::RegisterSerialisable(
        const std::string& name,
        const Serialiser& serialise,
        const Deserialiser& deserialise)
{
    GetSerialisables().emplace(name, std::make_pair(serialise, deserialise));
}

std::string SceneSerialiser::Serialise(const std::vector<Entity*>& entities)
{
    auto& serialisables = GetSerialisables();

    // Iterate over each entity in the scene
    std::string fileData;
    for (auto& entity : entities)
    {
        // Only serialise entities that register a serialisable interface
        auto it = serialisables.find(entity->GetName());
        if (it == serialisables.end()) continue;

        // Serialise the general entity information
        fileData += entity->GetName() + SEP;
        fileData += DefineField("POSITION", StringHelpers::VectorToString(entity->GetPosition()));
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

void SceneSerialiser::Deserialise(const std::vector<std::string>& sceneString, OUT std::vector<Entity*>& entities)
{
    // Iterate over each line of the file
    for (const std::string& line : sceneString)
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
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
    }
}