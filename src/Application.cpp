#include "Application.h"

namespace VkBootstrap
{

const int MAX_FRAMEBUFFER = 2;

void Application::run()
{
    setupWindow();
    setupVulkan();
    setupSurface();
    setupDevice();
    setupSyncObjects();

    swapChain = SwapChain::createSwapChain(device, surface, window);

	Shader *shader = Shader::createShader(device, "resources/shaders/vert.spv", "resources/shaders/frag.spv");

    Size windowSize = window->getSize();
    Viewport *viewport = new Viewport(windowSize.width, windowSize.height); // SETAR VIEWPORT no window setup ?!

    graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport);

    CommandManager::init(device, swapChain, graphicPipeline);
    commandManager = CommandManager::getInstance();

    Command *command = commandManager->createCommand(graphicPipeline, swapChain);
    command->begin();

    for (VkCommandBuffer commandBuffer : command->commandBuffers)
        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

    command->end();

    size_t currentFrame = 0;

    while (isRunning)
    {
        window->update(0);

        vkWaitForFences(device->logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(device->logicalDevice, 1, &inFlightFences[currentFrame]);

        VkSwapchainKHR swapChains[] = {swapChain->vulkanSwapChain};
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore[currentFrame]};
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphore[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device->logicalDevice, swapChain->vulkanSwapChain, std::numeric_limits<uint64_t>::max(), imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &command->commandBuffers[imageIndex];

        VkResult operationResult = vkQueueSubmit(device->graphicsQueue->queue, 1, &submitInfo, inFlightFences[currentFrame]);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to submit draw command buffer: " + VkHelper::getVkResultDescription(operationResult));

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        operationResult = vkQueuePresentKHR(device->presentQueue->queue, &presentInfo);

        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to present command buffer: " + VkHelper::getVkResultDescription(operationResult));

        currentFrame = (currentFrame + 1) % MAX_FRAMEBUFFER;

        vkQueueWaitIdle(device->presentQueue->queue);
    }

    vkDeviceWaitIdle(device->logicalDevice);

    delete command;
    delete shader;
}

void Application::setupDevice()
{
	std::vector<const char *> requiredExtensions = VkPhysicalDeviceManager::getRequiredExtensionsForGraphic();
    device = new Device(vulkanInstance, surface, requiredExtensions);
}

void Application::setupSurface()
{
    window->createSurface(vulkanInstance, &surface);
}

void Application::setupSyncObjects()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    imageAvailableSemaphore.resize(MAX_FRAMEBUFFER);
    renderFinishedSemaphore.resize(MAX_FRAMEBUFFER);
    inFlightFences.resize(MAX_FRAMEBUFFER);

    VkResult operationResult;

    for (size_t i = 0; i != MAX_FRAMEBUFFER; i++)
    {
        operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]);
        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to create imageAvailableSemaphores!");

        operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]);
        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to create renderFinishedSemaphore!");

        operationResult = vkCreateFence(device->logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]);
        if (operationResult != VK_SUCCESS)
            throw std::runtime_error("failed to create fence in flight!");
    }
}

void Application::setupVulkan()
{
    std::vector<const char *> extensionsRequired = window->getRequiredExtensions();

#if DEBUG
    VkExtensionsConfiguration::printSupportedExtensions();
    window->printRequiredExtensions();
    VkInstanceLayerConfiguration::printInstanceLayersSupported();

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
    const std::vector<const char *> validationLayers = {"VK_LAYER_LUNARG_standard_validation"};

    if (VkInstanceLayerConfiguration::isInstanceLayerSupported(validationLayers[0]))
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
#endif

    VkResult operationResult = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);

    if (operationResult != VK_SUCCESS)
        throw std::runtime_error("failed to create vulkan instance!");

#if DEBUG
    setupDebugCallback();
#endif
}

void Application::setupWindow()
{
    VkBootstrap::WindowInfo windowInfo("VkDemo", 800, 600);
    windowInfo.setResizable(true);

    window = new VkBootstrap::Window;
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

    if (VkInstanceLayerConfiguration::createDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr) != VK_SUCCESS)
        throw std::runtime_error("failed to set up debug callback!");
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
	VkInstanceLayerConfiguration::destroyDebugUtilsMessengerEXT(vulkanInstance, nullptr);
#endif

    if (commandManager != nullptr)
    {
        delete commandManager;
        commandManager = nullptr;
    }

    for (VkSemaphore semaphore : renderFinishedSemaphore)
        vkDestroySemaphore(device->logicalDevice, semaphore, nullptr);

    for (VkSemaphore semaphore : imageAvailableSemaphore)
        vkDestroySemaphore(device->logicalDevice, semaphore, nullptr);

    for (VkFence fence : inFlightFences)
        vkDestroyFence(device->logicalDevice, fence, nullptr);

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

}