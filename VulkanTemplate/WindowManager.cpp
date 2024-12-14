
#include "WindowManager.h"
#include <GLFW/glfw3.h>



void WindowManager::InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		WindowManager::window = glfwCreateWindow(WindowManager::WIDTH, WindowManager::HEIGHT, "VulkanTemplate", nullptr, nullptr);
}

WindowManager::~WindowManager(){
	glfwDestroyWindow(window);
	glfwTerminate();
}


