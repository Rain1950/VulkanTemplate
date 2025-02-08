
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"
#include "PhysicalDeviceManager.h"
#include "LogicalDeviceManager.h"
#include <memory>
#include "GraphicsPipelineManager.h"

class App {
public:
	std::shared_ptr<WindowManager> windowManager;
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDeviceManager physicalDeviceManager;
	LogicalDeviceManager logicalDeviceManager;
	ValidationLayersManager validationLayersManager;
	GraphicsPipelineManager graphicsPipelineManager;

	App(std::shared_ptr<WindowManager> WindowManager, std::shared_ptr<VulkanInstance> VulkanInstace, PhysicalDeviceManager PhysicalDeviceManager, LogicalDeviceManager LogicalDeviceManager, ValidationLayersManager ValidationLayersManager, GraphicsPipelineManager GraphicsPipelineManager) :
		windowManager{ WindowManager },
		vulkanInstance{ VulkanInstace },
		physicalDeviceManager{ PhysicalDeviceManager },
		logicalDeviceManager{ LogicalDeviceManager },
		validationLayersManager{ ValidationLayersManager },
		graphicsPipelineManager {GraphicsPipelineManager}
	{};
		

	void Run() {
		windowManager->InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}
private:


	void InitVulkan() {
		vulkanInstance->CreateInstance();
		windowManager->CreateSurface();
		physicalDeviceManager.PickPhysicalDevice();
		logicalDeviceManager.CreateLogicalDevice(&physicalDeviceManager,&validationLayersManager);
		physicalDeviceManager.CreateSwapChain(logicalDeviceManager.device);
		physicalDeviceManager.CreateImageViews(logicalDeviceManager.device);
		graphicsPipelineManager.CreateRenderPass(physicalDeviceManager.swapChainImageFormat,logicalDeviceManager.device);
		graphicsPipelineManager.CreateGraphicsPipeline(logicalDeviceManager.device);
		physicalDeviceManager.CreateFrameBuffers(logicalDeviceManager.device,graphicsPipelineManager.renderPass);
		physicalDeviceManager.CreateCommandPool(logicalDeviceManager.device);
		physicalDeviceManager.CreateCommandBuffer(logicalDeviceManager.device);
		physicalDeviceManager.CreateSyncObjects(logicalDeviceManager.device);

	}

	void DrawFrame() {
		vkWaitForFences(logicalDeviceManager.device, 1, &physicalDeviceManager.inFlightFence, VK_TRUE, UINT64_MAX);
		vkResetFences(logicalDeviceManager.device, 1, &physicalDeviceManager.inFlightFence);
		
		uint32_t imageIndex;
		vkAcquireNextImageKHR(logicalDeviceManager.device, physicalDeviceManager.swapChain, UINT64_MAX, physicalDeviceManager.imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

		vkResetCommandBuffer(physicalDeviceManager.commandBuffer, 0);
		physicalDeviceManager.RecordCommandBuffer(physicalDeviceManager.commandBuffer, imageIndex, graphicsPipelineManager);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		VkSemaphore waitSemaphores[] = { physicalDeviceManager.imageAvailableSemaphore };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &physicalDeviceManager.commandBuffer;

		VkSemaphore signalSemaphores[] = { physicalDeviceManager.renderFinishedSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		if (vkQueueSubmit(logicalDeviceManager.graphicsQueue, 1, &submitInfo, physicalDeviceManager.inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("Failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		VkSwapchainKHR swapChains[] = { physicalDeviceManager.swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;

		vkQueuePresentKHR(logicalDeviceManager.presentQueue, &presentInfo);




	}

	void MainLoop() {
		while (!glfwWindowShouldClose(windowManager->window)) {
			glfwPollEvents();
			DrawFrame();
		}
		vkDeviceWaitIdle(logicalDeviceManager.device);
		
	}

	void Cleanup() {
		physicalDeviceManager.CleanupSyncObjects(logicalDeviceManager.device);
		physicalDeviceManager.CleanupCommandPool(logicalDeviceManager.device);
		physicalDeviceManager.CleanupFrameBuffers(logicalDeviceManager.device);
		graphicsPipelineManager.CleanGraphicsPipeline(logicalDeviceManager.device);
		graphicsPipelineManager.CleanPipelineLayout(logicalDeviceManager.device);
		graphicsPipelineManager.CleanRenderPass(logicalDeviceManager.device);
		physicalDeviceManager.CleanupImageViews(logicalDeviceManager.device);
		physicalDeviceManager.CleanupSwapChain(logicalDeviceManager.device, physicalDeviceManager.swapChain);
		logicalDeviceManager.Cleanup();
		windowManager->Cleanup();
		vulkanInstance->Cleanup();
	}

};



int main() {
	ValidationLayersManager validationLayersManager{};
	std::shared_ptr<VulkanInstance> vulkanInstance( new VulkanInstance(validationLayersManager));
	std::shared_ptr<WindowManager> windowManager(new WindowManager{vulkanInstance});

	PhysicalDeviceManager physicalDeviceManager{vulkanInstance,windowManager};
	LogicalDeviceManager logicalDeviceManager{};
	GraphicsPipelineManager graphicsPipelineManager{physicalDeviceManager.swapChainExtent};


	App app(windowManager,vulkanInstance,physicalDeviceManager,logicalDeviceManager,validationLayersManager,graphicsPipelineManager);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}