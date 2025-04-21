#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "Window.h"
#include "VulkanInstance.h"
#include "PhysicalDevice.h"
#include "LogicalDevice.h"
#include <memory>
#include "GraphicsPipeline.h"

class App {
public:
	 inline static const std::string MODEL_PATH = "models/sphere.obj";
	 inline static const std::string MODEL_TEXTURE = "textures/hamster.png";
	std::shared_ptr<Window> window;
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDevice physicalDevice;
	LogicalDevice logicalDevice;
	ValidationLayers validationLayers;
	GraphicsPipeline graphicsPipeline;

	App(std::shared_ptr<Window> Window, std::shared_ptr<VulkanInstance> VulkanInstace, PhysicalDevice PhysicalDevice, LogicalDevice LogicalDevice, ValidationLayers ValidationLayers, GraphicsPipeline GraphicsPipeline) :
		window{ Window },
		vulkanInstance{ VulkanInstace },
		physicalDevice{ PhysicalDevice },
		logicalDevice{ LogicalDevice },
		validationLayers{ ValidationLayers },
		graphicsPipeline{ GraphicsPipeline }
	{
	};


	void Run();
	
	
private:

	void InitVulkan();
	void DrawFrame();
	void MainLoop();
	void Cleanup();
		
	
};