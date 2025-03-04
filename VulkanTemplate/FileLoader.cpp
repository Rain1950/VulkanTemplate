#include "FileLoader.h"
#include "GraphicsPipelineManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

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

	TextureImageData data{};
	data.height = height;
	data.width = width;
	data.pixels = pixels;
	return data;


	

	
}

void FileLoader::CloseTextureImage(stbi_uc* pixels)
{
	stbi_image_free(pixels);
}

