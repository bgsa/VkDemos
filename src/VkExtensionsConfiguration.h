#ifndef VK_EXTENSIONS_CONFIGURATION_HEADER
#define VK_EXTENSIONS_CONFIGURATION_HEADER

#include "VkDemosHeader.h"

using namespace std;

namespace VkDemos
{
class VkExtensionsConfiguration
{
public:
  static uint32_t getSupportedExtensionsCount();
  static vector<VkExtensionProperties> getSupportedExtensions();
  static bool isExtensionSupported(string extensionName);
  static void printSupportedExtensions();
};
} // namespace VkDemos

#endif