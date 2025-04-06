#pragma once
#include <vulkan/vulkan.h>
#include "PhysicalDeviceManager.h"

class LogicalDeviceManager {
public:
	VkDevice device{};
	VkQueue** graphicsQueue{};
	VkQueue presentQueue{};
	LogicalDeviceManager();
	
	void CreateLogicalDevice(PhysicalDeviceManager* physicalDeviceManager, ValidationLayersManager* validationLayersManager);
	void Cleanup();

	

};




