#ifndef SIEGE_ENGINE_STATICS_H
#define SIEGE_ENGINE_STATICS_H

class Statics
{
public:

    static class CollisionSystem& Collision();

    static class InputSystem& Input();

    static class ResourceSystem& Resource();

    static class RenderSystem& Render();

    static class SceneSystem& Scene();

    static class EntitySystem& Entity();

    static class EntitySystem& Tool();
};

#endif // SIEGE_ENGINE_STATICS_H
