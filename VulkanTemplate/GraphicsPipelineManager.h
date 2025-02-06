#pragma once

#include <vector>
#include <vulkan/vulkan.h>

class GraphicsPipelineManager {
public:
	VkExtent2D swapChainExtent{};
	VkRenderPass renderPass{};
	VkPipelineLayout pipelineLayout{};
	VkPipeline graphicsPipeline{};


	GraphicsPipelineManager(VkExtent2D SwapChainExtent);
	void CreateGraphicsPipeline(VkDevice& device);
	void CleanGraphicsPipeline(VkDevice& device);
	void CleanPipelineLayout(VkDevice& device);
	void CleanRenderPass(VkDevice& device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice& device);
	void CreateRenderPass(VkFormat swapChainImageFormat, VkDevice& device);


};