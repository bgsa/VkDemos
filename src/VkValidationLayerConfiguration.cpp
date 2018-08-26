#include "VkValidationLayerConfiguration.h"

namespace VkDemos
{
static VkDebugUtilsMessengerEXT debugMessager;

uint32_t VkValidationLayerConfiguration::getValidationLayersSupportedCount()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    return layerCount;
}

vector<VkLayerProperties> VkValidationLayerConfiguration::getValidationLayersSupported()
{
    uint32_t layerCount = getValidationLayersSupportedCount();

    vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    return availableLayers;
}

void VkValidationLayerConfiguration::printValidationLayersSupported()
{
    vector<VkLayerProperties> validationLayers = VkValidationLayerConfiguration::getValidationLayersSupported();

    VkLogger::getOutputStream() << validationLayers.size() << " validation layers available:" << endl;

    for (const auto &layer : validationLayers)
        VkLogger::info(layer.layerName);
}

bool VkValidationLayerConfiguration::isValidationLayerSupported(const string &validationLayerName)
{
    vector<VkLayerProperties> validationLayers = VkValidationLayerConfiguration::getValidationLayersSupported();

    for (const auto &layer : validationLayers)
        if (layer.layerName == validationLayerName)
            return true;

    return false;
}

VkResult VkValidationLayerConfiguration::createDebugUtilsMessengerEXT(
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

void VkValidationLayerConfiguration::destroyDebugUtilsMessengerEXT(
    VkInstance instance,
    const VkAllocationCallbacks *pAllocator)
{
    auto function = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

    if (function != nullptr)
        function(instance, debugMessager, pAllocator);
}

} // namespace VkDemos