#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include "../entity_system/IndexAllocator.h"
#include <raylib-cpp.hpp>

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

    void QueueFree(bool forceFree = false);

    void Free(bool forceFree = false);

    std::string GetName();

    void SetName(std::string entityName);

    void SetFreeable(bool freeable);

    bool IsSerialisable() const;

    void SetSerialisable(bool serialisable);

    void SetPosition(raylib::Vector3 newPosition);

    void SetIsTool(bool isTool);

    bool IsTool();

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
    name("Entity"),
    isSerialisable(true),
    isFreeable(true),
    isTool(false)
    {};

    // Protected fields

    float rotation;

    raylib::Vector3 position;

    GenerationalIndex index;

private:

    // Private fields

    bool isFreeable;

    bool isSerialisable;

    bool isTool;

    std::string name;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
