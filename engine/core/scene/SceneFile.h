//
// Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
//
// This code is released under an unmodified zlib license.
// For conditions of distribution and use, please see:
//     https://opensource.org/licenses/Zlib
//

#ifndef SIEGE_ENGINE_SCENEFILE_H
#define SIEGE_ENGINE_SCENEFILE_H

#include <utils/Macros.h>
#include <utils/Maths.h>
#include <utils/String.h>

#include <functional>
#include <map>
#include <utility>
#include <vector>

// Define macros
#define REGISTER_SERIALISATION_INTERFACE(name, serialiser, deserialiser)                  \
    static SerialisationInterfaceRegisterer CONCAT_SYMBOL(_si_reg_, __LINE__)(name,       \
                                                                              serialiser, \
                                                                              deserialiser)

// Define constants
static constexpr const char SEP = '|';
static constexpr const char NAME_SEP = ':';
static constexpr const char* SCENE_FILE_EXT = ".scene";

// Define types
typedef struct EntityData EntityData;
typedef const std::function<String(class Entity*)> Serialiser;
typedef const std::function<Entity*(const EntityData&, const std::vector<String>&)> Deserialiser;
typedef std::pair<Serialiser, Deserialiser> SerialisationInterface;

/**
 * An enum for common serialisation field names
 */
enum SerialisationFields
{
    ENTITY_NAME = 0,
    ENTITY_POS = 1,
    ENTITY_ROT = 2,
    ENTITY_Z_IDX = 3,
    CUSTOM_FIELD_1 = 4,
    CUSTOM_FIELD_2 = 5,
    CUSTOM_FIELD_3 = 6,
};

class SceneFile
{
public:

    // 'Structors

    /**
     * Scene files must specify a filename
     */
    SceneFile() = delete;

    /**
     * Scene file constructor method
     * @param sceneName - the name of the scene file
     */
    explicit SceneFile(String sceneName) : sceneName(std::move(sceneName)) {}

    // Public methods

    /**
     * Registers a serialisation interface for an entity
     * @param name - the name of the serialisable entity
     * @param serialise - the serialise function for the entity
     * @param deserialise - the deserialise function for the entity
     */
    static void RegisterSerialisable(const String& name,
                                     const Serialiser& serialise,
                                     const Deserialiser& deserialise);

    /**
     * Serialises a list of entities into the scene file format
     * @param entities - the list of entities to serialise
     * @return true if the operation was successful
     */
    bool Serialise(const std::vector<class Entity*>& entities);

    /**
     * Serialises a list of entities into the scene file format as
     * a string
     * @param entities - the list of entities to serialise
     * @return a string representation of the scene
     */
    static String SerialiseToString(const std::vector<Entity*>& entities);

    /**
     * Deserialises a scene file's content into a list of
     * entity pointers
     * @param entities - a reference to a vector of entity
     *                   pointers to populate
     * @return true if the operation was successful
     */
    bool Deserialise(OUT std::vector<Entity*>& entities);

    /**
     * Deserialises a scene file's content from a vector
     * of strings
     * @param lines - strings for each line of the scene file
     * @param entities - a reference to a vector of entity
     *                   pointers to populate
     */
    static void DeserialiseLines(const std::vector<String>& lines,
                                 OUT std::vector<Entity*>& entities);

private:

    // Private methods

    /**
     * Creates a filepath to the specified scene relative to
     * the base resource directory
     * @param sceneName - the name of the scene
     * @return a filepath to a scene file
     */
    static String MakeScenePath(const String& sceneName);

    /**
     * Getter & storage for mapped serialisation interfaces
     * @return a map of string to serialisation interfaces
     */
    static std::map<String, SerialisationInterface>& GetSerialisables()
    {
        static std::map<String, SerialisationInterface> serialisables;
        return serialisables;
    }

    // Private fields

    /**
     * The name of the scene file
     */
    String sceneName;
};

/**
 * A container struct for basic entity field data
 */
struct EntityData
{
    // Public members

    /**
     * The entity position
     */
    Vec3 position;

    /**
     * The entity rotation
     */
    float rotation {0};

    /**
     * The entity z-index
     */
    int zIndex {0};
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
    SerialisationInterfaceRegisterer(const String& name,
                                     const Serialiser& serialise,
                                     const Deserialiser& deserialise)
    {
        SceneFile::RegisterSerialisable(name, serialise, deserialise);
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
inline String DefineField(const String& name, const String& content)
{
    return name + NAME_SEP + content + SEP;
}

#endif // SIEGE_ENGINE_SCENEFILE_H
