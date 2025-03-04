#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <vulkan/vulkan.h>
#include <string>
#include <stb_image.h>
	struct TextureImageData {
		int width;
		int height;
		int texChannels;
		stbi_uc* pixels;
	};
 class FileLoader {

public:

	static std::vector<char> ReadShaderFile(const std::string& filename);
	static TextureImageData ReadTextureImage(VkDevice& device,std::string textureFilePath);
	static void CloseTextureImage(stbi_uc* pixels);
};
