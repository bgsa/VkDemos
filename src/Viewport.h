#ifndef VIEWPORT_HEADER
#define VIEWPORT_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class Viewport
{
  private:
    int width = 0;
    int height = 0;

  public:
    Viewport(int width, int height);

    VkPipelineViewportStateCreateInfo *getViewportState();
};
} // namespace VkDemos

#endif