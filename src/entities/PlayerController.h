//
// Created by Jonathan Moallem on 9/12/20.
//

#ifndef A_DARK_DISCOMFORT_PLAYERCONTROLLER_H
#define A_DARK_DISCOMFORT_PLAYERCONTROLLER_H

#include "Entity.h"

class PlayerController : public Entity
{
public:

    // Constructors

    PlayerController();

    // Public methods

    void setPawn(Entity* entity);

protected:

    // Protected overrides

    void onUpdate() override;

    void onDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;

    Entity* pawn;
};


#endif //A_DARK_DISCOMFORT_PLAYERCONTROLLER_H
