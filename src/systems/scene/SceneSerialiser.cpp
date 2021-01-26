#include "SceneSerialiser.h"
#include "../../utils/StringHelpers.h"
#include "../../entities/Geometry.h"
#include "../../entities/Player.h"
#include "../../entities/SplashScreen.h"
#include <iostream>
#include <algorithm>

// Define macros
#define SEP '|'
#define DEFINE_FIELD(name, content) name + std::string(":") + content + SEP

std::string SceneSerialiser::Serialise(const std::vector<Entity*>& entities)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : entities)
    {
        // Serialise the general entity information
        fileData += entity->GetName() + SEP;
        fileData += DEFINE_FIELD("POSITION", StringHelpers::VectorToString(entity->GetPosition()));
        fileData += DEFINE_FIELD("ROTATION", std::to_string(entity->GetRotation()));
        fileData += DEFINE_FIELD("Z-INDEX", std::to_string(entity->GetZIndex()));

        // Try find any specific instructions for the given entity
        std::string entityName = entity->GetName();
        if (entityName == "Geometry")
        {
            auto geometry = dynamic_cast<Geometry*>(entity);
            fileData += DEFINE_FIELD("DIMENSIONS", StringHelpers::VectorToString(geometry->GetDimensions()));
            fileData += DEFINE_FIELD("MODEL_PATH", geometry->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", geometry->GetModelData().GetTexturePath());
        }
        else if (entityName == "Player")
        {
            auto player = dynamic_cast<Player*>(entity);
            fileData += DEFINE_FIELD("MODEL_PATH", player->GetModelData().GetModelPath());
            fileData += DEFINE_FIELD("TEXTURE_PATH", player->GetModelData().GetTexturePath());
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
        for (std::string& arg : args) arg = arg.substr(arg.find(':') + 1, arg.size());

        // Get standard entity fields
        raylib::Vector3 position = StringHelpers::StringToVector(args[ENTITY_POS]);
        float rotation = std::stof(args[ENTITY_ROT]);
        int zIndex = std::stoi(args[ENTITY_Z_IDX]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            raylib::Vector3 dimensions = StringHelpers::StringToVector(args[CUSTOM_FIELD_1]);
            std::string modelPath = args[CUSTOM_FIELD_2];
            std::string texturePath = args[CUSTOM_FIELD_3];
            entities.push_back(new Geometry(position, rotation, dimensions, ModelData(modelPath, texturePath)));
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            entities.push_back(new Player(position, rotation));
        }
        else if (args[ENTITY_NAME] == "SplashScreen")
        {
            entities.push_back(new SplashScreen());
        }
        else std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
    }
}
