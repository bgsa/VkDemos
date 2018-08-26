#include "VkLogicalDevice.h"

namespace VkDemos
{

VkDevice *VkLogicalDevice::createLogicalDevice(const VkInstance &vulkanInstance, VkSurfaceKHR *surface, vector<string> extensions)
{
    VkPhysicalDeviceManager deviceManager(vulkanInstance);

    VkPhysicalDevice physicalDevice = deviceManager.findSuitableGraphicalDevice(extensions);

    VkDevice *device = createLogicalDevice(physicalDevice, surface, extensions);

    return device;
}

VkDevice *VkLogicalDevice::createLogicalDevice(const VkPhysicalDevice &physicalDevice, VkSurfaceKHR *surface, vector<string> extensions)
{
    uint32_t graphicQueueIndex = VkQueueFamily::getGraphicQueueFamilyIndex(physicalDevice);
    uint32_t presentQueueIndex = VkQueueFamily::getSurfaceQueueFamilyIndex(physicalDevice, surface);
    set<uint32_t> queueFamilies = {graphicQueueIndex, presentQueueIndex};

    char **extensionsAsArray = VectorHelper::convertToCharArray(extensions);

    float queuePriority = 1.0f;

    vector<VkDeviceQueueCreateInfo> queueInfos;
    for (int queueFamily : queueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeaturesRequired = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeaturesRequired;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    createInfo.pQueueCreateInfos = queueInfos.data();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensionsAsArray;

#if DEBUG
    const vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    if (VkValidationLayerConfiguration::isValidationLayerSupported(validationLayers[0]))
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;
#endif

    VkDevice *device = new VkDevice;

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, device) != VK_SUCCESS)
        throw runtime_error("failed to create logical device!");

    //vkDestroyDevice(*device, nullptr);

    return device;
}

} // namespace VkDemos