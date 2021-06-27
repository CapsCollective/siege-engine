#ifndef A_DARK_DISCOMFORT_SCENESERIALISER_H
#define A_DARK_DISCOMFORT_SCENESERIALISER_H

#include "../../utils/Macros.h"
#include <Vector3.hpp>
#include <functional>
#include <string>
#include <vector>
#include <map>

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

/**
 * An enum for common serialisation field names
 */
enum SerialisationFields {
    ENTITY_NAME = 0,
    ENTITY_POS = 1,
    ENTITY_ROT = 2,
    ENTITY_Z_IDX = 3,
    CUSTOM_FIELD_1 = 4,
    CUSTOM_FIELD_2 = 5,
    CUSTOM_FIELD_3 = 6,
};

class SceneSerialiser
{
public:

    // Public methods

    /**
     * Registers a serialisation interface for an entity
     * @param name - the name of the serialisable entity
     * @param serialise - the serialise function for the entity
     * @param deserialise - the deserialise function for the entity
     */
    static void RegisterSerialisable(
            const std::string& name,
            const Serialiser& serialise,
            const Deserialiser& deserialise);

    /**
     * Serialises a list of entities into system the scene
     * file format
     * @param entities - the list of entities to serialise
     * @return the serialised scene as a string
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

    /**
     * Getter & storage for mapped serialisation interfaces
     * @return a map of string to serialisation interfaces
     */
    static std::map<std::string, SerialisationInterface>& GetSerialisables()
    {
        static std::map<std::string, SerialisationInterface> serialisables;
        return serialisables;
    }
};

/**
 * A container struct for basic entity field data
 */
struct EntityData {

    // Public members

    /**
     * The entity position
     */
    raylib::Vector3 position;

    /**
     * The entityrotation
     */
    float rotation;

    /**
     * The entity z-index
     */
    int zIndex;
};

/**
 * An empty struct for static member registration of
 * serialisable interfaces
 */
struct SerialisationInterfaceRegisterer
{

    // 'Structors

    /**
     * Constructor for serialisation interface registration
     * that registers provided serialisation interfaces at
     * initialisation time
     * @note This is supposedly 1B in size per-instantiation
     * @param name - the name of the serialisable entity
     * @param serialise - the serialise function for the entity
     * @param deserialise - the deserialise function for the entity
     */
    SerialisationInterfaceRegisterer(
            const std::string& name,
            const Serialiser& serialise,
            const Deserialiser& deserialise)
    {
        SceneSerialiser::RegisterSerialisable(name, serialise, deserialise);
    }
};

// Free functions

/**
 * An inlined helper function to define the structure of the
 * property field in a .scene file
 * @param name - the name of the property field
 * @param content - the content of the property
 * @return the field content as a const string
 */
inline const std::string DefineField(const std::string& name, const std::string& content)
{
    return name + NAME_SEP + content + SEP;
}

#endif //A_DARK_DISCOMFORT_SCENESERIALISER_H
