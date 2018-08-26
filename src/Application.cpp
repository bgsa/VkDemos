#include "Application.h"

#include "VkQueueFamily.h"

namespace VkDemos
{

void Application::run()
{
    setupWindow();
    setupVulkan();
    setupSurface();

    VkPhysicalDeviceManager deviceManager(vulkanInstance);
    vector<const char *> requiredExtensions = VkPhysicalDeviceManager::getRequiredExtensionsForGraphic();

    VkPhysicalDevice physicalDevice = deviceManager.findSuitableGraphicalDevice();

    device = VkLogicalDevice::createLogicalDevice(physicalDevice, surface, requiredExtensions);

    //create queue families
    queueFamily = VkQueueFamily::createQueueFamily(physicalDevice, *device, surface);

    //cria swap chain
    swapChain = VkSwapChain::createSwapChain(physicalDevice, *device, surface, *queueFamily);
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

void Application::setupSurface()
{
    window->createSurface(vulkanInstance, &surface);
}

void Application::setupVulkan()
{
    std::vector<const char *> extensionsRequired = window->getRequiredExtensions();

#if DEBUG
    VkExtensionsConfiguration::printSupportedExtensions();
    window->printRequiredExtensions();
    VkValidationLayerConfiguration::printInstanceLayersSupported();

    extensionsRequired.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

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
    createInfo.ppEnabledExtensionNames = extensionsRequired.data();
    createInfo.enabledLayerCount = 0;

#if DEBUG
    const vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    if (VkValidationLayerConfiguration::isInstanceLayerSupported(validationLayers[0]))
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
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

    if (swapChain != nullptr)
    {
        vkDestroySwapchainKHR(*device, swapChain->vulkanSwapChain, nullptr);
        swapChain = nullptr;
    }

    if (queueFamily != nullptr)
    {
        delete queueFamily;
        queueFamily = nullptr;
    }

    if (device != nullptr)
    {
        vkDestroyDevice(*device, nullptr);
        device = nullptr;
    }

    //if (surface != nullptr)
    {
        vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
        //surface = nullptr;
    }

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