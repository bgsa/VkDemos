#ifndef RENDERER_OBJECT_HEADER
#define RENDERER_OBJECT_HEADER

#include "VkBootstrapHeader.h"
#include <array>
#include "MemoryBuffer.h"
#include "OpenML.h"

namespace VkBootstrap
{

	struct Vertex {
		OpenML::Vec2f pos;
		OpenML::Vec3f color;
	};

	class RendererObject
	{
	private:
		const std::vector<Vertex> vertices = {
			{ {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{ {0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
			{ {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		MemoryBuffer* memoryBuffer = nullptr;
		VkVertexInputBindingDescription bindingDescription = {};
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		
	public:
		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {  };
		 
		RendererObject(Device* device);

		void render(const VkCommandBuffer& commandBuffer);

		~RendererObject();
	};

}

#endif