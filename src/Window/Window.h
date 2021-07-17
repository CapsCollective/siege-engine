#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <cstdint>

namespace SnekVk 
{
    class Window 
    {
    public:

        // 'Structors

        Window(char* name, int width, int height) : 
        width(width), height(height), name(name) 
        {
            InitWindow();
        }

        Window() :
            Window("Window", 800, 600) {}

        ~Window() {}

        // Public Getters

        const int& GetHeight() { return height; }

        const int& GetWidth() { return width; }

        // Public Functions

        void Update();

        void DestroyWindow();

        bool WindowShouldClose();

        bool CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:

        // Private static variables
        static bool glfwInitialised;
        static size_t glfwWindows;

        // Private variables
        GLFWwindow* window;

        char* name;

        int width;
        int height;

        // Private funtions
        void InitWindow();
    };
}
