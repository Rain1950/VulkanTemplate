#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <vulkan/vulkan.h>
#include <stb_image.h>
#include "App.h"
#include "GraphicsPipeline.h"
#include "Vertex.h"
	struct TextureImageData {
		int width{};
		int height{};
		int texChannels{};
		stbi_uc* pixels = nullptr;
		VkDeviceSize imageSize{};

		TextureImageData(int Width, int Height, int TexChannels, stbi_uc* Pixels) : 
			width{ Width }, 
			height{ Height },
			texChannels{ TexChannels }, 
			pixels{Pixels}
		{
			
			imageSize = width * height * 4; // 4 for rgba byte size
		}

		TextureImageData() {};
	};
 class FileLoader {

public:

	static std::vector<char> ReadShaderFile(const std::string& filename);
	static TextureImageData ReadTextureImage(VkDevice& device,std::string textureFilePath);
	static void CloseTextureImage(stbi_uc* pixels);
	static void LoadModel(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
};
