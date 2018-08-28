#include "ImageView.h"

namespace VkDemos
{

ImageView *ImageView::createImageViews(VkDevice *device, VkSwapChain *swapChain)
{
    size_t imageCount = swapChain->swapChainImages.size();

    ImageView *imageView = new ImageView;
    imageView->device = device;

    for (size_t i = 0; i != imageCount; i++)
    {
        VkImage vkImage = swapChain->swapChainImages[i];
        VkImageView vkImageView;

        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = vkImage;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChain->surfaceFormat.format;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VkResult operationResult = vkCreateImageView(*device, &createInfo, nullptr, &vkImageView);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to create image views: " + VkHelper::getVkResultDescription(operationResult));

        imageView->views.push_back(vkImageView);
    }

    return imageView;
}

ImageView::~ImageView()
{
    for (auto imageView : views)
        vkDestroyImageView(*device, imageView, nullptr);
}

} // namespace VkDemos