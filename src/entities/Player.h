#ifndef A_DARK_DISCOMFORT_PLAYER_H
#define A_DARK_DISCOMFORT_PLAYER_H

#include <entity/Entity.h>
#include <resource/ModelData.h>
#include <resource/Modelable.h>

class Player : public Entity, public Modelable
{
public:

    // Public constants

    static const std::string ENTITY_NAME;

    // 'Structors

    Player() :
        Player(raylib::Vector3::Zero(), 0.f)
    {};

    Player(raylib::Vector3 position, float rotation) :
        Entity(ENTITY_NAME, position, rotation),
        speed(1.5f),
        velocity(raylib::Vector3::Zero()),
        modelData(ModelData(
                "assets/models/cube/cube.obj",
                "assets/models/cube/cube.png"))
    {};

    // Public overrides

    Entity* Clone() const override;

    BoundingBox GetBoundingBox() const override;

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
