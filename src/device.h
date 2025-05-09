#ifndef DEVICE_H
#define DEVICE_H
#include <optional>
#include <vulkan/vulkan.hpp>

static const std::vector deviceExtensions {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

extern VkPhysicalDevice physicalDevice;
extern VkDevice logicalDevice;

struct QueueCollections
{
    std::optional<uint32_t> graphicsIndex;
    std::optional<uint32_t> presentIndex;
    QueueCollections(VkPhysicalDevice, VkSurfaceKHR);
    bool isComplete();
};

// Initializes `physicalDevice` and `logicalDevice`.
VkQueue initVulkanDevice(VkInstance instance, VkSurfaceKHR surface);

#endif // DEVICE_H