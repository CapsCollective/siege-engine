//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include <Vector3.hpp>
#include "../entity_system/IndexAllocator.h"

class Entity {
public:

    // Pure virtual methods

    virtual void onUpdate() = 0;

    virtual void onDraw() = 0;

    const GenerationalIndex &GetIndex() const {
        return index;
    }

    void SetIndex(const GenerationalIndex &index) {
        Entity::index = index;
    }

protected:

    // Protected constructors

    Entity() :
            Entity(raylib::Vector3::Zero()) {};

    explicit Entity(raylib::Vector3 pos) :
            position(pos), index({0, 0}) {};

    // Protected fields

    raylib::Vector3 position;

    GenerationalIndex index;
};

#endif //A_DARK_DISCOMFORT_ENTITY_H
