#include "Window.h"

namespace VkDemos
{

void Window::setup(VkDemos::WindowInfo &windowInfo)
{
    glfwSetErrorCallback(VkDemos::VkLogger::error);

    if (!glfwInit())
        return;

    windowHandler = glfwCreateWindow(windowInfo.getWidth(), windowInfo.getHeight(), windowInfo.getWindowName().c_str(), NULL, NULL);
    if (!windowHandler)
    {
        glfwTerminate();
        return;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwSetWindowPos(windowHandler, 100, 100);

    glfwMakeContextCurrent(windowHandler);

    /*
    while (!glfwWindowShouldClose(windowHandler))
    {
        glfwPollEvents();
    }
    */
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