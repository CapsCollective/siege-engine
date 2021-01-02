#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <Camera3D.hpp>

// TODO comment API functions
class ServiceLocator
{
public:

    // Camera functions

    static raylib::Camera* GetCamera()
    {
        return camera;
    }

    static void Provide(raylib::Camera* cam)
    {
        camera = cam;
    }

    // EditorController functions

    static class EditorController* GetEditorController()
    {
        return editorController;
    }

    static void Provide(EditorController* editor)
    {
        editorController = editor;
    }

    // MessageDisplay functions

    static class MessageDisplay* GetMessageDisplay()
    {
        return messageDisplay;
    }

    static void Provide(MessageDisplay* display)
    {
        messageDisplay = display;
    }

    // MessageDisplay functions

    static class Profiler* GetProfiler()
    {
        return systemProfiler;
    }

    static void Provide(Profiler* profiler)
    {
        systemProfiler = profiler;
    }

private:

    // Private fields

    static raylib::Camera* camera;

    static EditorController* editorController;

    static MessageDisplay* messageDisplay;

    static Profiler* systemProfiler;
};


#endif //A_DARK_DISCOMFORT_SERVICELOCATOR_H
