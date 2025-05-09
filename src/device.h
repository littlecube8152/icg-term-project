#ifndef DEVICE_H
#define DEVICE_H
#include <optional>
#include <vulkan/vulkan.hpp>

extern VkPhysicalDevice physicalDevice;
extern VkDevice logicalDevice;

struct QueueCollections
{
    std::optional<uint32_t> graphicsIndex;
    QueueCollections(VkPhysicalDevice);
    bool isComplete();
};

// Initializes `physicalDevice` and `logicalDevice`.
void initVulkanDevice(VkInstance instance);

#endif // DEVICE_H