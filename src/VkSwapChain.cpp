#include "VkSwapChain.h"

namespace VkDemos
{

VkSwapChain *VkSwapChain::createSwapChain(const VkPhysicalDevice &physicalDevice, const VkDevice &logicalDevice, const VkSurfaceKHR &surface, const VkQueueFamily &queueFamily)
{
    VkSwapChain *swapChain = new VkSwapChain;

    VkSwapChainProperties *swapChainProperties = VkSwapChainProperties::getSwapChainProperties(physicalDevice, surface);

    if (swapChainProperties->formats.empty() || swapChainProperties->presentModes.empty())
        throw std::runtime_error("swap chain dows not support formats or present modes!");

    swapChain->surfaceFormat = chooseSwapSurfaceFormat(swapChainProperties->formats);
    swapChain->presentMode = chooseSwapPresentMode(swapChainProperties->presentModes);
    swapChain->extent = chooseSwapExtent(swapChainProperties->capabilities);

    uint32_t imageCount = swapChainProperties->capabilities.minImageCount + 1;

    if (swapChainProperties->capabilities.maxImageCount > 0 && imageCount > swapChainProperties->capabilities.maxImageCount)
        imageCount = swapChainProperties->capabilities.maxImageCount;

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
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

    if (queueFamily.graphicsQueueIndex != queueFamily.presentQueueIndex)
    {
        uint32_t queuesFamily[2] = {queueFamily.graphicsQueueIndex, queueFamily.presentQueueIndex};

        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queuesFamily;
    }

    VkResult result = vkCreateSwapchainKHR(logicalDevice, &createInfo, nullptr, &swapChain->vulkanSwapChain);

    if (result != VK_SUCCESS)
    {
        VkLogger::getOutputStream() << "failed to create swap chain: " << VkHelper::getVkResultDescription(result) << endl;
        throw std::runtime_error("failed to create swap chain!");
    }

    delete swapChainProperties;

    return swapChain;
}

VkSurfaceFormatKHR VkSwapChain::chooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR> &surfaceFormats)
{
    if (surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
    {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
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

VkPresentModeKHR VkSwapChain::chooseSwapPresentMode(const vector<VkPresentModeKHR> &presentModes)
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

VkExtent2D VkSwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &surfacecapabilities)
{
    if (surfacecapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return surfacecapabilities.currentExtent;
    }
    else
    {
        VkExtent2D actualExtent = {800, 600};
        actualExtent.width = std::max(surfacecapabilities.minImageExtent.width, std::min(surfacecapabilities.maxImageExtent.width, actualExtent.width));
        actualExtent.height = std::max(surfacecapabilities.minImageExtent.height, std::min(surfacecapabilities.maxImageExtent.height, actualExtent.height));

        return actualExtent;
    }
}

} // namespace VkDemos