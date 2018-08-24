#include "Application.h"

namespace VkDemos
{

void Application::run()
{
    setupWindow();
    setupVulkan();
}

void Application::setupDebugCallback()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = VkLogger::vkDebugCallback;
    createInfo.pUserData = nullptr;

    if (VkValidationLayerConfiguration::createDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr) != VK_SUCCESS)
        throw runtime_error("failed to set up debug callback!");
}

void Application::setupVulkan()
{
    auto extensionsRequired = window->getRequiredExtensions();

#if DEBUG
    VkExtensionsConfiguration::printSupportedExtensions();
    VkValidationLayerConfiguration::printValidationLayersSupported();

    extensionsRequired.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

    char **extensions = VectorHelper::convertToCharArray(extensionsRequired);

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsRequired.size());
    createInfo.ppEnabledExtensionNames = extensions;

#if DEBUG
    const vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    if (VkValidationLayerConfiguration::isValidationLayerSupported(validationLayers[0]))
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
        createInfo.enabledLayerCount = 0;
#endif

    VkResult operationResult = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);

    if (operationResult != VK_SUCCESS)
        throw runtime_error("failed to create vulkan instance!");

#if DEBUG
    setupDebugCallback();
#endif
}

void Application::setupWindow()
{
    VkDemos::WindowInfo windowInfo("VkDemo", 800, 600);

    window = new VkDemos::Window;
    window->setup(windowInfo);
}

void Application::exit()
{
#if DEBUG
    VkValidationLayerConfiguration::destroyDebugUtilsMessengerEXT(vulkanInstance, nullptr);
#endif

    if (vulkanInstance != nullptr)
    {
        vkDestroyInstance(vulkanInstance, nullptr);
        vulkanInstance = nullptr;
    }

    if (window != nullptr)
    {
        delete window;
        window = nullptr;
    }
}

Application::~Application()
{
    exit();
}

} // namespace VkDemos