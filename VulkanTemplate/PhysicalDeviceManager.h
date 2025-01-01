#pragma once

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <memory>
#include <optional>
class PhysicalDeviceManager {
	
public:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;

		bool IsComplete() {
			return graphicsFamily.has_value();
		}
	};

	std::shared_ptr<VulkanInstance> vulkanInstance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures deviceFeatures{};
	PhysicalDeviceManager(std::shared_ptr<VulkanInstance> VulkanInstance);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void PickPhysicalDevice();


private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	

};