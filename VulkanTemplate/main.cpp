
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"


class App {
public:
	WindowManager windowManager;

	App(WindowManager WindowManager) : windowManager{ WindowManager } {};
		

	void Run() {
		windowManager.InitWindow();
		InitVulkan();
		MainLoop();
		Cleanup();
	}
private:


	void InitVulkan() {

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
	App app(windowManager);
	

	try {
		app.Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}