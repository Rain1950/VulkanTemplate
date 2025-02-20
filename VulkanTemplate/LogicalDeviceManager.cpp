#include <vulkan/vulkan.h>
#include "LogicalDeviceManager.h"
#include "PhysicalDeviceManager.h"
#include <stdexcept>
#include <set>

void LogicalDeviceManager::CreateLogicalDevice(PhysicalDeviceManager* physicalDeviceManager, ValidationLayersManager* validationLayersManager) {
	PhysicalDeviceManager::QueueFamilyIndices indices = physicalDeviceManager->FindQueueFamilies(physicalDeviceManager->physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(),indices.presentFamily.value() };
	float queuePriority = 1.0f;


	for (uint32_t queueFamily : uniqueQueueFamilies) {

		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	/*VkPhysicalDeviceSynchronization2Features sync2Features{};
	sync2Features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SYNCHRONIZATION_2_FEATURES;
	sync2Features.synchronization2 = VK_TRUE;*/

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos= queueCreateInfos.data();
	createInfo.pEnabledFeatures = &physicalDeviceManager->deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDeviceManager->deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = physicalDeviceManager->deviceExtensions.data();
	//createInfo.pNext = &sync2Features;



	if (validationLayersManager->enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayersManager->validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayersManager->validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDeviceManager->physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

	
}

void LogicalDeviceManager::Cleanup()
{
	vkDestroyDevice(device, nullptr);
}

