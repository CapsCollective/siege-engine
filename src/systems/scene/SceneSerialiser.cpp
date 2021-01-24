#include "SceneSerialiser.h"
#include "../../utils/StringHelpers.h"
#include "serialisers/EntitySerialiser.h"
#include "serialisers/PlayerSerialiser.h"
#include "serialisers/GeometrySerialiser.h"
#include "serialisers/SplashScreenSerialiser.h"
#include <iostream>
#include <algorithm>

std::vector<EntitySerialiser*> SceneSerialiser::serialisers {
        new GeometrySerialiser(),
        new PlayerSerialiser(),
        new SplashScreenSerialiser(),
};

std::string SceneSerialiser::Serialise(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : entities)
    {
        // Try find the appropriate serialiser for the given entity
        auto serialiser = TryGetSerialiser(entity->GetName());
        if (serialiser) fileData += serialiser->Serialise(entity) + "\n";
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
        for (std::string& arg : args) arg = arg.substr(arg.find(':') + 1, arg.size());

        // Register entities by entity name
        auto serialiser = TryGetSerialiser(args[ENTITY_NAME]);
        if (serialiser) entities.push_back(serialiser->Deserialise(args));
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
    }
}

EntitySerialiser* SceneSerialiser::TryGetSerialiser(const std::string& entityName)
{
    for (auto& serialiser : serialisers)
    {
        if (serialiser->GetName() == entityName) return serialiser;
    }
    return nullptr;
}
