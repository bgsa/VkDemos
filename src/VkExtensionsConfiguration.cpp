#include "VkExtensionsConfiguration.h"

namespace VkDemos
{

uint32_t VkExtensionsConfiguration::getSupportedExtensionsCount()
{
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  return extensionCount;
}

vector<VkExtensionProperties> VkExtensionsConfiguration::getSupportedExtensions()
{
  uint32_t extensionCount = getSupportedExtensionsCount();

  vector<VkExtensionProperties> extensions(extensionCount);

  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  return extensions;
}

bool VkExtensionsConfiguration::isExtensionSupported(string extensionName)
{
  auto extensions = VkExtensionsConfiguration::getSupportedExtensions();

  for (auto extension : extensions)
    if (string(extension.extensionName) == extensionName)
      return true;

  return false;
}

void VkExtensionsConfiguration::printSupportedExtensions()
{
  auto extensions = VkExtensionsConfiguration::getSupportedExtensions();

  VkLogger::getOutputStream() << extensions.size() << " extensions available:" << endl;

  for (auto prop : extensions)
    VkLogger::getOutputStream() << prop.extensionName << " - (Specification Version " << prop.specVersion << ")" << endl;
}

} // namespace VkDemos