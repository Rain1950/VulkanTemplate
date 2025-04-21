#include <vulkan/vulkan.h>
#include "LogicalDevice.h"
#include "PhysicalDevice.h"
#include <stdexcept>
#include <set>


LogicalDevice::LogicalDevice()
{
	graphicsQueue = new VkQueue*;
	*graphicsQueue = new VkQueue;
}

void LogicalDevice::CreateLogicalDevice(PhysicalDevice* physicalDevice, ValidationLayers* validationLayers) {
	PhysicalDevice::QueueFamilyIndices indices = physicalDevice->FindQueueFamilies(physicalDevice->physicalDevice);

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


	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos= queueCreateInfos.data();
	createInfo.pEnabledFeatures = &physicalDevice->deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(physicalDevice->deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = physicalDevice->deviceExtensions.data();




	if (validationLayers->enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers->validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers->validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice->physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create logical device");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, *graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);

	
}

void LogicalDevice::Cleanup()
{
	vkDestroyDevice(device, nullptr);
}

