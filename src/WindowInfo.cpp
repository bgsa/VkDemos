#include "WindowInfo.h"

namespace VkBootstrap
{

WindowInfo::WindowInfo(std::string windowName, int width, int height)
{
    setWindowName(windowName);
    setWidth(width);
    setHeight(height);
}

std::string WindowInfo::getWindowName()
{
    return windowName;
}
void WindowInfo::setWindowName(std::string windowName)
{
    this->windowName = windowName;
}

int WindowInfo::getWidth()
{
    return width;
}
void WindowInfo::setWidth(int width)
{
    this->width = width;
}

int WindowInfo::getHeight()
{
    return height;
}
void WindowInfo::setHeight(int height)
{
    this->height = height;
}

bool WindowInfo::isResizable()
{
    return resizable;
}
void WindowInfo::setResizable(bool isResizable)
{
    this->resizable = isResizable;
}

}
