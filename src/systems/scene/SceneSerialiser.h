#ifndef A_DARK_DISCOMFORT_SCENESERIALISER_H
#define A_DARK_DISCOMFORT_SCENESERIALISER_H

#include "../../utils/SystemMacros.h"
#include <string>
#include <vector>
#include <map>
#include <Vector3.hpp>

// Define macros
#define REGISTER_SERIALISATION_INTERFACE(name, serialiser, deserialiser) \
    static SerialisationInterfaceRegisterer CONCAT_SYMBOL(_si_reg_, __LINE__)(name, serialiser, deserialiser)

// Define constants
static constexpr const char SEP = '|';
static constexpr const char NAME_SEP = ':';

// Define types
typedef const std::function<std::string(class Entity*)> Serialiser;
typedef const std::function<class Entity*(const struct EntityData&, const std::vector<std::string>&)> Deserialiser;
typedef std::pair<Serialiser, Deserialiser> SerialisationInterface;

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

    // 'Structors

    EntityData(raylib::Vector3 position, float rotation, int zIndex) :
            position(position),
            rotation(rotation),
            zIndex(zIndex) {};

    // Public members

    raylib::Vector3 position;

    float rotation;

    int zIndex;
};

class SceneSerialiser
{
public:

    // Public methods

    static void RegisterSerialisable(
            const std::string& name, const Serialiser& serialise, const Deserialiser& deserialise);

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

    static std::map<std::string, SerialisationInterface>& GetSerialisables()
    {
        static std::map<std::string, SerialisationInterface> serialisables;
        return serialisables;
    }
};

struct SerialisationInterfaceRegisterer
{
    SerialisationInterfaceRegisterer(
            const std::string& name, const Serialiser& serialise, const Deserialiser& deserialise)
    {
        SceneSerialiser::RegisterSerialisable(name, serialise, deserialise);
    }
};

// Free functions

inline const std::string DefineField(const std::string& name, const std::string& content)
{
    return name + NAME_SEP + content + SEP;
}

#endif //A_DARK_DISCOMFORT_SCENESERIALISER_H
