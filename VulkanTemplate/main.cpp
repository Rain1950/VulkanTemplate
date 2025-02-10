
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
#include "App.h"



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