#include <vulkan/vulkan.h>
#include "LogicalDeviceManager.h"
#include "PhysicalDeviceManager.h"
#include <stdexcept>

void LogicalDeviceManager::CreateLogicalDevice(PhysicalDeviceManager* physicalDeviceManager, ValidationLayersManager* validationLayersManager) {
	PhysicalDeviceManager::QueueFamilyIndices indices = physicalDeviceManager->FindQueueFamilies(physicalDeviceManager->physicalDevice);
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
	queueCreateInfo.queueCount = 1;

	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;


	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos= &queueCreateInfo;
	createInfo.queueCreateInfoCount = 1;
	createInfo.pEnabledFeatures = &physicalDeviceManager->deviceFeatures;
	createInfo.enabledExtensionCount = 0;
	if (validationLayersManager->enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayersManager->validationLayers.size());
		createInfo.ppEnabledExtensionNames = validationLayersManager->validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDeviceManager->physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);

	
}

LogicalDeviceManager::~LogicalDeviceManager()
{
	vkDestroyDevice(device, nullptr);
}

