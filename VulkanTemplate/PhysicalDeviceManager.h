#pragma once

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <memory>
#include <optional>
#include "WindowManager.h"
class PhysicalDeviceManager {
	
public:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};
	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	std::shared_ptr<VulkanInstance> vulkanInstance;
	std::shared_ptr<WindowManager> windowManager;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures deviceFeatures{};
	PhysicalDeviceManager(std::shared_ptr<VulkanInstance> VulkanInstance,std::shared_ptr<WindowManager> WindowManager);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	void PickPhysicalDevice();
	

private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

};