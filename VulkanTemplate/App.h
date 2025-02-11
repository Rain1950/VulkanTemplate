#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>
#include "WindowManager.h"
#include "VulkanInstance.h"
#include "PhysicalDeviceManager.h"
#include "LogicalDeviceManager.h"
#include <memory>
#include "GraphicsPipelineManager.h"

class App {
public:
	std::shared_ptr<WindowManager> windowManager;
	std::shared_ptr<VulkanInstance> vulkanInstance;
	PhysicalDeviceManager physicalDeviceManager;
	LogicalDeviceManager logicalDeviceManager;
	ValidationLayersManager validationLayersManager;
	GraphicsPipelineManager graphicsPipelineManager;

	App(std::shared_ptr<WindowManager> WindowManager, std::shared_ptr<VulkanInstance> VulkanInstace, PhysicalDeviceManager PhysicalDeviceManager, LogicalDeviceManager LogicalDeviceManager, ValidationLayersManager ValidationLayersManager, GraphicsPipelineManager GraphicsPipelineManager) :
		windowManager{ WindowManager },
		vulkanInstance{ VulkanInstace },
		physicalDeviceManager{ PhysicalDeviceManager },
		logicalDeviceManager{ LogicalDeviceManager },
		validationLayersManager{ ValidationLayersManager },
		graphicsPipelineManager{ GraphicsPipelineManager }
	{
	};


	void Run();
	
	
private:

	void InitVulkan();
	void DrawFrame();
	void MainLoop();
	void Cleanup();
		
	
};