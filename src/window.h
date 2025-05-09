#ifndef WINDOW_H
#define WINDOW_H

#define VK_USE_PLATFORM_XCB_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <vulkan/vulkan.hpp>
#include <vector>

VkSurfaceKHR initVulkanWindowSurface(VkInstance instance, GLFWwindow *window);

#endif // WINDOW_H