#ifndef INSTANCE_H
#define INSTANCE_H
#include <vulkan/vulkan.hpp>

VkInstance initVulkanInstance();
void destroyVulkanInstance(VkInstance instance);

#endif // INSTANCE_H