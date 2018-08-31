#include "QueueManager.h"

namespace VkDemos
{

vector<VkQueueFamilyProperties> QueueManager::getQueueFamilies(const VkPhysicalDevice &device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    return queueFamilies;
}

bool QueueManager::hasGraphicQueueFamily(vector<VkQueueFamilyProperties> queueFamilies)
{
    for (const auto &queueFamily : queueFamilies)
        if (isGraphicQueueFamily(queueFamily))
            return true;

    return false;
}

bool QueueManager::hasGraphicQueueFamily(const VkPhysicalDevice &device)
{
    vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilies(device);

    for (const auto &queueFamily : queueFamilies)
        if (isGraphicQueueFamily(queueFamily))
            return true;

    return false;
}

bool QueueManager::isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily)
{
    return (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT);
}

uint32_t QueueManager::getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice)
{
    vector<VkQueueFamilyProperties> queueFamilies = QueueManager::getQueueFamilies(physicalDevice);

    for (uint32_t i = 0; i != queueFamilies.size(); i++)
        if (QueueManager::isGraphicQueueFamily(queueFamilies[i]))
            return i;

    return -1;
}

uint32_t QueueManager::getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface)
{
    vector<VkQueueFamilyProperties> queueFamilies = QueueManager::getQueueFamilies(physicalDevice);

    for (uint32_t i = 0; i != queueFamilies.size(); i++)
    {
        if (queueFamilies[i].queueCount == 0)
            return false;

        VkBool32 presentSupport = false;
        VkResult operationResult = vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to get physical device surface support");

        if (presentSupport)
            return i;
    }

    return -1;
}

} // namespace VkDemos