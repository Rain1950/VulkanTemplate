
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"
#include "PhysicalDeviceManager.h"
#include "LogicalDeviceManager.h"
#include <memory>


class App {
public:
	std::shared_ptr<WindowManager> windowManager;
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDeviceManager physicalDeviceManager;
	LogicalDeviceManager logicalDeviceManager;
	ValidationLayersManager validationLayersManager;

	App(std::shared_ptr<WindowManager> WindowManager, std::shared_ptr<VulkanInstance> VulkanInstace, PhysicalDeviceManager PhysicalDeviceManager, LogicalDeviceManager LogicalDeviceManager, ValidationLayersManager ValidationLayersManager) :
		windowManager{ WindowManager },
		vulkanInstance{ VulkanInstace },
		physicalDeviceManager{ PhysicalDeviceManager },
		logicalDeviceManager{ LogicalDeviceManager },
		validationLayersManager{ ValidationLayersManager }
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
		physicalDeviceManager.CreateSwapChain(&logicalDeviceManager.device);
	}


	void MainLoop() {
		while (!glfwWindowShouldClose(windowManager->window)) {
			glfwPollEvents();
		}
		
	}

	void Cleanup() {
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

	App app(windowManager,vulkanInstance,physicalDeviceManager,logicalDeviceManager,validationLayersManager);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}