#include "Application.h"

namespace VkDemos
{

void Application::run()
{
    setupWindow();
    setupVulkan();
    setupSurface();
    setupDevice();

    swapChain = SwapChain::createSwapChain(device, surface);

    Shader *shader = Shader::createShader(device, "shaders/vert.spv", "shaders/frag.spv");

    Viewport *viewport = new Viewport(800, 600); // SETAR VIEWPORT no window setup ?!

    graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport);

    CommandManager::init(device, swapChain, graphicPipeline);
    commandManager = CommandManager::getInstance();

    Command *command = commandManager->createCommand(graphicPipeline, swapChain);
    command->begin();

    for (VkCommandBuffer commandBuffer : command->commandBuffers)
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    command->end();

    setupSemaphores();

    //setup for render looping
    VkSwapchainKHR swapChains[] = {swapChain->vulkanSwapChain};
    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    //main loop
    while (isRunning)
    {
        window->update(0);

        uint32_t currentBufferIndex;
        vkAcquireNextImageKHR(device->logicalDevice, swapChain->vulkanSwapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore, VK_NULL_HANDLE, &currentBufferIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &command->commandBuffers[currentBufferIndex];

        VkResult operationResult = vkQueueSubmit(device->graphicsQueue->queue, 1, &submitInfo, VK_NULL_HANDLE);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer: " + VkHelper::getVkResultDescription(operationResult));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &currentBufferIndex;
        presentInfo.pResults = nullptr; // Optional

        operationResult = vkQueuePresentKHR(device->presentQueue->queue, &presentInfo);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to present command buffer: " + VkHelper::getVkResultDescription(operationResult));
    }

    vkDeviceWaitIdle(device->logicalDevice);

    delete command;

    vkDestroySemaphore(device->logicalDevice, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(device->logicalDevice, imageAvailableSemaphore, nullptr);

    delete shader;
}

void Application::setupDevice()
{
    vector<const char *> requiredExtensions = VkPhysicalDeviceManager::getRequiredExtensionsForGraphic();
    device = new Device(vulkanInstance, surface, requiredExtensions);
}

void Application::setupSurface()
{
    window->createSurface(vulkanInstance, &surface);
}

void Application::setupSemaphores()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkResult operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create imageAvailableSemaphores!");

    operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore);
    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create renderFinishedSemaphore!");
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
    window->addHandler(this);
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

void Application::window_OnClose()
{
    isRunning = false;
}

void Application::window_OnResize(int width, int height)
{
    ;
}

void Application::exit()
{
#if DEBUG
    VkValidationLayerConfiguration::destroyDebugUtilsMessengerEXT(vulkanInstance, nullptr);
#endif

    if (commandManager != nullptr)
    {
        delete commandManager;
        commandManager = nullptr;
    }

    if (graphicPipeline != nullptr)
    {
        delete graphicPipeline;
        graphicPipeline = nullptr;
    }

    if (swapChain != nullptr)
    {
        delete swapChain;
        swapChain = nullptr;
    }

    if (device != nullptr)
    {
        delete device;
        device = nullptr;
    }

    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
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