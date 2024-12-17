
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"
#include "PhysicalDeviceManager.h"


class App {
public:
	WindowManager windowManager;
	VulkanInstance* vulkanInstance;
	PhysicalDeviceManager physicalDeviceManager;


	App(WindowManager WindowManager, VulkanInstance* VulkanInstace, PhysicalDeviceManager PhysicalDeviceManager) : 
		windowManager{ WindowManager }, 
		vulkanInstance{ VulkanInstace }, 
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
	VulkanInstance* vulkanInstance = new VulkanInstance(validationLayersManager);

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