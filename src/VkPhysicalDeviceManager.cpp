#include "VkPhysicalDeviceManager.h"

namespace VkDemos
{

VkPhysicalDeviceManager::VkPhysicalDeviceManager(const VkInstance &instance)
{
    vulkanInstance = instance;
}

uint32_t VkPhysicalDeviceManager::getSupportedDevicesCount()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, nullptr);

    return deviceCount;
}

vector<VkPhysicalDevice> VkPhysicalDeviceManager::getSupportedDevices()
{
    uint32_t deviceCount = getSupportedDevicesCount();

    vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vulkanInstance, &deviceCount, devices.data());

    return devices;
}

vector<VkPhysicalDevice> VkPhysicalDeviceManager::findGraphicalDevices()
{
    vector<VkPhysicalDevice> devices = getSupportedDevices();
    vector<VkPhysicalDevice> graphicalDevices;

    for (const VkPhysicalDevice &device : devices)
    {
        VkPhysicalDeviceFeatures deviceFeatures = getFeatures(device);
        bool hasGraphicalSupport = VkQueueFamily::hasGraphicQueueFamily(device);

        if (deviceFeatures.geometryShader && hasGraphicalSupport)
            graphicalDevices.push_back(device);
    }

    return graphicalDevices;
}

VkPhysicalDevice VkPhysicalDeviceManager::findSuitableGraphicalDevice(vector<VkPhysicalDevice> devices)
{
    size_t suitableDevice = -1;

    for (size_t i = 0; i != devices.size(); i++)
    {
        VkPhysicalDevice device = devices[i];
        VkPhysicalDeviceProperties deviceProperties = getProperties(device);

        if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            suitableDevice = i;
        }
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        {
            if (suitableDevice <= 500)
                suitableDevice = 500;
        }
        else if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
        {
            if (suitableDevice <= 250)
                suitableDevice = 250;
        }
        else if (suitableDevice <= 100)
            suitableDevice = 100;
    }

    return devices[suitableDevice];
}

VkPhysicalDevice VkPhysicalDeviceManager::findSuitableGraphicalDevice()
{
    vector<VkPhysicalDevice> devices = findGraphicalDevices();
    VkPhysicalDevice device = findSuitableGraphicalDevice(devices);

    return device;
}

void VkPhysicalDeviceManager::printSupportedDevices()
{
    vector<VkPhysicalDevice> devices = getSupportedDevices();

    VkLogger::info("Devices:");
    for (const VkPhysicalDevice &device : devices)
    {
        VkPhysicalDeviceProperties properties = getProperties(device);
        VkLogger::getOutputStream() << "DeviceID (" << properties.deviceID << ") - "
                                    << VkPhysicalDeviceManager::getPhysicalTypeDescription(properties.deviceType) << " - "
                                    << properties.deviceName;
    }
}

VkPhysicalDeviceProperties VkPhysicalDeviceManager::getProperties(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties deviceProperties;

    vkGetPhysicalDeviceProperties(device, &deviceProperties);

    return deviceProperties;
}

VkPhysicalDeviceFeatures VkPhysicalDeviceManager::getFeatures(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceFeatures deviceFeatures;

    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceFeatures;
}

bool VkPhysicalDeviceManager::isGpuDevice(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties deviceProperties = getProperties(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
           deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU ||
           deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
}

bool VkPhysicalDeviceManager::isCpuDevice(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties deviceProperties = getProperties(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU;
}

bool VkPhysicalDeviceManager::isVirtualGpuDevice(const VkPhysicalDevice &device)
{
    VkPhysicalDeviceProperties deviceProperties = getProperties(device);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
}

string VkPhysicalDeviceManager::getPhysicalTypeDescription(VkPhysicalDeviceType deviceType)
{
    if (deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        return "Discrete GPU";
    else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        return "Integrated GPU";
    else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
        return "Virtual GPU";
    else if (deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
        return "CPU";
    else
        return "Unknown";
}

} // namespace VkDemos