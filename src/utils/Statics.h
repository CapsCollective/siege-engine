#ifndef A_DARK_DISCOMFORT_STATICS_H
#define A_DARK_DISCOMFORT_STATICS_H

#include "../input/Input.h"
#include "../physics/CollisionSystem.h"

class Statics
{
public:

#ifndef CC_IGNORE_STATICS
    static CollisionSystem CollisionSystem;

    static Input Input;
#endif
};

#endif // A_DARK_DISCOMFORT_STATICS_H
