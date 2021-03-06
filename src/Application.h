#ifndef APPLICATION_HEADER
#define APPLICATION_HEADER

#include "VkBootstrapHeader.h"
#include "Window.h"
#include "VkExtensionsConfiguration.h"
#include "VkInstanceLayerConfiguration.h"
#include "VkPhysicalDeviceManager.h"
#include "Device.h"
#include "SwapChain.h"
#include "Viewport.h"
#include "Rasterizer.hpp"
#include "WindowInputDevice.h"
#include "GraphicPipeline.h"
#include "CommandManager.h"
#include "Renderer.h"
#include "Timer.h"

#include <thread>
#include <chrono>

namespace VkBootstrap
{

	class Application : public WindowInputDeviceHandler
	{

	private:
		Window *window = nullptr;
		VkInstance vulkanInstance = VK_NULL_HANDLE;
		Device *device = nullptr;
		VkSurfaceKHR surface = VK_NULL_HANDLE;
		SwapChain *swapChain = nullptr;		
		CommandManager *commandManager = nullptr;		
		Renderer* renderer = nullptr;
		Timer timer;

		std::vector<VkSemaphore> imageAvailableSemaphore;
		std::vector<VkSemaphore> renderFinishedSemaphore;
		std::vector<VkFence> inFlightFences;

		const uint64_t semaphoresTimeout = std::numeric_limits<uint64_t>::max();

		bool isRunning = true;

		void setupWindow();
		void setupVulkan();
		void setupSurface();
		void setupDevice();
		void setupSyncObjects();
		void setupRenderer();
		void setupDebugCallback();
		void cleanUpRenderer();
		void cleanUpSyncObjects();

		uint32_t getImageIndex(size_t currentFrame);
		void swapBuffer(uint32_t imageIndex, size_t currentFrame);

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