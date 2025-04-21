#pragma once
#include <vulkan/vulkan.h>
#include "PhysicalDevice.h"

class LogicalDevice {
public:
	VkDevice device{};
	VkQueue** graphicsQueue{};
	VkQueue presentQueue{};
	LogicalDevice();
	
	void CreateLogicalDevice(PhysicalDevice* physicalDevice, ValidationLayers* validationLayers);
	void Cleanup();

	

};




