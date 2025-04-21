#pragma once

#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <memory>
#include <optional>
#include "Window.h"
#include <vector>
#include "GraphicsPipeline.h"


#define MAX_FRAMES_IN_FLIGHT  2
class PhysicalDevice {
	
public:
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool IsComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;

	};

	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			//VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME
	};


	std::shared_ptr<VulkanInstance> vulkanInstance;
	std::shared_ptr<Window> window;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkPhysicalDeviceFeatures deviceFeatures{ .samplerAnisotropy = VK_TRUE };
	VkSwapchainKHR swapChain{};
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFrameBuffers;

	uint32_t currentFrame = 0;
	VkFormat swapChainImageFormat{};
	VkExtent2D* swapChainExtent;
	VkCommandPool** commandPool;
	
	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	bool frameBufferResized = false;

	PhysicalDevice(std::shared_ptr<VulkanInstance> VulkanInstance,std::shared_ptr<Window> Window);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	void PickPhysicalDevice();
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain(VkDevice& device);
	void CreateFrameBuffers(VkDevice& device, VkRenderPass& renderPass, VkImageView& depthImageView);

	void CreateImageViews(VkDevice& device);
	void CreateCommandPool(VkDevice& device);
	void CreateCommandBuffers(VkDevice& device);
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, GraphicsPipeline& graphicsPipeline);
	void CreateSyncObjects(VkDevice& device);
	void RecreateSwapChain(VkDevice& device, GraphicsPipeline& graphicsPipeline);
	static VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features, VkPhysicalDevice& physicalDevice);
	

	void CleanupSyncObjects(VkDevice& device);
	void CleanupCommandPool(VkDevice& device);
	void CleanupImageViews(VkDevice& device);
	void CleanupSwapChain(VkDevice& device, VkSwapchainKHR swapChain, GraphicsPipeline& graphicsPipeline);
	void CleanupFrameBuffers(VkDevice& device);
private:
	bool IsDeviceSuitable(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

};


