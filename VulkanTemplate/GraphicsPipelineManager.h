#pragma once

#include <vector>
#include <vulkan/vulkan.h>

class GraphicsPipelineManager {
public:
	void CreateGraphicsPipeline(VkDevice* device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice* device);
};