#define GLFW_INCLUDE_VULKAN
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>

void WindowManager::InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		WindowManager::window = glfwCreateWindow(WindowManager::WIDTH, WindowManager::HEIGHT, "VulkanTemplate", nullptr, nullptr);

}

void WindowManager::CreateSurface() {
	if (glfwCreateWindowSurface(vulkanInstance->instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create windows surface");
	}
	
}

WindowManager::WindowManager(std::shared_ptr<VulkanInstance> VulkanInstance) : vulkanInstance{ VulkanInstance } {};

void WindowManager::Cleanup() {
	vkDestroySurfaceKHR(vulkanInstance->instance, surface, nullptr);
}

WindowManager::~WindowManager(){
	glfwDestroyWindow(window);
	glfwTerminate();
}


