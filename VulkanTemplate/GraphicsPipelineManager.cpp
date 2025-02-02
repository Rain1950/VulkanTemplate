

#include <vulkan/vulkan.h>
#include "GraphicsPipelineManager.h"
#include "FileLoader.h"


VkShaderModule GraphicsPipelineManager::CreateShaderModule(const std::vector<char>& code,VkDevice* device) {
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(*device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create Shader module");
	}
	return shaderModule;
}


void GraphicsPipelineManager::CreateGraphicsPipeline(VkDevice* device)
{
	auto vertShaderCode = ReadFile("shaders/vert.spv");
	auto fragShaderCode = ReadFile("shaders/frag.spv");
	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode,device);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode,device);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";
	
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	vertShaderStageInfo.module = fragShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo,fragShaderStageInfo };


	vkDestroyShaderModule(*device, fragShaderModule, nullptr);
	vkDestroyShaderModule(*device, vertShaderModule, nullptr);
}

