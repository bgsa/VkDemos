#ifndef VIEWPORT_HEADER
#define VIEWPORT_HEADER

#include "VkBootstrapHeader.h"

namespace VkBootstrap
{
class Viewport
{
  private:
    int width = 0;
    int height = 0;
	VkViewport* viewport = nullptr;
	VkRect2D *scissor = nullptr;

  public:
    Viewport(int width, int height);
	void setSize(int width, int height);
	
	VkViewport* getVkViewport();
	VkRect2D* getVkScissors();	
	VkPipelineViewportStateCreateInfo *getViewportState();
};
}

#endif