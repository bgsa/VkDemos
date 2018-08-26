#include "Window.h"

#include <glfw/glfw3native.h>

namespace VkDemos
{

void Window::createSurface(const VkInstance &vulkanInstance, VkSurfaceKHR *surface)
{
    if (glfwCreateWindowSurface(vulkanInstance, windowHandler, nullptr, surface) != VK_SUCCESS)
        throw runtime_error("failed to create window surface!");
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

    glfwSetWindowPos(windowHandler, 100, 100);

    glfwMakeContextCurrent(windowHandler);

    //while (!glfwWindowShouldClose(windowHandler))
    //    update();
}

void Window::update()
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