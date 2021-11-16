#ifndef A_DARK_DISCOMFORT_TESTENTITY_H
#define A_DARK_DISCOMFORT_TESTENTITY_H

#include <entity/Entity.h>

class TestEntity : public Entity
{
public:

    static const std::string ENTITY_NAME;

    TestEntity()
        : TestEntity(Xform())
    {}

    TestEntity(Xform transform)
        : Entity(ENTITY_NAME, transform)
    {}
};


#endif //A_DARK_DISCOMFORT_TESTENTITY_H
