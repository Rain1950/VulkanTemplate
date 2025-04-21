#pragma once


#include <vulkan/vulkan.h>
#include <vector>

class ValidationLayers {
	
public:
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif
	bool CheckValidationLayerSupport();
	


};