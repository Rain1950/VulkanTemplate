#include "App.h"


void App::Run() {
	windowManager->InitWindow();
	glfwSetWindowUserPointer(windowManager->window, &physicalDeviceManager);
	InitVulkan();
	MainLoop();
	Cleanup();
}

void App::InitVulkan() {
	vulkanInstance->CreateInstance();
	windowManager->CreateSurface();
	physicalDeviceManager.PickPhysicalDevice();
	logicalDeviceManager.CreateLogicalDevice(&physicalDeviceManager, &validationLayersManager);
	physicalDeviceManager.CreateSwapChain(logicalDeviceManager.device);
	physicalDeviceManager.CreateImageViews(logicalDeviceManager.device);
	graphicsPipelineManager.CreateRenderPass(physicalDeviceManager.swapChainImageFormat, logicalDeviceManager.device);
	graphicsPipelineManager.CreateDescriptorSetLayout(logicalDeviceManager.device);
	graphicsPipelineManager.CreateGraphicsPipeline(logicalDeviceManager.device);
	physicalDeviceManager.CreateFrameBuffers(logicalDeviceManager.device, graphicsPipelineManager.renderPass);
	physicalDeviceManager.CreateCommandPool(logicalDeviceManager.device);
	graphicsPipelineManager.CreateTextureImage(logicalDeviceManager.device,physicalDeviceManager.physicalDevice);
	graphicsPipelineManager.CreateVertexBuffer(logicalDeviceManager.device,physicalDeviceManager.physicalDevice);
	graphicsPipelineManager.CreateIndexBuffer(logicalDeviceManager.device, physicalDeviceManager.physicalDevice);
	graphicsPipelineManager.CreateUniformBuffers(logicalDeviceManager.device, physicalDeviceManager.physicalDevice, physicalDeviceManager.MAX_FRAMES_IN_FLIGHT);
	graphicsPipelineManager.CreateDescriptorPool(logicalDeviceManager.device, physicalDeviceManager.MAX_FRAMES_IN_FLIGHT);
	graphicsPipelineManager.CreateDescriptorSets(logicalDeviceManager.device,physicalDeviceManager.MAX_FRAMES_IN_FLIGHT);
	physicalDeviceManager.CreateCommandBuffers(logicalDeviceManager.device);
	physicalDeviceManager.CreateSyncObjects(logicalDeviceManager.device);

}

void App::DrawFrame()
{
	vkWaitForFences(logicalDeviceManager.device, 1, &physicalDeviceManager.inFlightFences[physicalDeviceManager.currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(logicalDeviceManager.device, physicalDeviceManager.swapChain, UINT64_MAX, physicalDeviceManager.imageAvailableSemaphores[physicalDeviceManager.currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		physicalDeviceManager.RecreateSwapChain(logicalDeviceManager.device, graphicsPipelineManager.renderPass);
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("Failed to acquire swap chain images");
	}

	vkResetFences(logicalDeviceManager.device, 1, &physicalDeviceManager.inFlightFences[physicalDeviceManager.currentFrame]);

	vkResetCommandBuffer(physicalDeviceManager.commandBuffers[physicalDeviceManager.currentFrame], 0);
	physicalDeviceManager.RecordCommandBuffer(physicalDeviceManager.commandBuffers[physicalDeviceManager.currentFrame], imageIndex, graphicsPipelineManager);

	graphicsPipelineManager.UpdateUniformBuffers(physicalDeviceManager.currentFrame);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { physicalDeviceManager.imageAvailableSemaphores[physicalDeviceManager.currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &physicalDeviceManager.commandBuffers[physicalDeviceManager.currentFrame];

	VkSemaphore signalSemaphores[] = { physicalDeviceManager.renderFinishedSemaphores[physicalDeviceManager.currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(**logicalDeviceManager.graphicsQueue, 1, &submitInfo, physicalDeviceManager.inFlightFences[physicalDeviceManager.currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("Failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { physicalDeviceManager.swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(logicalDeviceManager.presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || physicalDeviceManager.frameBufferResized) {
		physicalDeviceManager.frameBufferResized = false;
		physicalDeviceManager.RecreateSwapChain(logicalDeviceManager.device, graphicsPipelineManager.renderPass);
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("Failed to present swap chain image");
	}

	physicalDeviceManager.currentFrame = (physicalDeviceManager.currentFrame + 1) % physicalDeviceManager.MAX_FRAMES_IN_FLIGHT;



}

void App::MainLoop()
{
	while (!glfwWindowShouldClose(windowManager->window)) {
		glfwPollEvents();
		DrawFrame();
	}
	vkDeviceWaitIdle(logicalDeviceManager.device);

}

void App::Cleanup()
{
	physicalDeviceManager.CleanupSyncObjects(logicalDeviceManager.device);
	physicalDeviceManager.CleanupCommandPool(logicalDeviceManager.device);
	physicalDeviceManager.CleanupFrameBuffers(logicalDeviceManager.device);
	graphicsPipelineManager.CleanGraphicsPipeline(logicalDeviceManager.device);
	graphicsPipelineManager.CleanPipelineLayout(logicalDeviceManager.device);
	graphicsPipelineManager.CleanRenderPass(logicalDeviceManager.device);
	physicalDeviceManager.CleanupImageViews(logicalDeviceManager.device);
	physicalDeviceManager.CleanupSwapChain(logicalDeviceManager.device, physicalDeviceManager.swapChain);
	graphicsPipelineManager.CleanupUniformBuffers(logicalDeviceManager.device, physicalDeviceManager.MAX_FRAMES_IN_FLIGHT);
	graphicsPipelineManager.CleanupDescriptorSetLayout(logicalDeviceManager.device);
	graphicsPipelineManager.CleanupIndexBuffer(logicalDeviceManager.device);
	graphicsPipelineManager.CleanupVertexBuffer(logicalDeviceManager.device);
	

	logicalDeviceManager.Cleanup();
	windowManager->Cleanup();
	vulkanInstance->Cleanup();
}

