#ifndef RENDERER_OBJECT_HEADER
#define RENDERER_OBJECT_HEADER

#include "VkBootstrapHeader.h"
#include "MemoryBuffer.h"
#include "Shader.h"
#include "GraphicPipeline.h"

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
			{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} }, 
			{ { 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} }, 
			{ { 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} }, 
			{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} } 
		};
		const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };

		Device *device = nullptr;
		Shader *shader = nullptr;
		MemoryBuffer* dataBuffer = nullptr;
		GraphicPipeline *graphicPipeline = nullptr;
		Viewport *viewport = nullptr;
				
		void createPipeline(Device* device, SwapChain* swapChain, VkDescriptorSetLayout* descriptorSetLayout, const std::vector<VkDescriptorSet>& descriptorSets);
				
	public:
		RendererObject(Device* device, SwapChain* swapChain, Viewport* viewport, VkDescriptorSetLayout* descriptorSetLayout, const std::vector<VkDescriptorSet>& descriptorSets);

		void update(long long elapsedTime);
		void render(SwapChain* swapChain, uint32_t imageIndex);

		~RendererObject();
	};

}

#endif