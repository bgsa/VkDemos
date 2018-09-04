#include "VkInstanceLayerConfiguration.h"

namespace VkBootstrap
{

static VkDebugUtilsMessengerEXT debugMessager;

uint32_t VkInstanceLayerConfiguration::getInstanceLayersSupportedCount()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    return layerCount;
}

std::vector<VkLayerProperties> VkInstanceLayerConfiguration::getInstanceLayersSupported()
{
    uint32_t layerCount = getInstanceLayersSupportedCount();

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    return availableLayers;
}

void VkInstanceLayerConfiguration::printInstanceLayersSupported()
{
	std::vector<VkLayerProperties> validationLayers = VkInstanceLayerConfiguration::getInstanceLayersSupported();

    VkLogger::getOutputStream() << validationLayers.size() << " instance layers available:" << std::endl;

    for (const auto &layer : validationLayers)
        VkLogger::info(layer.layerName);
}

bool VkInstanceLayerConfiguration::isInstanceLayerSupported(const std::string &validationLayerName)
{
	std::vector<VkLayerProperties> validationLayers = VkInstanceLayerConfiguration::getInstanceLayersSupported();

    for (const VkLayerProperties &layer : validationLayers)
        if (layer.layerName == validationLayerName)
            return true;

    return false;
}

VkResult VkInstanceLayerConfiguration::createDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator)
{
    auto function = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

    if (function != nullptr)
        return function(instance, pCreateInfo, pAllocator, &debugMessager);
    else
        return VK_ERROR_EXTENSION_NOT_PRESENT;
}

void VkInstanceLayerConfiguration::destroyDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkAllocationCallbacks *pAllocator)
{
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (function != nullptr)
        function(instance, debugMessager, pAllocator);
}

}