
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "Window.h"
#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include <memory>
#include "GraphicsPipeline.h"
#include "App.h"



int main() {
	
	ValidationLayers validationLayers{};
	std::shared_ptr<VulkanInstance> vulkanInstance( new VulkanInstance(validationLayers));

	std::shared_ptr<Window> window(new Window{vulkanInstance});
	PhysicalDevice physicalDevice{vulkanInstance,window};
	LogicalDevice logicalDevice{};
	GraphicsPipeline graphicsPipeline{ 
		physicalDevice.commandPool,
		physicalDevice.swapChainExtent,
		logicalDevice.graphicsQueue
	};
	

	App app(window,vulkanInstance,physicalDevice,logicalDevice,validationLayers,graphicsPipeline);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}