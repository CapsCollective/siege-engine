//
// Created by Jonathan Moallem on 30/11/20.
//

#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include "Entity.h"

class Player : public Entity
{
public:

    // Constructors

    Player() :
    Player(raylib::Vector3::Zero())
    {};

    explicit Player(raylib::Vector3 position) :
    Entity(position)
    {
        // Set default fields
        speed = 1.5;
        velocity = raylib::Vector3::Zero();
    };

protected:

    // Protected overrides

    void onUpdate() override;

    void onDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;
};

#endif //A_DARK_DISCOMFORT_PLAYER_H
