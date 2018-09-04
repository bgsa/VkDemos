#include "Device.h"

namespace VkBootstrap
{

Device::Device(const VkInstance &vulkanInstance, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
{
    VkPhysicalDeviceManager deviceManager(vulkanInstance);

    physicalDevice = deviceManager.findSuitableGraphicalDevice(extensions);

    createDevices(physicalDevice, surface, extensions);
}

Device::Device(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
{
    createDevices(physicalDevice, surface, extensions);
}

void Device::createDevices(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
{
    this->physicalDevice = physicalDevice;

    uint32_t graphicQueueIndex = QueueManager::getGraphicQueueFamilyIndex(physicalDevice);
    uint32_t presentQueueIndex = QueueManager::getSurfaceQueueFamilyIndex(physicalDevice, surface);
	std::set<uint32_t> queueFamilies = {graphicQueueIndex, presentQueueIndex};

    float queuePriority = 1.0f;

	std::vector<VkDeviceQueueCreateInfo> queueInfos;
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
    createInfo.ppEnabledExtensionNames = extensions.data();
    createInfo.enabledLayerCount = 0;

#if DEBUG
    const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    if (VkInstanceLayerConfiguration::isInstanceLayerSupported(validationLayers[0]))
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
#endif

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
        throw std::runtime_error("failed to create logical device!");

    createQueues(surface);
}

void Device::createQueues(const VkSurfaceKHR &surface)
{
    graphicsQueue = new Queue;
    presentQueue = new Queue;

    graphicsQueue->index = QueueManager::getGraphicQueueFamilyIndex(physicalDevice);
    vkGetDeviceQueue(logicalDevice, graphicsQueue->index, 0, &graphicsQueue->queue);

    presentQueue->index = QueueManager::getSurfaceQueueFamilyIndex(physicalDevice, surface);
    vkGetDeviceQueue(logicalDevice, presentQueue->index, 0, &presentQueue->queue);
}

Device::~Device()
{
    vkDestroyDevice(logicalDevice, nullptr);
    logicalDevice = VK_NULL_HANDLE;
}

}
