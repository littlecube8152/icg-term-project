#ifndef INSTANCE_H
#define INSTANCE_H
#include <vulkan/vulkan.hpp>
#include <vector>

static const std::vector instanceExtensions {
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
};

VkInstance initVulkanInstance();
void destroyVulkanInstance(VkInstance instance);

#endif // INSTANCE_H