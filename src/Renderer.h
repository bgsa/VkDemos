#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "VkBootstrapHeader.h"
#include <array>
#include "MemoryBuffer.h"
#include "OpenML.h"
#include "Shader.h"
#include "GraphicPipeline.h"
#include "CommandManager.h"
#include "Camera.h"
#include "RendererObject.h"

namespace VkBootstrap
{
	class Renderer
	{
	private:
		struct UniformBufferObject {
			OpenML::Mat4f modelMatrix;
			OpenML::Mat4f viewMatrix;
			OpenML::Mat4f projectionMatrix;
		};

		UniformBufferObject ubo = {};

		Device *device = nullptr;
		SwapChain* swapChain = nullptr;
		MemoryBuffer* uniformBuffer = nullptr;
		Viewport *viewport = nullptr;
		
		VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
		std::vector<VkDescriptorSet> descriptorSets;

		void createDescriptorPool(const Device* device);
		void createDescriptorSetLayout(const Device* device);
		void createDescriptorSets(const Device* device);

		Camera camera;
		RendererObject* rendererObject = nullptr;
						
	public:
		Renderer(Device* device, SwapChain* swapChain, Size windowSize);

		void setSwapChain(SwapChain* swapChain);

		void resize(int width, int height);
		void update(long long elapsedTime);
		void render(uint32_t imageIndex);

		~Renderer();
	};

}

#endif