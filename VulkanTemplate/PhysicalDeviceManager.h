#pragma once

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
class PhysicalDeviceManager {
	
public:
	VulkanInstance* vulkanInstance;
	PhysicalDeviceManager(VulkanInstance* VulkanInstance);

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	void PickPhysicalDevice();

private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	

};