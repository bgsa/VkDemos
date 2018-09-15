#ifndef VIEWPORT_HEADER
#define VIEWPORT_HEADER

#include "VkBootstrapHeader.h"
#include "Size.hpp"

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

		int getWidth();
		int getHeight();

		Size getSize();
		void setSize(int width, int height);

		float getAspectRatio();

		VkViewport* getVkViewport();
		VkRect2D* getVkScissors();
		VkPipelineViewportStateCreateInfo *getViewportState();
	};
}

#endif