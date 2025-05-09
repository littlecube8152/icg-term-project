#include "window.h"

#include <stdexcept>

VkSurfaceKHR initVulkanWindowSurface(VkInstance instance, GLFWwindow *window)
{
    VkSurfaceKHR surface;
    int screen_index = 0;
    VkXcbSurfaceCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .connection = xcb_connect(NULL, &screen_index),
        .window = static_cast<xcb_window_t>(glfwGetX11Window(window))};

    if (vkCreateXcbSurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
    return surface;
}