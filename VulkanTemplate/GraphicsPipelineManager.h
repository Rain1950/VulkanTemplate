#pragma once

#include <vector>
#include <vulkan/vulkan.h>

class GraphicsPipelineManager {
public:
	VkExtent2D swapChainExtent{};
	VkPipelineLayout pipelineLayout{};

	GraphicsPipelineManager(VkExtent2D SwapChainExtent);
	void CreateGraphicsPipeline(VkDevice* device);
	void CleanPipelineLayout(VkDevice* device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice* device);

};