#include "App.h"
#include "FileLoader.h"

void App::Run() {
	window->InitWindow();
	glfwSetWindowUserPointer(window->window, &physicalDevice);
	InitVulkan();
	MainLoop();
	Cleanup();
}

void App::InitVulkan() {
	vulkanInstance->CreateInstance();
	window->CreateSurface();
	physicalDevice.PickPhysicalDevice();
	logicalDevice.CreateLogicalDevice(&physicalDevice, &validationLayers);
	physicalDevice.CreateSwapChain(logicalDevice.device);
	physicalDevice.CreateImageViews(logicalDevice.device);
	graphicsPipeline.CreateRenderPass(physicalDevice.swapChainImageFormat, logicalDevice.device,physicalDevice.physicalDevice);
	graphicsPipeline.CreateDescriptorSetLayout(logicalDevice.device);
	graphicsPipeline.CreateGraphicsPipeline(logicalDevice.device);
	physicalDevice.CreateCommandPool(logicalDevice.device);
	graphicsPipeline.CreateDepthResources(physicalDevice.physicalDevice,logicalDevice.device);
	physicalDevice.CreateFrameBuffers(logicalDevice.device, graphicsPipeline.renderPass,graphicsPipeline.depthImageView);
	graphicsPipeline.CreateTextureImage(logicalDevice.device,physicalDevice.physicalDevice);
	graphicsPipeline.CreateTextureImageView(logicalDevice.device);
	graphicsPipeline.CreateTextureSampler(logicalDevice.device,physicalDevice.physicalDevice);
	FileLoader::LoadModel(graphicsPipeline.vertices, graphicsPipeline.indices);
	graphicsPipeline.CreateVertexBuffer(logicalDevice.device,physicalDevice.physicalDevice);
	graphicsPipeline.CreateIndexBuffer(logicalDevice.device, physicalDevice.physicalDevice);
	graphicsPipeline.CreateUniformBuffers(logicalDevice.device,physicalDevice.physicalDevice);
	graphicsPipeline.CreateDescriptorPool(logicalDevice.device);
	graphicsPipeline.CreateDescriptorSets(logicalDevice.device);
	physicalDevice.CreateCommandBuffers(logicalDevice.device);
	physicalDevice.CreateSyncObjects(logicalDevice.device);

}

void App::DrawFrame()
{
	vkWaitForFences(logicalDevice.device, 1, &physicalDevice.inFlightFences[physicalDevice.currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDevice.device, physicalDevice.swapChain, UINT64_MAX, physicalDevice.imageAvailableSemaphores[physicalDevice.currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		physicalDevice.RecreateSwapChain(logicalDevice.device, graphicsPipeline);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain images");
	}

	vkResetFences(logicalDevice.device, 1, &physicalDevice.inFlightFences[physicalDevice.currentFrame]);

	vkResetCommandBuffer(physicalDevice.commandBuffers[physicalDevice.currentFrame], 0);
	physicalDevice.RecordCommandBuffer(physicalDevice.commandBuffers[physicalDevice.currentFrame], imageIndex, graphicsPipeline);

	graphicsPipeline.UpdateUniformBuffers(physicalDevice.currentFrame);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { physicalDevice.imageAvailableSemaphores[physicalDevice.currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &physicalDevice.commandBuffers[physicalDevice.currentFrame];

	VkSemaphore signalSemaphores[] = { physicalDevice.renderFinishedSemaphores[physicalDevice.currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(**logicalDevice.graphicsQueue, 1, &submitInfo, physicalDevice.inFlightFences[physicalDevice.currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { physicalDevice.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(logicalDevice.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || physicalDevice.frameBufferResized) {
		physicalDevice.frameBufferResized = false;
		physicalDevice.RecreateSwapChain(logicalDevice.device, graphicsPipeline);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image");
	}

	physicalDevice.currentFrame = (physicalDevice.currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;



}

void App::MainLoop()
{

	auto startTime = std::chrono::high_resolution_clock::now();
	int targetFPS = 60;
	while (!glfwWindowShouldClose(window->window)) {
		glfwPollEvents();
		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - startTime).count();
		if (time > 1000/targetFPS) {
			time = 0;
			startTime = std::chrono::high_resolution_clock::now();
			DrawFrame();
		}
	}
	vkDeviceWaitIdle(logicalDevice.device);

}

void App::Cleanup()
{
	physicalDevice.CleanupSyncObjects(logicalDevice.device);
	physicalDevice.CleanupCommandPool(logicalDevice.device);
	physicalDevice.CleanupFrameBuffers(logicalDevice.device);
	graphicsPipeline.CleanGraphicsPipeline(logicalDevice.device);
	graphicsPipeline.CleanPipelineLayout(logicalDevice.device);
	graphicsPipeline.CleanRenderPass(logicalDevice.device);
	physicalDevice.CleanupImageViews(logicalDevice.device);
	physicalDevice.CleanupSwapChain(logicalDevice.device, physicalDevice.swapChain,graphicsPipeline);
	graphicsPipeline.CleanupTextureSampler(logicalDevice.device);
	graphicsPipeline.CleanupTextureView(logicalDevice.device);
	graphicsPipeline.CleanupTextureImage(logicalDevice.device);
	graphicsPipeline.CleanupUniformBuffers(logicalDevice.device);
	graphicsPipeline.CleanupDescriptorSetLayout(logicalDevice.device);
	graphicsPipeline.CleanupIndexBuffer(logicalDevice.device);
	graphicsPipeline.CleanupVertexBuffer(logicalDevice.device);
	

	logicalDevice.Cleanup();
	window->Cleanup();
	vulkanInstance->Cleanup();
}

