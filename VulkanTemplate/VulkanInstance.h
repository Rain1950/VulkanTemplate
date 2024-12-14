#pragma once
#include <Vulkan/vulkan.h>

class VulkanInstance {

public:
	VkInstance instance;

	void CreateInstance();
	void CheckExtensions();

	~VulkanInstance();

};