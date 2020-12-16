#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include <string>
#include <Vector3.hpp>
#include "../entity_system/IndexAllocator.h"

class Entity 
{
public:

    // Virtual methods

    virtual void OnUpdate() {};

    virtual void OnDraw() {};

    virtual void OnUIDraw() {};

    virtual void QueueFree();

    virtual void Free();

    virtual BoundingBox GetBoundingBox();

    // Public methods

    std::string GetName();

    void SetName(std::string entityName);

    void SetPosition(raylib::Vector3 newPosition);

    raylib::Vector3 GetPosition();

    const GenerationalIndex& GetIndex() const {
        return index;
    }

    void SetIndex(GenerationalIndex idx) {
        index = idx;
    }

protected:

    // Protected constructors

    Entity() :
    Entity(raylib::Vector3::Zero())
    {};

    explicit Entity(raylib::Vector3 position) :
    position(position),
    index({0, 0}),
    name("Entity")
    {};

    // Protected fields

    raylib::Vector3 position;

    GenerationalIndex index;

private:

    // Private fields

    std::string name;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
