#ifndef VK_EXTENSIONS_CONFIGURATION_HEADER
#define VK_EXTENSIONS_CONFIGURATION_HEADER

#include "VkBootstrapHeader.h"

namespace VkBootstrap
{
class VkExtensionsConfiguration
{
public:
  static uint32_t getSupportedExtensionsCount();
  static std::vector<VkExtensionProperties> getSupportedExtensions();
  static bool isExtensionSupported(std::string extensionName);
  static void printSupportedExtensions();
};
}

#endif