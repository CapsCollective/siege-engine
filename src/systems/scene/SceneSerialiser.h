#ifndef A_DARK_DISCOMFORT_SCENESERIALISER_H
#define A_DARK_DISCOMFORT_SCENESERIALISER_H

#include "../../utils/SystemMacros.h"
#include "serialisers/EntitySerialiser.h"
#include <string>
#include <vector>
#include <map>
#include <Vector3.hpp>

class SceneSerialiser
{
public:

    // Public methods

    /**
     * Serialises a list of entities into system the scene
     * file format
     * @param entities - the list of entities to serialise
     * @return The serialised scene as a string
     */
    static std::string Serialise(const std::vector<class Entity*>& entities);

    /**
     * Deserialises a scene file's content into a list of
     * entity pointers
     * @param sceneLines - the lines of the scene file as
     *                     a vector of strings
     * @param entities - a reference to a vector of entity
     *                   pointers to populate
     */
    static void Deserialise(const std::vector<std::string>& sceneLines, OUT std::vector<Entity*>& entities);

private:

    // Private methods

    static EntitySerialiser* TryGetSerialiser(const std::string& entityName);

    // Private fields

    static std::vector<EntitySerialiser*> serialisers;
};


#endif //A_DARK_DISCOMFORT_SCENESERIALISER_H
