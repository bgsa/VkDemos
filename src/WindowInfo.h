#ifndef WINDOW_INFO_HEADER
#define WINDOW_INFO_HEADER

#include "VkBootstrapHeader.h"

namespace VkBootstrap
{
class WindowInfo
{
private:
	std::string windowName;
  int width;
  int height;
  bool resizable = false;

public:
  WindowInfo(std::string windowName, int width, int height);

  std::string getWindowName();
  void setWindowName(std::string windowName);

  int getWidth();
  void setWidth(int width);

  int getHeight();
  void setHeight(int height);

  bool isResizable();
  void setResizable(bool isResizable);
};
}

#endif