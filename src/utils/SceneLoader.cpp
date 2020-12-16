#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include "SceneLoader.h"
#include "../entity_system/EntityStorage.h"
#include "../entities/Geometry.h"
#include "../entities/Player.h"

void SceneLoader::SaveScene(const std::string& sceneName)
{
    // Iterate over each entity in the scene
    std::string fileData;
    for (auto entity : EntityStorage::GetEntities())
    {
        // Add it's name and position to the data
        fileData += (entity->GetName() + ";" + VectorToString(entity->GetPosition()) + ";");

        // Add any additional fields needed to the data
        if (entity->GetName() == "Geometry")
        {
            fileData += VectorToString(dynamic_cast<Geometry*>(entity)->GetDimensions()) + ";";
        }

        // Add new line as entity delimiter
        fileData += "\n";
    }

    // Open a new file stream, dave the data to it and close it
    std::ofstream fileStream("./assets/scenes/" + sceneName + ".scene");
    fileStream << fileData;
    fileStream.close();
}

void SceneLoader::LoadScene(const std::string& sceneName)
{
    //TODO remove debug output in this method
    std::cout << "Loading " + sceneName + "..." << std::endl;

    // Begin the loading process, open the file for streaming
    std::ifstream file("./assets/scenes/" + sceneName + ".scene");
    if (!file.is_open()) return;

    // Iterate over each line of the file
    std::string line;
    while (getline(file, line))
    {
        //TODO remove this debug line
        std::cout << line << std::endl;

        // Define default field values
        enum {
            ENTITY_NAME = 0,
            ENTITY_POS = 1,
            CUSTOM_FIELD_1 = 2,
            CUSTOM_FIELD_2 = 3,
            CUSTOM_FIELD_3 = 4,
        };

        // Split the line into arguments
        std::vector<std::string> args = SplitString(line, ';');

        // Calculate the position of the entity
        raylib::Vector3 position = StringToVector(args[ENTITY_POS]);

        // Register entities by entity name
        if (args[ENTITY_NAME] == "Geometry")
        {
            raylib::Vector3 dimensions = StringToVector(args[CUSTOM_FIELD_1]);
            EntityStorage::Register(new Geometry(position, dimensions));
        }
        else if (args[ENTITY_NAME] == "Player")
        {
            EntityStorage::Instance()->Register(new Player(position));
        }
        else
        {
            // Notify console of default case
            std::cout << "\"" << args[ENTITY_NAME] << "\" has no deserialisation protocols defined" << std::endl;
        }
    }

    // Close the file stream
    file.close();
}

std::string SceneLoader::VectorToString(raylib::Vector3 vector)
{
    return std::to_string(vector.x) + "," +
           std::to_string(vector.y) + "," +
           std::to_string(vector.z) + ",";
}

raylib::Vector3 SceneLoader::StringToVector(std::string string)
{
    std::vector<std::string> components = SplitString(std::move(string), ',');
    return {
        std::stof(components[0]),
        std::stof(components[1]),
        std::stof(components[2]),
    };
}

std::vector<std::string> SceneLoader::SplitString(std::string string, char delimiter)
{
    // Iterate over the string while there is still a delimiter
    int delimiterPos;
    std::vector<std::string> args;
    while ((delimiterPos = string.find(delimiter)) != std::string::npos)
    {
        // Get up to the next delimiter, add it to the vector, and erase it
        args.push_back(string.substr(0, delimiterPos));
        string.erase(0, args.back().size()+1);
    }
    return args;
}

