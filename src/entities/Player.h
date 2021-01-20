#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include "../systems/entity/Entity.h"
#include "../systems/resource/ModelData.h"
#include "../interfaces/Modelable.h"
#include "../interfaces/Cloneable.h"

class Player : public Entity, public Modelable, public Cloneable
{
public:

    // Constructors

    Player() :
    Player(raylib::Vector3::Zero(), 0.f)
    {};

    Player(raylib::Vector3 position, float rotation) :
    Entity(position, rotation),
    speed(1.5f),
    velocity(raylib::Vector3::Zero()),
    modelData(ModelData(
            "assets/models/cube/cube.obj",
            "assets/models/cube/cube.png"))
    {
        Entity::SetName("Player");
    };

    // Public overrides

    Entity* Clone() override;

    BoundingBox GetBoundingBox() override;

    const ModelData& GetModelData() override;

    void SetModelData(const ModelData& modelData) override;

protected:

    // Protected overrides

    void OnUpdate() override;

    void OnDraw() override;

private:

    // Private fields

    float speed;

    raylib::Vector3 velocity;

    ModelData modelData;
};

#endif //A_DARK_DISCOMFORT_PLAYER_H
