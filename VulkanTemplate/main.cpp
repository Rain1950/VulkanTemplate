
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"

class App {
public:
	WindowManager windowManager;
	VulkanInstance vulkanInstance;
	ValidationLayersManager validationLayersManager;

	App(WindowManager WindowManager, VulkanInstance VulkanInstace) : windowManager{ WindowManager }, vulkanInstance{ VulkanInstace } {};
		

	void Run() {
		windowManager.InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}
private:


	void InitVulkan() {
		vulkanInstance.CreateInstance();
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
	VulkanInstance vulkanInstance{validationLayersManager};

	App app(windowManager,vulkanInstance);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}