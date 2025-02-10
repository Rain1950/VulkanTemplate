#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include "VulkanInstance.h"
#include <memory>
#include "GraphicsPipelineManager.h"
class WindowManager {
	public:
		std::shared_ptr<VulkanInstance> vulkanInstance;
		GLFWwindow* window = {};
		VkSurfaceKHR surface;
		~WindowManager();
		WindowManager(std::shared_ptr<VulkanInstance> VulkanInstance);
		void CreateSurface();
		static void FrameBufferResizeCallback(GLFWwindow* window, int width, int height);
		void InitWindow();
		void Cleanup();
	private:
		const uint32_t WIDTH = 800;
		const uint32_t HEIGHT = 600;
};