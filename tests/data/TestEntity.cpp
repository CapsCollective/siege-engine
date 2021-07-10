#include "TestEntity.h"
#include "../../src/engine/scene/SceneSerialiser.h"

const std::string TestEntity::ENTITY_NAME("TestEntity");

static std::string Serialise(Entity* entity)
{
    return "";
}

static Entity* Deserialise(const EntityData& data, const std::vector<std::string>& args)
{
    return new TestEntity(data.position, data.rotation);
}

REGISTER_SERIALISATION_INTERFACE(TestEntity::ENTITY_NAME, Serialise, Deserialise);


