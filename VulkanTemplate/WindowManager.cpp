#define GLFW_INCLUDE_VULKAN
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include "App.h"
#include "PhysicalDeviceManager.h"

void WindowManager::InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(WindowManager::WIDTH, WindowManager::HEIGHT, "VulkanTemplate", nullptr, nullptr);
		glfwSetFramebufferSizeCallback(window, FrameBufferResizeCallback);

}

void WindowManager::CreateSurface() {
	if (glfwCreateWindowSurface(vulkanInstance->instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create windows surface");
	}
	
}

  void WindowManager::FrameBufferResizeCallback(GLFWwindow* window, int width, int height){
	 auto physicalDeviceManager = reinterpret_cast<PhysicalDeviceManager*>(glfwGetWindowUserPointer(window));
	 physicalDeviceManager->frameBufferResized = true;
}

 WindowManager::WindowManager(std::shared_ptr<VulkanInstance> VulkanInstance) : vulkanInstance{ VulkanInstance } {};

void WindowManager::Cleanup() {
	vkDestroySurfaceKHR(vulkanInstance->instance, surface, nullptr);
}

WindowManager::~WindowManager(){
	glfwDestroyWindow(window);
	glfwTerminate();
}


