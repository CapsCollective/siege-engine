#ifndef A_DARK_DISCOMFORT_TESTENTITY_H
#define A_DARK_DISCOMFORT_TESTENTITY_H
#include <entity/Entity.h>

class TestEntity : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity()
        : TestEntity(Vec3::Zero, 0.f)
    {}

    TestEntity(Vec3 position, float rotation)
        : Entity(ENTITY_NAME, position, rotation)
    {}
};


#endif //A_DARK_DISCOMFORT_TESTENTITY_H
