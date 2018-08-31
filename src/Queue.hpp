#ifndef QUEUE_HEADER
#define QUEUE_HEADER

#include "VkDemosHeader.h"

namespace VkDemos
{
class Queue
{
public:
  uint32_t index = -1;
  VkQueue queue = VK_NULL_HANDLE;

  Queue()
  {
    ;
  }

  Queue(uint32_t index, VkQueue queue)
  {
    this->index = index;
    this->queue = queue;
  }
};
} // namespace VkDemos

#endif