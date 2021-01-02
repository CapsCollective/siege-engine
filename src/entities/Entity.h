#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "../entity_system/IndexAllocator.h"
#include <raylib-cpp.hpp>

// TODO comment API functions
class Entity
{
public:

    // Virtual methods

    virtual void OnUpdate() {};

    virtual void OnDraw() {};

    virtual void OnUIDraw() {};

    virtual Entity* Clone();

    virtual BoundingBox GetBoundingBox();

    // Public methods

    void QueueFree();

    void Free();

    std::string GetName();

    void SetName(std::string entityName);

    void SetPosition(raylib::Vector3 newPosition);

    raylib::Vector3 GetPosition();

    void SetRotation(float newRotation);

    float GetRotation() const;

    const GenerationalIndex& GetIndex() const {
        return index;
    }

    void SetIndex(GenerationalIndex idx) {
        index = idx;
    }

protected:

    // Protected constructors

    Entity() :
    Entity(raylib::Vector3::Zero(), 0.f)
    {};

    explicit Entity(raylib::Vector3 position, float rotation) :
    rotation(rotation),
    position(position),
    index({0, 0}),
    name("Entity")
    {};

    // Protected fields

    float rotation;

    raylib::Vector3 position;

    GenerationalIndex index;

private:

    // Private fields

    std::string name;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
