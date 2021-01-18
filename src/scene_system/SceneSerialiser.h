#ifndef A_DARK_DISCOMFORT_SCENESERIALISER_H
#define A_DARK_DISCOMFORT_SCENESERIALISER_H

#include "../utils/SystemMacros.h"
#include <string>
#include <map>

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

    // Private enums

    enum SerialisationFields {
        ENTITY_NAME = 0,
        ENTITY_POS = 1,
        ENTITY_ROT = 2,
        CUSTOM_FIELD_1 = 3,
        CUSTOM_FIELD_2 = 4,
        CUSTOM_FIELD_3 = 5,
    };
};


#endif //A_DARK_DISCOMFORT_SCENESERIALISER_H
