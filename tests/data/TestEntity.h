#ifndef A_DARK_DISCOMFORT_TESTENTITY_H
#define A_DARK_DISCOMFORT_TESTENTITY_H
#include "../src/systems/entity/Entity.h"


class TestEntity : public Entity {

    public:

    TestEntity()
            : Entity(ENTITY_NAME, raylib::Vector3::Zero(), 0.f)
    {};

    TestEntity(raylib::Vector3 position, float rotation)
            : Entity(ENTITY_NAME, position, rotation)
    {};

    static const std::string ENTITY_NAME;
};


#endif //A_DARK_DISCOMFORT_TESTENTITY_H
