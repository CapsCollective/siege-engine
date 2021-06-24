#include "SceneSerialiser.h"
#include "../../utils/StringHelpers.h"
#include "../entity/Entity.h"
#include <iostream>
#include <algorithm>

// Static member initialisation
std::map<std::string, SerialisationInterface> SceneSerialiser::serialisables;


void SceneSerialiser::RegisterSerialisable(const std::string& name, Serialiser* serialise, Deserialiser* deserialise)
{
    serialisables.emplace(name, std::make_pair(serialise, deserialise));
}

std::string SceneSerialiser::Serialise(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto& entity : entities)
    {
        // Serialise the general entity information
        fileData += entity->GetName() + SEP;
        fileData += DefineField("POSITION", StringHelpers::VectorToString(entity->GetPosition()));
        fileData += DefineField("ROTATION", std::to_string(entity->GetRotation()));
        fileData += DefineField("Z-INDEX", std::to_string(entity->GetZIndex()));

        // TODO Try find any specific instructions for the given entity
        auto it = serialisables.find(entity->GetName());
        if (it != serialisables.end())
        {
            Serialiser* serialiser = it->second.first;
            if (serialiser) fileData += serialiser(entity);
        }

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
        // Split the line into arguments
        std::vector<std::string> args = StringHelpers::SplitString(line, SEP);

        // Strip labels from each item
        for (std::string& arg : args) arg = arg.substr(arg.find(NAME_SEP) + 1, arg.size());

        // Get standard entity fields
        EntityData data(
                StringHelpers::StringToVector(args[ENTITY_POS]),
                std::stof(args[ENTITY_ROT]),
                std::stoi(args[ENTITY_Z_IDX]));

        auto it = serialisables.find(args[ENTITY_NAME]);
        if (it != serialisables.end())
        {
            Deserialiser* deserialiser = it->second.second;
            if (deserialiser) entities.push_back(deserialiser(data, args));
        }
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
    }
}