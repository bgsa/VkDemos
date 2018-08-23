#include "VkExtensionsConfiguration.h"

namespace VkDemos
{

vector<VkExtensionProperties> VkExtensionsConfiguration::getSupportedExtensions()
{
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

  vector<VkExtensionProperties> extensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

  return extensions;
}

void VkExtensionsConfiguration::printSupportedExtensions()
{
  auto extensions = VkExtensionsConfiguration::getSupportedExtensions();

  cout << extensions.size() << " extensions found:" << endl;

  for (auto prop : extensions)
  {
    cout << prop.extensionName << " - (Specification Version " << prop.specVersion << ")" << endl;
  }
}

} // namespace VkDemos