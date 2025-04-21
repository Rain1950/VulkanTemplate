#define GLFW_INCLUDE_VULKAN
#include "Window.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>
#include "App.h"
#include "PhysicalDevice.h"

void Window::InitWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		window = glfwCreateWindow(Window::WIDTH, Window::HEIGHT, "VulkanTemplate", nullptr, nullptr);
		glfwSetFramebufferSizeCallback(window, FrameBufferResizeCallback);

}

void Window::CreateSurface() {
	if (glfwCreateWindowSurface(vulkanInstance->instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create windows surface");
	}
	
}

  void Window::FrameBufferResizeCallback(GLFWwindow* window, int width, int height){
	 auto physicalDevice = reinterpret_cast<PhysicalDevice*>(glfwGetWindowUserPointer(window));
	 physicalDevice->frameBufferResized = true;
}

 Window::Window(std::shared_ptr<VulkanInstance> VulkanInstance) : vulkanInstance{ VulkanInstance } {};

void Window::Cleanup() {
	vkDestroySurfaceKHR(vulkanInstance->instance, surface, nullptr);
}

Window::~Window(){
	glfwDestroyWindow(window);
	glfwTerminate();
}


