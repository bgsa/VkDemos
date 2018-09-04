#include "VkExtensionsConfiguration.h"

namespace VkBootstrap
{

uint32_t VkExtensionsConfiguration::getSupportedExtensionsCount()
{
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  return extensionCount;
}

std::vector<VkExtensionProperties> VkExtensionsConfiguration::getSupportedExtensions()
{
  uint32_t extensionCount = getSupportedExtensionsCount();

  std::vector<VkExtensionProperties> extensions(extensionCount);

  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  return extensions;
}

bool VkExtensionsConfiguration::isExtensionSupported(std::string extensionName)
{
  auto extensions = VkExtensionsConfiguration::getSupportedExtensions();

  for (const auto &extension : extensions)
    if (std::string(extension.extensionName) == extensionName)
      return true;

  return false;
}

void VkExtensionsConfiguration::printSupportedExtensions()
{
  std::vector<VkExtensionProperties> extensions = VkExtensionsConfiguration::getSupportedExtensions();

  VkLogger::getOutputStream() << extensions.size() << " extensions available:" << std::endl;

  for (const VkExtensionProperties &prop : extensions)
    VkLogger::getOutputStream() << prop.extensionName << " - (Revision " << prop.specVersion << ")" << std::endl;
}

}