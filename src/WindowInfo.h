#ifndef WINDOW_INFO_HEADER
#define WINDOW_INFO_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class WindowInfo
{
private:
  string windowName;
  int width;
  int height;
  bool isResizable = true;

public:
  WindowInfo(string windowName, int width, int height);

  string getWindowName();
  void setWindowName(string windowName);

  int getWidth();
  void setWidth(int width);

  int getHeight();
  void setHeight(int height);

  bool isResizable();
  void setResizable(bool isResizable);
};
} // namespace VkDemos

#endif