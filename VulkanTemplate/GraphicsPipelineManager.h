#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <glm.hpp>
#include <array>

class GraphicsPipelineManager {
public:

	struct Vertex {
		glm::vec2 pos;
		glm::vec3 color;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;

		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);


			return attributeDescriptions;

		}
	};

	const  std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{0.7f, -0.1f}, {1.0f, 0.0f, 0.0f}},
	{{0.2f, 0.2f}, {0.0f, 1.0f, 0.0f}},
	{{-0.2f, 0.2f}, {0.0f, 0.0f, 1.0f}},

	};

	VkExtent2D swapChainExtent{};
	VkRenderPass renderPass{};
	VkPipelineLayout pipelineLayout{};
	VkPipeline graphicsPipeline{};
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	GraphicsPipelineManager(VkExtent2D SwapChainExtent);
	void CreateGraphicsPipeline(VkDevice& device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice& device);
	void CreateRenderPass(VkFormat swapChainImageFormat, VkDevice& device);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
	void CreateVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);

	void CleanGraphicsPipeline(VkDevice& device);
	void CleanPipelineLayout(VkDevice& device);
	void CleanRenderPass(VkDevice& device);
	void CleanupVertexBuffer(VkDevice& device);


};