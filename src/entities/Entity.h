//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_ENTITY_H
#define A_DARK_DISCOMFORT_ENTITY_H

#include <raylib-cpp.hpp>

class Entity
{
public:

    // Pure virtual methods

    virtual void onUpdate() = 0;

    virtual void onDraw() = 0;
};


#endif //A_DARK_DISCOMFORT_ENTITY_H
