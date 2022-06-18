#ifndef SIEGE_ENGINE_GEOMETRY_H
#define SIEGE_ENGINE_GEOMETRY_H

#include <core/entity/Entity.h>
#include <core/render/RenderSystem.h>

#include <utility>

class Geometry : public Entity
{
public:

    // Public constants

    static const String ENTITY_NAME;

    // 'Structors

    Geometry() : Geometry({Vec3::Zero, 0.f}) {};

    explicit Geometry(const Xform& transform) :
        Geometry(transform, "models/cube/cube.obj", "models/cube/cube.png") {};

    Geometry(const Xform& transform, String modelPath, String texturePath) :
        Entity(ENTITY_NAME, transform),
        modelPath(std::move(modelPath)),
        texturePath(std::move(texturePath)) {};

    // Public overrides

    Entity* Clone() const override;

    BoundedBox GetBoundingBox() const override;

    ModelData GetModelData();

    // Public methods

    const Vec3& GetDimensions();

protected:

    // Protected overrides

    void OnStart() override;

    void OnDestroy() override;

private:

    // Private fields

    String modelPath, texturePath;
};

#endif // SIEGE_ENGINE_GEOMETRY_H
