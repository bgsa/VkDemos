#ifndef WINDOW_HEADER
#define WINDOW_HEADER

#include <glfw/glfw3.h>

namespace VkDemos
{

class Window
{
  private:
    GLFWwindow *windowHandler;

  public:
    void init();

    ~Window();
};

} // namespace VkDemos

#endif