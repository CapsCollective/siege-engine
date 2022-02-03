#ifndef A_DARK_DISCOMFORT_STATICS_H
#define A_DARK_DISCOMFORT_STATICS_H

class Statics
{
public:

    static class CollisionSystem& Collision();

    static class InputSystem& Input();

    static class ResourceManager& Resource();

    static class RenderSystem& Render();

    static class SceneManager& Scene();

    static class EntityStorage& Entity();
};

#endif // A_DARK_DISCOMFORT_STATICS_H
