#include "SwapChain.h"

namespace VkBootstrap
{

	SwapChain *SwapChain::createSwapChain(const Device *device, const VkSurfaceKHR &surface, Window *window)
	{
		SwapChain *swapChain = new SwapChain;
		swapChain->device = device->logicalDevice;

		SwapChainProperties *swapChainProperties = SwapChainProperties::getSwapChainProperties(device->physicalDevice, surface);

		if (swapChainProperties->formats.empty() || swapChainProperties->presentModes.empty())
			throw std::runtime_error("swap chain dows not support formats or present modes!");

		swapChain->surfaceFormat = chooseSwapSurfaceFormat(swapChainProperties->formats);
		swapChain->presentMode = chooseSwapPresentMode(swapChainProperties->presentModes);
		swapChain->extent = chooseSwapExtent(swapChainProperties->capabilities, window);

		uint32_t numberOfImage = swapChainProperties->capabilities.minImageCount + 1;

		if (swapChainProperties->capabilities.maxImageCount > 0 && numberOfImage > swapChainProperties->capabilities.maxImageCount)
			numberOfImage = swapChainProperties->capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.flags = 0;
		createInfo.pNext = nullptr;
		createInfo.surface = surface;
		createInfo.minImageCount = numberOfImage;
		createInfo.imageFormat = swapChain->surfaceFormat.format;
		createInfo.imageColorSpace = swapChain->surfaceFormat.colorSpace;
		createInfo.imageExtent = swapChain->extent;
		createInfo.presentMode = swapChain->presentMode;
		createInfo.preTransform = swapChainProperties->capabilities.currentTransform;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		
		uint32_t* queuesFamily = nullptr;

		std::set<uint32_t> queueFamilySet = {
			device->queueManager->getGraphicQueueFamily()->getIndex(),
			device->queueManager->getPresentationQueueFamily()->getIndex(),
			device->queueManager->getTransferQueueFamily()->getIndex()
		};

		if (queueFamilySet.size() > 1) 
		{
			queuesFamily = new uint32_t[queueFamilySet.size()];

			size_t counter = 0;
			for (uint32_t index : queueFamilySet) 
			{
				queuesFamily[counter] = index;
				counter++;
			}
			
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = (uint32_t) queueFamilySet.size();
			createInfo.pQueueFamilyIndices = queuesFamily;
		}

		VkResult result = vkCreateSwapchainKHR(device->logicalDevice, &createInfo, nullptr, &swapChain->vulkanSwapChain);
		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to create swap chain: " + VkHelper::getVkResultDescription(result));

		delete queuesFamily;
		delete swapChainProperties;

		result = vkGetSwapchainImagesKHR(device->logicalDevice, swapChain->vulkanSwapChain, &numberOfImage, nullptr);
		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to get images count from swap chain: " + VkHelper::getVkResultDescription(result));

		swapChain->swapChainImages.resize(numberOfImage);

		result = vkGetSwapchainImagesKHR(device->logicalDevice, swapChain->vulkanSwapChain, &numberOfImage, swapChain->swapChainImages.data());

		if (result != VK_SUCCESS)
			throw std::runtime_error("failed to get images from swap chain: " + VkHelper::getVkResultDescription(result));

		swapChain->createImageViews(device->logicalDevice);
		swapChain->createRenderPass(device->logicalDevice);
		swapChain->createFramebuffers(device->logicalDevice);

		return swapChain;
	}

	void SwapChain::createImageViews(const VkDevice &device)
	{
		size_t imageCount = swapChainImages.size();

		for (size_t i = 0; i != imageCount; i++)
		{
			VkImage vkImage = swapChainImages[i];
			VkImageView vkImageView;

			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = vkImage;
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = surfaceFormat.format;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkResult operationResult = vkCreateImageView(device, &createInfo, nullptr, &vkImageView);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to create image views: " + VkHelper::getVkResultDescription(operationResult));

			imageViews.push_back(vkImageView);
		}
	}

	void SwapChain::createRenderPass(const VkDevice &device)
	{
		//create render pass
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = surfaceFormat.format;
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VkResult operationResult = vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to create render pass: " + VkHelper::getVkResultDescription(operationResult));
	}

	VkRenderPassBeginInfo SwapChain::getRenderPassBegin(size_t framebufferIndex)
	{
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = renderPass;
		renderPassInfo.framebuffer = framebuffers[framebufferIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = extent;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		return renderPassInfo;
	}

	void SwapChain::createFramebuffers(const VkDevice &device)
	{
		framebuffers.resize(imageViews.size());

		for (size_t i = 0; i < imageViews.size(); i++)
		{
			VkImageView attachments[] = { imageViews[i] };
			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = extent.width;
			framebufferInfo.height = extent.height;
			framebufferInfo.layers = 1;

			VkResult operationResult = vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to create framebuffer!");
		}
	}

	VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &surfaceFormats)
	{
		if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
		{
			return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
		}

		for (const auto &availableFormat : surfaceFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return availableFormat;
			}
		}

		return surfaceFormats[0];
	}

	VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes)
	{
		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

		for (const auto &availablePresentMode : presentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				return availablePresentMode;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
			{
				bestMode = availablePresentMode;
			}
		}

		return bestMode;
	}
	VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfacecapabilities, Window *window)
	{
		if (surfacecapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return surfacecapabilities.currentExtent;
		}
		else
		{
			Size windowSize = window->getSize();
			VkExtent2D actualExtent = { (uint32_t)windowSize.width, (uint32_t)windowSize.height };
			actualExtent.width = std::max(surfacecapabilities.minImageExtent.width, std::min(surfacecapabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(surfacecapabilities.minImageExtent.height, std::min(surfacecapabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	SwapChain::~SwapChain()
	{
		for (VkFramebuffer framebuffer : framebuffers)
			vkDestroyFramebuffer(device, framebuffer, nullptr);

		vkDestroyRenderPass(device, renderPass, nullptr);

		for (VkImageView imageView : imageViews)
			vkDestroyImageView(device, imageView, nullptr);

		if (vulkanSwapChain != VK_NULL_HANDLE)
			vkDestroySwapchainKHR(device, vulkanSwapChain, nullptr);
	}

}
