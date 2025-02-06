

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "VulkanInstance.h"
#include "ValidationLayersManager.h"


VulkanInstance::VulkanInstance(ValidationLayersManager ValidationLayersManager) : validationLayersManager{ ValidationLayersManager }{};


	void VulkanInstance::Cleanup() {
		vkDestroyInstance(instance, nullptr);
	}

	void  VulkanInstance::CreateInstance() {

		if (validationLayersManager.enableValidationLayers && !validationLayersManager.CheckValidationLayerSupport()) {
			throw std::runtime_error("Validation layers requested, but not available!");
		}

		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "VulkanTemplate";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.apiVersion = VK_API_VERSION_1_0;



		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		if (validationLayersManager.enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayersManager.validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayersManager.validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;		

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Instace creation failed");

		}


		CheckExtensions();

	}

	void VulkanInstance::CheckExtensions() {
		uint32_t extensionCount;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		/*std::cout << "Available extensions \n";
		std::cout << "Extension Count: " << extensionCount;
	
		for (const auto& extension : extensions) {
			std::cout << '\t' << extension.extensionName << '\n';
		}*/

	}

	

		