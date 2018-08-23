#include "WindowInfo.h"

namespace VkDemos
{

WindowInfo::WindowInfo(string windowName, int width, int height)
{
    setWindowName(windowName);
    setWidth(width);
    setHeight(height);
}

string WindowInfo::getWindowName()
{
    return windowName;
}
void WindowInfo::setWindowName(string windowName)
{
    this->windowName = windowName;
}

int WindowInfo::getWidth()
{
    return height;
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
    return isResizable;
}
void WindowInfo::setResizable(bool isResizable)
{
    this->isResizable = isResizable;
}

} // namespace VkDemos