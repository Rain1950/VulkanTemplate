#include "FileLoader.h"
#include "GraphicsPipelineManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>

std::vector<char> FileLoader::ReadShaderFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		throw std::runtime_error("Failed to open file!");
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

TextureImageData FileLoader::ReadTextureImage(VkDevice& device, std::string textureFilePath)
{
	if (textureFilePath.empty()) {
		throw std::runtime_error("Empty texture file path, failed to load texture image");
	}

	int width, height, texChannels;
	stbi_uc* pixels = stbi_load(textureFilePath.data(), &width, &height, &texChannels, STBI_rgb_alpha);
	
	VkDeviceSize imageSize = width * height * 4;// 4 for rgba size

	if (!pixels) {
		throw std::runtime_error("Failed to load texture image");
	}
	

	TextureImageData data{width,height,texChannels,pixels};
	

	
	return data;	


	

	
}

void FileLoader::CloseTextureImage(stbi_uc* pixels)
{
	stbi_image_free(pixels);
}

void FileLoader::LoadModel(std::vector<GraphicsPipelineManager::Vertex>& vertices, std::vector<uint32_t>& indices)
{
	std::unordered_map<GraphicsPipelineManager::Vertex, uint32_t> uniqueVertices{};
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, App::MODEL_PATH.c_str())) {
		throw std::runtime_error(warn + err);
	}

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			GraphicsPipelineManager::Vertex vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2],
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1 - attrib.texcoords[2 * index.texcoord_index + 1] // flip it because of obj format vertical cooridnate being inverse of vulkan
			};

			vertex.color = { 1.0f,1.0f,1.0f };


			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}

			indices.push_back(uniqueVertices[vertex]);

			
		}
	}
}



