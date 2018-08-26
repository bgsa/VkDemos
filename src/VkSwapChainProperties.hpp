#ifndef VK_SWAP_CHAIN_PROPERTIES_HEADER
#define VK_SWAP_CHAIN_PROPERTIES_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class VkSwapChainProperties
{
  public:
    VkSurfaceCapabilitiesKHR capabilities;
    vector<VkSurfaceFormatKHR> formats;
    vector<VkPresentModeKHR> presentModes;

    static VkSwapChainProperties *getSwapChainProperties(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface)
    {
        VkSwapChainProperties *swapChainProperties = new VkSwapChainProperties;

        VkResult operationResult = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &swapChainProperties->capabilities);
        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("fail to get physical device surface capabilities");

        uint32_t formatCount = 0;
        operationResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("fail to get physical device surface formats count");

        if (formatCount != 0)
        {
            swapChainProperties->formats.resize(formatCount);

            operationResult = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, swapChainProperties->formats.data());

            if (operationResult != VK_SUCCESS)
                throw std::runtime_error("fail to get physical device surface formats count");
        }

        uint32_t presentModeCount = 0;
        operationResult = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("fail to get physical device surface modes count");

        if (presentModeCount != 0)
        {
            swapChainProperties->presentModes.resize(presentModeCount);

            operationResult = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, swapChainProperties->presentModes.data());

            if (operationResult != VK_SUCCESS)
                throw std::runtime_error("fail to get physical device surface modes");
        }

        return swapChainProperties;
    }
};
} // namespace VkDemos

#endif