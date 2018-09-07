#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "VkBootstrapHeader.h"
#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkInstanceLayerConfiguration.h"
#include "VkPhysicalDeviceManager.h"
#include "Device.h"
#include "SwapChain.h"
#include "Shader.h"
#include "Viewport.h"
#include "Rasterizer.hpp"
#include "WindowInputDevice.h"
#include "GraphicPipeline.h"
#include "CommandManager.h"

namespace VkBootstrap
{

	class Application : public WindowInputDeviceHandler
	{

	private:
		VkBootstrap::Window *window = nullptr;
		VkInstance vulkanInstance = VK_NULL_HANDLE;
		Device *device = nullptr;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		Viewport *viewport = nullptr;
		SwapChain *swapChain = nullptr;
		GraphicPipeline *graphicPipeline = nullptr;
		CommandManager *commandManager = nullptr;
		Shader *shader = nullptr;

		std::vector<VkSemaphore> imageAvailableSemaphore;
		std::vector<VkSemaphore> renderFinishedSemaphore;
		std::vector<VkFence> inFlightFences;

		bool isRunning = true;

		void setupWindow();
		void setupVulkan();
		void setupSurface();
		void setupDevice();
		void setupSwapChain();
		void setupSyncObjects();
		void setupDebugCallback();
		void cleanUpSwapChain();

		void window_OnClose();
		void window_OnResize(int width, int height);

	public:
		void run();
		void exit();
		void start();
		void stop();
		

		~Application();
	};

}

#endif