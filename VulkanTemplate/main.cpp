
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"
#include "PhysicalDeviceManager.h"
#include <memory>


class App {
public:
	WindowManager windowManager;
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDeviceManager physicalDeviceManager;


	App(WindowManager WindowManager, std::shared_ptr<VulkanInstance> VulkanInstace, PhysicalDeviceManager PhysicalDeviceManager) : 
		windowManager{ WindowManager }, 
		vulkanInstance{ VulkanInstace}, 
		physicalDeviceManager{ PhysicalDeviceManager } 
	{};
		

	void Run() {
		windowManager.InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}
private:


	void InitVulkan() {
		vulkanInstance->CreateInstance();
		physicalDeviceManager.PickPhysicalDevice();
	}


	void MainLoop() {
		while (!glfwWindowShouldClose(windowManager.window)) {
			glfwPollEvents();
		}
		
	}

	void Cleanup() {
		
	}

};



int main() {
	WindowManager windowManager{};
	ValidationLayersManager validationLayersManager{};
	std::shared_ptr<VulkanInstance> vulkanInstance( new VulkanInstance(validationLayersManager));

	PhysicalDeviceManager physicalDeviceManager{vulkanInstance};

	App app(windowManager,vulkanInstance,physicalDeviceManager);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}