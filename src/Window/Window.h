#pragma once

// Include Vulkan headers through glfw.
#include <volk.h>
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
            window = glfwCreateWindow(width, height, name, nullptr, nullptr);
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

        // Public Functions

        void Update();

        bool WindowShouldClose();

        bool CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:

        // Private static variables

        static bool glfwInitialised;
        static size_t glfwWindows;

        // Private variables

        GLFWwindow* window;

        char const* name;

        int width;
        int height;
    };
}
