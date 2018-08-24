#include "Window.h"

namespace VkDemos
{

void Window::setup(VkDemos::WindowInfo &windowInfo)
{
    glfwSetErrorCallback(VkDemos::VkLogger::error);

    if (!glfwInit())
        throw runtime_error("failed to init GLFW!");

    windowHandler = glfwCreateWindow(windowInfo.getWidth(), windowInfo.getHeight(), windowInfo.getWindowName().c_str(), NULL, NULL);
    if (!windowHandler)
    {
        glfwTerminate();
        throw runtime_error("failed to create GLFW window!");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, windowInfo.isResizable());

    glfwSetWindowPos(windowHandler, 100, 100);

    glfwMakeContextCurrent(windowHandler);

    //while (!glfwWindowShouldClose(windowHandler))
    //    update();
}

void Window::update()
{
    glfwPollEvents();
}

vector<string> Window::getRequiredExtensions()
{
    uint32_t extensionCount;
    const char **requiredExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

    vector<string> extensions(extensionCount);

    for (size_t i = 0; i < extensionCount; i++)
        extensions[i] = string(requiredExtensions[i]);

    return extensions;
}

void Window::printRequiredExtensions()
{
    auto requiredExtensions = getRequiredExtensions();

    VkLogger::info(requiredExtensions.size() + " extensions required to Window System:");
    for (auto requiredExtension : requiredExtensions)
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