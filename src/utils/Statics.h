#ifndef A_DARK_DISCOMFORT_STATICS_H
#define A_DARK_DISCOMFORT_STATICS_H

class Statics
{
public:

#ifndef CC_IGNORE_STATICS
    static class CollisionSystem CollisionSystem;

    static class Input Input;

    static class ResourceManager ResourceManager;

    static class RenderSystem RenderSystem;

    static class SceneManager SceneManager;
#endif
};

#endif // A_DARK_DISCOMFORT_STATICS_H
