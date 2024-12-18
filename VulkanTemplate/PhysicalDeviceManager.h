#pragma once

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <memory>
class PhysicalDeviceManager {
	
public:
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDeviceManager(std::shared_ptr<VulkanInstance> VulkanInstance);

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	void PickPhysicalDevice();

private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	

};