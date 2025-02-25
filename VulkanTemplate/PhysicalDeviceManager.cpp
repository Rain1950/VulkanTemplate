
#include "PhysicalDeviceManager.h"
#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <stdexcept>
#include <optional>
#include <vector>
#include "WindowManager.h"
#include <set>
#include <GLFW/glfw3.h>
#include <cstdint>
#include <algorithm>
#include <limits>
#include <iostream>


PhysicalDeviceManager::PhysicalDeviceManager(std::shared_ptr<VulkanInstance> VulkanInstance, std::shared_ptr<WindowManager> WindowManager) : 
	vulkanInstance{ VulkanInstance }, 
	windowManager{ WindowManager } {
	commandPool = new VkCommandPool*;
	*commandPool = new VkCommandPool{};
};
 

void PhysicalDeviceManager::PickPhysicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanInstance->instance, &deviceCount, nullptr);
	if (deviceCount == 0) {
		throw std::runtime_error("Failed to find GPUs with Vulkan support");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);

	vkEnumeratePhysicalDevices(vulkanInstance->instance, &deviceCount, devices.data());
	
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

VkSurfaceFormatKHR PhysicalDeviceManager::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& format : availableFormats) {
		if (format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR && format.format == VK_FORMAT_B8G8R8A8_SRGB) return format;
	}

	return availableFormats[0];
}

VkPresentModeKHR   PhysicalDeviceManager::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) return availablePresentMode;
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D PhysicalDeviceManager::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int width, height;
		glfwGetFramebufferSize(windowManager->window, &width, &height);

		VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height),
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}

}

void PhysicalDeviceManager::CreateSwapChain(VkDevice& device)
{
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);
	
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount+1;

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = windowManager->surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainExtent = extent;
}

void PhysicalDeviceManager::CleanupSwapChain(VkDevice& device, VkSwapchainKHR swapChain)
{
	vkDestroySwapchainKHR(device, swapChain, nullptr);
}

void PhysicalDeviceManager::CreateImageViews(VkDevice& device)
{
	swapChainImageViews.resize(swapChainImages.size());
	for(int i = 0; i < swapChainImages.size();i++){
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create image views");
		}
	}
}

void PhysicalDeviceManager::CleanupImageViews(VkDevice& device) {
	for (auto imageView : swapChainImageViews) {
		vkDestroyImageView(device, imageView, nullptr);
	}
}

void PhysicalDeviceManager::CreateFrameBuffers(VkDevice& device, VkRenderPass& renderPass)
{ 
	swapChainFrameBuffers.resize(swapChainImageViews.size());
	for (size_t i = 0; i < swapChainImageViews.size(); i++) {
		VkImageView attachments[] = {
			swapChainImageViews[i]
		};

		VkFramebufferCreateInfo frameBufferInfo{};
		frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass = renderPass;
		frameBufferInfo.attachmentCount = 1;
		frameBufferInfo.pAttachments = attachments;
		frameBufferInfo.width = swapChainExtent.width;
		frameBufferInfo.height = swapChainExtent.height;
		frameBufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &frameBufferInfo, nullptr, &swapChainFrameBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}

void PhysicalDeviceManager::CreateCommandPool(VkDevice& device)
{
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice);
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, *commandPool) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create command pool");
	}

	
	
}

void PhysicalDeviceManager::CreateCommandBuffers(VkDevice& device)
{
	commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = **commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void PhysicalDeviceManager::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,GraphicsPipelineManager& graphicsPipelineManager) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = graphicsPipelineManager.renderPass;
	renderPassInfo.framebuffer = swapChainFrameBuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,graphicsPipelineManager.graphicsPipeline);
	VkBuffer vertexBuffers[] = { graphicsPipelineManager.vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdDraw(commandBuffer, static_cast<uint32_t>(graphicsPipelineManager.vertices.size()), 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void PhysicalDeviceManager::CreateSyncObjects(VkDevice& device)
{

	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create semaphores!");
		}
	}
}

void PhysicalDeviceManager::RecreateSwapChain(VkDevice& device,VkRenderPass& renderPass)
{

	int width = 0, height = 0;
	glfwGetFramebufferSize(windowManager->window, &width, &height);
	while (width == 0 || height == 0) {
		glfwGetFramebufferSize(windowManager->window, &width, &height);
		glfwWaitEvents();
	}

	vkDeviceWaitIdle(device);
	
	CleanupFrameBuffers(device);
	CleanupImageViews(device);
	CleanupSwapChain(device, swapChain);

	CreateSwapChain(device);
	CreateImageViews(device);
	CreateFrameBuffers(device, renderPass);

}

void PhysicalDeviceManager::CleanupSyncObjects(VkDevice& device){
	for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(device, inFlightFences[i], nullptr);
	}
}


void PhysicalDeviceManager::CleanupCommandPool(VkDevice& device) {
	vkDestroyCommandPool(device, **commandPool, nullptr);
}


void PhysicalDeviceManager::CleanupFrameBuffers(VkDevice& device)
{
	for (auto framebuffer : swapChainFrameBuffers) {
		vkDestroyFramebuffer(device, framebuffer, nullptr);
	}
}

PhysicalDeviceManager::SwapChainSupportDetails PhysicalDeviceManager::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, windowManager->surface, &details.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowManager->surface, &formatCount,nullptr); // query number

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, windowManager->surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowManager->surface, &presentModeCount, nullptr);
	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, windowManager->surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}

PhysicalDeviceManager::QueueFamilyIndices PhysicalDeviceManager::FindQueueFamilies(VkPhysicalDevice device) {
	QueueFamilyIndices indices;
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;

	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, windowManager->surface, &presentSupport);
		if (presentSupport) {
			indices.presentFamily = i;
		}
		if (indices.IsComplete()) break;

		i++;
	}


	return indices;
}


bool PhysicalDeviceManager::IsDeviceSuitable(VkPhysicalDevice device) {
	QueueFamilyIndices indices = FindQueueFamilies(device);
	bool swapChainAdequate = false;
	bool extensionsSupported = CheckDeviceExtensionSupport(device);
	
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

bool PhysicalDeviceManager::CheckDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string>  requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}
	return requiredExtensions.empty();
}



