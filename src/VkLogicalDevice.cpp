#include "VkLogicalDevice.h"

namespace VkDemos
{

VkDevice *VkLogicalDevice::createLogicalDevice(const VkInstance &vulkanInstance, VkSurfaceKHR *surface)
{
    VkPhysicalDeviceManager deviceManager(vulkanInstance);

    VkPhysicalDevice physicalDevice = deviceManager.findSuitableGraphicalDevice();

    VkDevice *device = createLogicalDevice(physicalDevice, surface);

    return device;
}

VkDevice *VkLogicalDevice::createLogicalDevice(const VkPhysicalDevice &physicalDevice, VkSurfaceKHR *surface)
{
    uint32_t graphicQueueIndex = VkQueueFamily::getGraphicQueueFamilyIndex(physicalDevice);
    uint32_t presentQueueIndex = VkQueueFamily::getSurfaceQueueFamilyIndex(physicalDevice, surface);
    set<uint32_t> queueFamilies = {graphicQueueIndex, presentQueueIndex};

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
    /*
    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    */

    VkPhysicalDeviceFeatures deviceFeaturesRequired = {};

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeaturesRequired;
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
    createInfo.pQueueCreateInfos = queueInfos.data();
    //createInfo.pQueueCreateInfos = &queueCreateInfo;

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