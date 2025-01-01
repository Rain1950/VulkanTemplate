#pragma once
#include <vulkan/vulkan.h>
#include "PhysicalDeviceManager.h"

class LogicalDeviceManager {
public:
	VkDevice device;
	VkQueue graphicsQueue;
	void CreateLogicalDevice(PhysicalDeviceManager* physicalDeviceManager, ValidationLayersManager* validationLayersManager);
	~LogicalDeviceManager();
	

};




