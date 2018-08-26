#include "VkQueueFamily.h"

namespace VkDemos
{

vector<VkQueueFamilyProperties> VkQueueFamily::getQueueFamilies(const VkPhysicalDevice &device)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    return queueFamilies;
}

bool VkQueueFamily::hasGraphicQueueFamily(vector<VkQueueFamilyProperties> queueFamilies)
{
    for (const auto &queueFamily : queueFamilies)
        if (isGraphicQueueFamily(queueFamily))
            return true;

    return false;
}

bool VkQueueFamily::hasGraphicQueueFamily(const VkPhysicalDevice &device)
{
    vector<VkQueueFamilyProperties> queueFamilies = getQueueFamilies(device);

    for (const auto &queueFamily : queueFamilies)
        if (isGraphicQueueFamily(queueFamily))
            return true;

    return false;
}

bool VkQueueFamily::isGraphicQueueFamily(const VkQueueFamilyProperties &queueFamily)
{
    return (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT);
}

uint32_t VkQueueFamily::getGraphicQueueFamilyIndex(const VkPhysicalDevice &physicalDevice)
{
    vector<VkQueueFamilyProperties> queueFamilies = VkQueueFamily::getQueueFamilies(physicalDevice);

    for (uint32_t i = 0; i != queueFamilies.size(); i++)
        if (VkQueueFamily::isGraphicQueueFamily(queueFamilies[i]))
            return i;

    return -1;
}

uint32_t VkQueueFamily::getSurfaceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, VkSurfaceKHR *surface)
{
    vector<VkQueueFamilyProperties> queueFamilies = VkQueueFamily::getQueueFamilies(physicalDevice);

    VkLogger::getOutputStream() << "queue size: " << queueFamilies.size();

    for (uint32_t i = 0; i != queueFamilies.size(); i++)
    {
        if (queueFamilies[i].queueCount == 0)
            return false;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, *surface, &presentSupport);

        if (presentSupport)
            return i;
    }

    return -1;
}

} // namespace VkDemos