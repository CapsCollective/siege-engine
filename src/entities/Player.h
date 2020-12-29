#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include "Entity.h"
#include "../resources/ModelData.h"

class Player : public Entity
{
public:

    // Constructors

    Player() :
    Player(raylib::Vector3::Zero())
    {};

    explicit Player(raylib::Vector3 position) :
    Entity(position),
    speed(1.5f),
    velocity(raylib::Vector3::Zero())
    {
        Entity::SetName("Player");
        modelData = ModelData(
            "assets/models/cube/cube.obj",
            "assets/models/cube/CubeTexture.png"
        );
    };

    explicit Player(raylib::Vector3 position, const ModelData& data) :
            Entity(position),
            speed(1.5f),
            velocity(raylib::Vector3::Zero())
    {
        Entity::SetName("Player");
        modelData = ModelData(data);
    };

    // Public overrides
    BoundingBox GetBoundingBox() override;

    const ModelData& GetModelData();

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

    void OnUIDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;

    ModelData modelData;
};

#endif //A_DARK_DISCOMFORT_PLAYER_H
