#ifndef VK_PHYSICAL_DEVICE_MANAGER_HEADER
#define VK_PHYSICAL_DEVICE_MANAGER_HEADER

#include "VkDemosHeader.h"
#include "VkQueueFamily.h"

namespace VkDemos
{
class VkPhysicalDeviceManager
{
private:
  VkInstance vulkanInstance;

public:
  VkPhysicalDeviceManager(const VkInstance &instance);

  uint32_t getSupportedDevicesCount();
  vector<VkPhysicalDevice> getSupportedDevices();
  vector<VkPhysicalDevice> findGraphicalDevices();
  VkPhysicalDevice findSuitableGraphicalDevice();
  VkPhysicalDevice findSuitableGraphicalDevice(vector<string> requiredExtensions);
  VkPhysicalDevice findSuitableGraphicalDevice(vector<VkPhysicalDevice> devices, vector<string> requiredExtensions);
  VkPhysicalDeviceProperties getProperties(const VkPhysicalDevice &device);
  VkPhysicalDeviceFeatures getFeatures(const VkPhysicalDevice &device);
  bool isGpuDevice(const VkPhysicalDevice &device);
  bool isCpuDevice(const VkPhysicalDevice &device);
  bool isVirtualGpuDevice(const VkPhysicalDevice &device);
  static string getPhysicalTypeDescription(VkPhysicalDeviceType deviceType);
  static vector<VkExtensionProperties> getSupportedExtensions(const VkPhysicalDevice &physicalDevice);
  static bool hasSupportedExtension(const VkPhysicalDevice &physicalDevice, string extensionName);
  static bool hasSupportedExtensions(const VkPhysicalDevice &physicalDevice, vector<string> extensionsName);
  void printSupportedDevices();
  static vector<string> getRequiredExtensionsForGraphic();
};
} // namespace VkDemos

#endif