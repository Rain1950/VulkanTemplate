
#include "PhysicalDeviceManager.h"
#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <stdexcept>

PhysicalDeviceManager::PhysicalDeviceManager(VulkanInstance& VulkanInstance) :vulkanInstance{ VulkanInstance } {};
 

void PhysicalDeviceManager::PickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);

	vkEnumeratePhysicalDevices(vulkanInstance.instance, &deviceCount, devices.data());
	
	for (const auto& device : devices) {
		if (IsDeviceSuitable(device)) {
			physicalDevice = device;
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("Failed to find suitable GPU");
	}
}


bool PhysicalDeviceManager::IsDeviceSuitable(VkPhysicalDevice device) {
	 
}