#ifndef SWAP_CHAIN_H
#define SWAP_CHAIN_H
#include <vulkan/vulkan.hpp>
#include <vector>

#include "window.h"

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSwapchainKHR createSwapChain(GLFWwindow *window, VkSurfaceKHR surface);

#endif // SWAP_CHAIN_H