#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <glm.hpp>
#include <array>
#include "GraphicsPipelineManager.h"
#define GLM_FORCE_RADIANS
#include <chrono>
#include <gtc/matrix_transform.hpp>

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


	const std::vector<Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices{
		0,1,2,2,3,0
	};

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;

	};

	
	VkRenderPass renderPass{};
	VkPipelineLayout pipelineLayout{};
	VkPipeline graphicsPipeline{};
	VkBuffer vertexBuffer{};
	VkDeviceMemory vertexBufferMemory{};
	VkBuffer indexBuffer{};
	VkDeviceMemory indexBufferMemory{};
	std::vector<VkBuffer> uniformBuffers{};
	std::vector<VkDeviceMemory> uniformBuffersMemory{};
	std::vector<void*> uniformBuffersMapped{};


	VkCommandPool** commandPool{};
	VkExtent2D* swapChainExtent{};
	VkQueue** graphicsQueue{};
	VkDescriptorSetLayout descriptorSetLayout{};
	VkDescriptorPool descriptorPool{};
	std::vector<VkDescriptorSet> descriptorSets{};
	VkImage textureImage{};
	VkDeviceMemory textureImageMemory{};
	VkImageView textureImageView{};
	VkSampler textureSampler{};
	



	GraphicsPipelineManager(VkCommandPool** CommandPool, VkExtent2D* SwapChainExtent, VkQueue** GraphicsQueue);
	
	void CreateGraphicsPipeline(VkDevice& device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice& device);
	void CreateRenderPass(VkFormat swapChainImageFormat, VkDevice& device);
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
	void CreateVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);
	void CreateIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);
	static void CreateBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkDevice& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateDescriptorSetLayout(VkDevice& device);
	void CreateUniformBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice, int MAX_FRAMES_IN_FLIGHT);
	void UpdateUniformBuffers(uint32_t currentImage);
	void CreateDescriptorPool(VkDevice& device,int count);
	void CreateDescriptorSets(VkDevice& device, int count);
	void CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
	void CreateTextureImage(VkDevice& device, VkPhysicalDevice& physicalDevice);
	VkCommandBuffer BeginSingleTimeCommands(VkDevice& device);
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice& device);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice& device);
	void CopyBufferToImage(VkDevice& device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void CreateTextureImageView(VkDevice& device);
	static VkImageView CreateImageView(VkDevice& device, VkImage image, VkFormat forrmat);
	void CreateTextureSampler(VkDevice& device, VkPhysicalDevice& physicalDevice);
	


	void CleanupTextureSampler(VkDevice& device);
	void CleanupUniformBuffers(VkDevice& device, int MAX_FRAMES_IN_FLIGHT);
	void CleanGraphicsPipeline(VkDevice& device);
	void CleanPipelineLayout(VkDevice& device);
	void CleanRenderPass(VkDevice& device);
	void CleanupVertexBuffer(VkDevice& device);
	void CleanupIndexBuffer(VkDevice& device);
	void CleanupDescriptorSetLayout(VkDevice& device);
	void CleanupTextureImage(VkDevice& device);
	void CleanupTextureView(VkDevice& device);



};