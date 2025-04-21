#pragma once
#include <Vulkan/vulkan.h>
#include "ValidationLayers.h"
class VulkanInstance {

public:
	VkInstance instance{};
	ValidationLayers validationLayers;

	void CreateInstance();
	void CheckExtensions();
	void Cleanup();
	VulkanInstance(ValidationLayers ValidationLayers);
};