#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include <glm.hpp>
#include <array>
#include "GraphicsPipeline.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <chrono>
#include <gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/hash.hpp>




class GraphicsPipeline {
public:

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec2 texCoord;
		glm::vec3 normal;

		static VkVertexInputBindingDescription GetBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;

		}

		static std::array<VkVertexInputAttributeDescription, 4> GetAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions{};
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);


			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof(Vertex, normal);



			return attributeDescriptions;

		}
		
		bool operator==(const Vertex& other) const {
			return pos == other.pos && color == other.color && texCoord == other.texCoord && normal == other.normal;
		}

		
	};




	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<uint32_t> normals;

	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		float time;
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
	uint32_t mipLevels{};
	VkImage textureImage{};
	VkDeviceMemory textureImageMemory{};
	VkImageView textureImageView{};
	VkSampler textureSampler{};
	
	VkImage depthImage{};
	VkDeviceMemory depthImageMemory{};
	VkImageView depthImageView{};



	GraphicsPipeline(VkCommandPool** CommandPool, VkExtent2D* SwapChainExtent, VkQueue** GraphicsQueue);
	
	void CreateGraphicsPipeline(VkDevice& device);
	VkShaderModule CreateShaderModule(const std::vector<char>& code, VkDevice& device);
	void CreateRenderPass(VkFormat swapChainImageFormat, VkDevice& device, VkPhysicalDevice& phyiscalDevice);
	static uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);
	void CreateVertexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);
	void CreateIndexBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice);
	static void CreateBuffer(VkDevice& device, VkPhysicalDevice& physicalDevice,VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void CopyBuffer(VkDevice& device, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void CreateDescriptorSetLayout(VkDevice& device);
	void CreateUniformBuffers(VkDevice& device, VkPhysicalDevice& physicalDevice);
	void UpdateUniformBuffers(uint32_t currentImage);
	void CreateDescriptorPool(VkDevice& device);
	void CreateDescriptorSets(VkDevice& device);
	void CreateImage(uint32_t width, uint32_t height,uint32_t mipLevels,VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory, VkDevice& device, VkPhysicalDevice& physicalDevice);
	void CreateTextureImage(VkDevice& device, VkPhysicalDevice& physicalDevice);
	VkCommandBuffer BeginSingleTimeCommands(VkDevice& device);
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer, VkDevice& device);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, VkDevice& device, uint32_t mipLevels);
	void CopyBufferToImage(VkDevice& device, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void CreateTextureImageView(VkDevice& device);
	static VkImageView CreateImageView(VkDevice& device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	void CreateTextureSampler(VkDevice& device, VkPhysicalDevice& physicalDevice);
	void CreateDepthResources(VkPhysicalDevice& physicalDevice, VkDevice& device);
	VkFormat FindDepthFormat(VkPhysicalDevice& physicalDevice);
	void GenerateMipMaps(VkDevice& device, VkPhysicalDevice& physicalDevice, VkFormat imageFormat, VkImage image, int32_t textWidth, int32_t texHeight, uint32_t mipLevels);



	void CleanupTextureSampler(VkDevice& device);
	void CleanGraphicsPipeline(VkDevice& device);
	void CleanPipelineLayout(VkDevice& device);
	void CleanRenderPass(VkDevice& device);
	void CleanupUniformBuffers(VkDevice& device);
	void CleanupVertexBuffer(VkDevice& device);
	void CleanupIndexBuffer(VkDevice& device);
	void CleanupDescriptorSetLayout(VkDevice& device);
	void CleanupTextureImage(VkDevice& device);
	void CleanupTextureView(VkDevice& device);

	private:
		bool HasStencilComponent(VkFormat format);

};

namespace std {
	template<> struct hash<GraphicsPipeline::Vertex> {
		size_t operator()(GraphicsPipeline::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^
				(hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}