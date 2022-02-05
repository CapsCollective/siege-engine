#ifndef A_DARK_DISCOMFORT_STATICS_H
#define A_DARK_DISCOMFORT_STATICS_H

class Statics
{
public:

    static class CollisionSystem& Collision();

    static class InputSystem& Input();

    static class ResourceSystem& Resource();

    static class RenderSystem& Render();

    static class SceneSystem& Scene();

    static class EntitySystem& Entity();
};

#endif // A_DARK_DISCOMFORT_STATICS_H
