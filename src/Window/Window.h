#pragma once

// Include Vulkan headers through glfw.
#include <volk/volk.h>
#include <GLFW/glfw3.h>
#include <cstdint>

namespace SnekVk 
{
    class Window 
    {
    public:

        // 'Structors

        Window(char const* name, int width, int height) : 
        width(width), height(height), name(name) 
        {
            if (!glfwInitialised)
            {
                glfwInit();
                glfwInitialised = true;    
            }
            
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

            window = glfwCreateWindow(width, height, name, nullptr, nullptr);

            if (glfwRawMouseMotionSupported())
            {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }

            glfwSetWindowUserPointer(window, this);
            glfwSetWindowSizeCallback(window, ResizeCallback);
            glfwWindows++;
        }

        Window() :
            Window("Window", 800, 600) {}

        ~Window() 
        {
            glfwDestroyWindow(window);
            if (--glfwWindows <= 0) glfwTerminate();
        }

        // Public Getters

        const int& GetHeight() { return height; }

        const int& GetWidth() { return width; }

        VkExtent2D GetExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height)}; }

        GLFWwindow* GetGlfwWindow() { return window; }

        // Public Functions

        void Update();

        bool WindowShouldClose();

        bool CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

        bool WasResized() { return wasResized; }

        void ResetWindowResized() { wasResized = false; }

        void EnableCursor() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }

        void DisableCursor() { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }

        void ToggleCursor(bool state) { state ? DisableCursor() : EnableCursor(); }

        void WaitEvents() { glfwWaitEvents(); }

    private:

        static void ResizeCallback(GLFWwindow* windowPtr, int width, int height);

        // Private static variables

        static bool glfwInitialised;
        static size_t glfwWindows;

        // Private variables

        GLFWwindow* window;

        char const* name;

        int width;
        int height;
        bool wasResized = false;
    };
}
