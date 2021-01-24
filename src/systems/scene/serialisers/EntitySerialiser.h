#ifndef A_DARK_DISCOMFORT_ENTITYSERIALISER_H
#define A_DARK_DISCOMFORT_ENTITYSERIALISER_H

#include <utility>
#include "../../entity/Entity.h"

// Define macros
#define SEP '|'
#define DEFINE_FIELD(name, content) name + std::string(":") + content + SEP

enum SerialisationFields {
    ENTITY_NAME = 0,
    ENTITY_POS = 1,
    ENTITY_ROT = 2,
    ENTITY_Z_IDX = 3,
    CUSTOM_FIELD_1 = 4,
    CUSTOM_FIELD_2 = 5,
    CUSTOM_FIELD_3 = 6,
};

struct EntityData {

    // Constructors

    EntityData(raylib::Vector3 position, float rotation, int zIndex) :
    position(position),
    rotation(rotation),
    zIndex(zIndex)
    {};

    // Public fields

    raylib::Vector3 position;
    float rotation;
    int zIndex;
};

class EntitySerialiser
{
public:

    // Public virtual methods

    virtual std::string Serialise(Entity* entity);

    virtual Entity* Deserialise(const std::vector<std::string>& entityArgs) = 0;

    // Public methods

    static EntityData GetEntityData(const std::vector<std::string>& entityArgs);

    const std::string& GetName();

protected:

    // Constructors

    explicit EntitySerialiser(std::string name) :
            name(name.empty() ? "Entity" : std::move(name))
    {};

private:

    // Private fields

    std::string name;
};


#endif //A_DARK_DISCOMFORT_ENTITYSERIALISER_H
