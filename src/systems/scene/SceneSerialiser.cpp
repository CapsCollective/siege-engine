#include "SceneSerialiser.h"
#include "../../utils/StringHelpers.h"
#include "../entity/Entity.h"
#include "../../entities/Geometry.h"
#include "../../entities/Player.h"
#include "../../entities/SplashScreen.h"
#include "../collision/CollisionRegister.h"
#include <iostream>

// Define macros
#define SEP '|'
#define DEFINE_FIELD(name, content) name + std::string(":") + content + SEP

std::string SceneSerialiser::Serialise(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : entities)
    {
        // TODO make serialisation more modular (on a per-file basis)
        // Add its name, position and rotation to the data
        fileData += (entity->GetName() + SEP +
                     DEFINE_FIELD("POSITION", StringHelpers::VectorToString(entity->GetPosition())) +
                     DEFINE_FIELD("ROTATION", std::to_string(entity->GetRotation())));

        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            auto geometry = dynamic_cast<Geometry*>(entity);
            fileData += DEFINE_FIELD("DIMENSIONS", StringHelpers::VectorToString(geometry->GetDimensions()));
            fileData += DEFINE_FIELD("MODEL_PATH", geometry->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", geometry->GetModelData().GetTexturePath());
        }
        else if (entity->GetName() == "Player")
        {
            auto player = dynamic_cast<Player*>(entity);
            fileData += DEFINE_FIELD("MODEL_PATH", player->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", player->GetModelData().GetTexturePath());
        }

        // Add new line as entity delimiter
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
        for (std::string& arg : args)
        {
            arg = arg.substr(arg.find(':') + 1, arg.size());
        }

        // Get the position and rotation of the entity
        raylib::Vector3 position = StringHelpers::StringToVector(args[ENTITY_POS]);
        float rotation = std::stof(args[ENTITY_ROT]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            // Get custom fields
            raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
            std::string modelPath = args[CUSTOM_FIELD_2];
            std::string texturePath = args[CUSTOM_FIELD_3];

            // Register the new entity
            // TODO move this responsibility elsewhere
            auto geometry = new Geometry(position, rotation, dimensions, ModelData(modelPath, texturePath));
            CollisionRegister::Add(geometry);
            entities.push_back(geometry);
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            // Register the new entity
            entities.push_back(new Player(position, rotation));
        }
        else if (args[ENTITY_NAME] == "SplashScreen")
        {
            // Register the new entity
            entities.push_back(new SplashScreen());
        }
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
    }
}
