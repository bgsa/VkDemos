#include "Window.h"
#include <iostream>

namespace VkDemos
{

static void error_callback(int error, const char *description)
{
    fputs(description, stderr);
}

void Window::init()
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        return;

    windowHandler = glfwCreateWindow(800, 600, "WINDOW_NAME", NULL, NULL);
    if (!windowHandler)
    {
        glfwTerminate();
        return;
    }

    glfwSetWindowPos(windowHandler, 100, 100);

    glfwMakeContextCurrent(windowHandler);

    glfwTerminate();
}

Window::~Window()
{
    if (windowHandler != NULL)
        glfwDestroyWindow(windowHandler);

    windowHandler = NULL;
}

} // namespace VkDemos