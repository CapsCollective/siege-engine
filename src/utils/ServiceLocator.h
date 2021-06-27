#ifndef A_DARK_DISCOMFORT_SERVICELOCATOR_H
#define A_DARK_DISCOMFORT_SERVICELOCATOR_H

#include <Camera3D.hpp>

class ServiceLocator
{
public:

    // Camera functions

    /**
     * A getter method for the raylib camera
     * @return the current raylib camera
     */
    static raylib::Camera* GetCamera()
    {
        return camera;
    }

    /**
     * A provider method for the raylib camera
     * @param cam - the raylib camera to provide
     */
    static void Provide(raylib::Camera* cam)
    {
        camera = cam;
    }

    // EditorController functions

    /**
     * A getter method for the EditorController
     * @return the EditorController
     */
    static class EditorController* GetEditorController()
    {
        return editorController;
    }

    /**
     * A provider method for the EditorController
     * @param editor - the EditorController to provide
     */
    static void Provide(EditorController* editor)
    {
        editorController = editor;
    }

    // MessageDisplay functions

    /**
     * A getter method for the MessageDisplay
     * @return the MessageDisplay
     */
    static class MessageDisplay* GetMessageDisplay()
    {
        return messageDisplay;
    }

    /**
     * A provider method for the MessageDisplay
     * @param display - the MessageDisplay to provide
     */
    static void Provide(MessageDisplay* display)
    {
        messageDisplay = display;
    }

    // Profiler functions

    /**
     * A getter method for the Profiler
     * @return the Profiler
     */
    static class Profiler* GetProfiler()
    {
        return systemProfiler;
    }

    /**
     * A provider method for the Profiler
     * @param profiler - the Profiler to provide
     */
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
