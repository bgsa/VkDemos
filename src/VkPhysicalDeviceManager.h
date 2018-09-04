#ifndef VK_PHYSICAL_DEVICE_MANAGER_HEADER
#define VK_PHYSICAL_DEVICE_MANAGER_HEADER

#include "VkBootstrapHeader.h"
#include "QueueManager.h"

namespace VkBootstrap
{
class VkPhysicalDeviceManager
{
private:
  VkInstance vulkanInstance;

public:
  VkPhysicalDeviceManager(const VkInstance &instance);

  uint32_t getSupportedDevicesCount();
  std::vector<VkPhysicalDevice> getSupportedDevices();
  std::vector<VkPhysicalDevice> findGraphicalDevices();
  VkPhysicalDevice findSuitableGraphicalDevice();
  VkPhysicalDevice findSuitableGraphicalDevice(std::vector<const char *> requiredExtensions);
  VkPhysicalDevice findSuitableGraphicalDevice(std::vector<VkPhysicalDevice> devices, std::vector<const char *> requiredExtensions);
  VkPhysicalDeviceProperties getProperties(const VkPhysicalDevice &device);
  VkPhysicalDeviceFeatures getFeatures(const VkPhysicalDevice &device);
  bool isGpuDevice(const VkPhysicalDevice &device);
  bool isCpuDevice(const VkPhysicalDevice &device);
  bool isVirtualGpuDevice(const VkPhysicalDevice &device);
  static std::string getPhysicalTypeDescription(VkPhysicalDeviceType deviceType);
  static std::vector<VkExtensionProperties> getSupportedExtensions(const VkPhysicalDevice &physicalDevice);
  static bool hasSupportedExtension(const VkPhysicalDevice &physicalDevice, std::string extensionName);
  static bool hasSupportedExtensions(const VkPhysicalDevice &physicalDevice, std::vector<const char *> extensionsName);
  void printSupportedDevices();
  static std::vector<const char *> getRequiredExtensionsForGraphic();
};
}

#endif