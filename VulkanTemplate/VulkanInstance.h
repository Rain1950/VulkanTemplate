#pragma once
#include <Vulkan/vulkan.h>
#include "ValidationLayersManager.h"
class VulkanInstance {

public:
	VkInstance instance;
	ValidationLayersManager validationLayersManager;

	void CreateInstance();
	void CheckExtensions();

	~VulkanInstance();
	VulkanInstance(ValidationLayersManager& ValidationLayersManager);
};