#ifndef RENDERER_OBJECT_HEADER
#define RENDERER_OBJECT_HEADER

#include "VkBootstrapHeader.h"
#include <array>
#include "MemoryBuffer.h"
#include "OpenML.h"
#include "Shader.h"
#include "GraphicPipeline.h"
#include "CommandManager.h"
#include "Camera.h"

namespace VkBootstrap
{

	struct Vertex {
		OpenML::Vec2f pos;
		OpenML::Vec3f color;
	};

	struct UniformBufferObject {
		OpenML::Mat4f modelMatrix;
		OpenML::Mat4f viewMatrix;
		OpenML::Mat4f projectionMatrix;
	};

	class RendererObject
	{
	private:
		UniformBufferObject ubo = {};

		const std::vector<Vertex> vertices = { 
			{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f} }, 
			{ { 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f} }, 
			{ { 0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} }, 
			{ {-0.5f,  0.5f}, {1.0f, 1.0f, 1.0f} } 
		};
		const std::vector<uint16_t> indices = { 0, 1, 2, 2, 3, 0 };


		Device *device= nullptr;
		Shader *shader = nullptr;
		MemoryBuffer* dataBuffer = nullptr;
		MemoryBuffer* uniformBuffer = nullptr;
		GraphicPipeline *graphicPipeline = nullptr;
		Viewport *viewport = nullptr;

		Camera camera;

		void createPipeline(Device* device, SwapChain* swapChain, Viewport* viewport);
				
	public:
		RendererObject(Device* device, SwapChain* swapChain, Viewport* viewport);

		void update(long long elapsedTime);
		void render(CommandManager* commandManager, SwapChain* swapChain, uint32_t imageIndex, Viewport* viewport);

		~RendererObject();
	};

}

#endif