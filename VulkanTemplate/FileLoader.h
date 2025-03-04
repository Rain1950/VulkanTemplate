#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <vulkan/vulkan.h>
#include <string>
 class FileLoader {

public:
	static std::vector<char> ReadShaderFile(const std::string& filename);
	static void LoadTextureImage(VkDevice& device,std::string textureFilePath);
	
};
