#include "Window.h"

#include <glfw/glfw3native.h>

namespace VkDemos
{

void Window::createSurface(const VkInstance &vulkanInstance, VkSurfaceKHR *surface)
{
    VkResult operationResult = glfwCreateWindowSurface(vulkanInstance, windowHandler, nullptr, surface);

    if (operationResult != VK_SUCCESS)
        throw runtime_error("failed to create window surface!");
}

static void eventClose(GLFWwindow *windowHandler)
{
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(windowHandler));

    WindowInputDeviceHandler **handlers = window->getHandlers();

    for (size_t i = 0; i != window->getHandlersCount(); i++)
        handlers[i]->window_OnClose();
}

static void eventResize(GLFWwindow *windowHandler, int width, int height)
{
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(windowHandler));

    WindowInputDeviceHandler **handlers = window->getHandlers();

    for (size_t i = 0; i != window->getHandlersCount(); i++)
        handlers[i]->window_OnResize(width, height);
}

void Window::setup(VkDemos::WindowInfo &windowInfo)
{
    glfwSetErrorCallback(VkDemos::VkLogger::error);

    if (!glfwInit())
        throw runtime_error("failed to init GLFW!");

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, windowInfo.isResizable());

    windowHandler = glfwCreateWindow(windowInfo.getWidth(), windowInfo.getHeight(), windowInfo.getWindowName().c_str(), NULL, NULL);
    if (!windowHandler)
    {
        glfwTerminate();
        throw runtime_error("failed to create GLFW window!");
    }

    glfwSetWindowUserPointer(windowHandler, this);

    glfwSetWindowCloseCallback(windowHandler, eventClose);
    glfwSetWindowSizeCallback(windowHandler, eventResize);
    //glfwSetFramebufferSizeCallback(windowHandler, eventResize);

    //glfwMakeContextCurrent(windowHandler);
}

Size Window::getSize()
{
    int width, height;
    glfwGetWindowSize(windowHandler, &width, &height);

    return Size{width, height};
}

void Window::update(long long elapsedTime)
{
    glfwPollEvents();
}

vector<const char *> Window::getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    return extensions;
}

void Window::printRequiredExtensions()
{
    auto requiredExtensions = getRequiredExtensions();

    VkLogger::info(requiredExtensions.size() + " extensions required to Window System:");
    for (const auto &requiredExtension : requiredExtensions)
        VkLogger::info(requiredExtension);
}

Window::~Window()
{
    if (windowHandler != nullptr)
    {
        glfwDestroyWindow(windowHandler);
        windowHandler = nullptr;
    }

    glfwTerminate();
}

} // namespace VkDemos