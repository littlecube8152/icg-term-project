#include "window.h"

#include <stdexcept>
#include <format>
#include <string>

#include "GLFW/glfw3.h"

#include "constants.h"

Window::Window(int width, int height, bool visible, GLFWwindow *shared)
{
    glfwWindowHint(GLFW_VISIBLE, visible);
    window = glfwCreateWindow(width, height, kWindowName, NULL, shared);
    if (!window)
    {
        const char *description;
        int code = glfwGetError(&description);
        glfwTerminate();
        throw std::runtime_error(std::format("failed to create GLFW window: error code {} ({})", code, std::string(description)));
    }
}

Window::operator GLFWwindow *()
{
    return window;
}