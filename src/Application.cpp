#include "Application.h"

#include "RendererObject.h"

namespace VkBootstrap
{
	const int MAX_FRAMEBUFFER = 2;

	void Application::run()
	{
		setupWindow();
		setupVulkan();
		setupSurface();
		setupDevice();
		setupRenderer();

		start();
	}

	void Application::stop()
	{
		isRunning = false;
		vkDeviceWaitIdle(device->logicalDevice);
	}

	void Application::start()
	{
		timer.start();
		
		size_t currentFrame = 0;
		uint32_t imageIndex = 0;
		uint32_t fenceCount = 1;
		VkBool32 waitAllSemaphores = VK_TRUE;

		isRunning = true;
				
		while (isRunning)
		{
			long long elapsedTime = timer.getElapsedTime();

			window->update(elapsedTime);
			renderer->update(elapsedTime);

			if (!window->isVisible()) 
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(250));
				continue;
			}

			vkWaitForFences(device->logicalDevice, 1, &inFlightFences[currentFrame], waitAllSemaphores, semaphoresTimeout);
			vkResetFences(device->logicalDevice, 1, &inFlightFences[currentFrame]);

			imageIndex = getImageIndex(currentFrame);

			renderer->render(imageIndex);

			swapBuffer(imageIndex, currentFrame);

			commandManager->releaseCommands();

			timer.update();

			currentFrame = (currentFrame + 1) % MAX_FRAMEBUFFER;
		}

		vkDeviceWaitIdle(device->logicalDevice);

		cleanUpRenderer();
	}

	uint32_t Application::getImageIndex(size_t currentFrame) 
	{
		uint32_t imageIndex;

		VkResult operationResult = vkAcquireNextImageKHR(device->logicalDevice, swapChain->vulkanSwapChain, semaphoresTimeout, imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to acquire image: " + VkHelper::getVkResultDescription(operationResult));

		return imageIndex;
	}

	void Application::swapBuffer(uint32_t imageIndex, size_t currentFrame) 
	{
		VkSwapchainKHR swapChains[] = { swapChain->vulkanSwapChain };
		VkSemaphore renderFinishedSemaphores[] = { renderFinishedSemaphore[currentFrame] };
		VkSemaphore imageAvailableSemaphores[] = { imageAvailableSemaphore[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkCommandBuffer* commandBuffers = commandManager->getCommandBuffers();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = imageAvailableSemaphores;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = renderFinishedSemaphores;
		submitInfo.commandBufferCount = (uint32_t)commandManager->getCommandsCount();
		submitInfo.pCommandBuffers = commandBuffers;

		VkResult operationResult = vkQueueSubmit(device->queueManager->getGraphicQueueFamily()->getQueues()[0]->queue, 1, &submitInfo, inFlightFences[currentFrame]);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to submit draw command buffer: " + VkHelper::getVkResultDescription(operationResult));

		delete commandBuffers;

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = renderFinishedSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		operationResult = vkQueuePresentKHR(device->queueManager->getPresentationQueueFamily()->getQueues()[0]->queue, &presentInfo);

		if (operationResult != VK_SUCCESS)
		{
			if (operationResult == VK_ERROR_OUT_OF_DATE_KHR)
			{
				cleanUpRenderer();
				setupRenderer();
			}
			else
				throw std::runtime_error("failed to present command buffer: " + VkHelper::getVkResultDescription(operationResult));
		}

		vkQueueWaitIdle(device->queueManager->getPresentationQueueFamily()->getQueues()[0]->queue);
	}

	void Application::setupWindow()
	{
		int width = 800;
		int height = 600;

		VkBootstrap::WindowInfo windowInfo("VkDemo", width, height);
		windowInfo.setResizable(true);

		window = new VkBootstrap::Window;
		window->setup(windowInfo);
		window->addHandler(this);
	}

	void Application::setupVulkan()
	{
		std::vector<const char *> extensionsRequired = window->getRequiredExtensions();

#if DEBUG
		VkExtensionsConfiguration::printSupportedExtensions();
		window->printRequiredExtensions();
		VkInstanceLayerConfiguration::printInstanceLayersSupported();

		extensionsRequired.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsRequired.size());
		createInfo.ppEnabledExtensionNames = extensionsRequired.data();
		createInfo.enabledLayerCount = 0;

#if DEBUG
		const std::vector<const char *> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

		if (VkInstanceLayerConfiguration::isInstanceLayerSupported(validationLayers[0]))
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
#endif

		VkResult operationResult = vkCreateInstance(&createInfo, nullptr, &vulkanInstance);

		if (operationResult != VK_SUCCESS)
			throw std::runtime_error("failed to create vulkan instance!");

#if DEBUG
		setupDebugCallback();
#endif
	}

	void Application::setupSurface()
	{
		window->createSurface(vulkanInstance, &surface);
	}

	void Application::setupDevice()
	{
		std::vector<const char *> requiredExtensions = VkPhysicalDeviceManager::getRequiredExtensionsForGraphic();
		device = new Device(vulkanInstance, surface, requiredExtensions);
	}

	void Application::setupSyncObjects()
	{
		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		imageAvailableSemaphore.resize(MAX_FRAMEBUFFER);
		renderFinishedSemaphore.resize(MAX_FRAMEBUFFER);
		inFlightFences.resize(MAX_FRAMEBUFFER);

		VkResult operationResult;

		for (size_t i = 0; i != MAX_FRAMEBUFFER; i++)
		{
			operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore[i]);
			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to create imageAvailableSemaphores!");

			operationResult = vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore[i]);
			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to create renderFinishedSemaphore!");

			operationResult = vkCreateFence(device->logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]);
			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to create fence in flight!");
		}
	}

	void Application::cleanUpSyncObjects() 
	{
		for (VkSemaphore semaphore : renderFinishedSemaphore)
			vkDestroySemaphore(device->logicalDevice, semaphore, nullptr);
		renderFinishedSemaphore.clear();

		for (VkSemaphore semaphore : imageAvailableSemaphore)
			vkDestroySemaphore(device->logicalDevice, semaphore, nullptr);
		imageAvailableSemaphore.clear();

		for (VkFence fence : inFlightFences)
			vkDestroyFence(device->logicalDevice, fence, nullptr);
		inFlightFences.clear();
	}

	void Application::setupRenderer()
	{
		swapChain = SwapChain::createSwapChain(device, surface, window);
		setupSyncObjects();

		CommandManager::init(device);
		commandManager = CommandManager::getInstance();

		Size windowSize = window->getSize();

		if (renderer == nullptr)
		{
			Renderer::init(device, swapChain, windowSize);
			renderer = Renderer::getInstance();
		}
		else
			renderer->setSwapChain(swapChain);
	}

	void Application::cleanUpRenderer()
	{
		vkDeviceWaitIdle(device->logicalDevice);

		if (commandManager != nullptr)
		{
			delete commandManager;
			commandManager = nullptr;
		}

		if (swapChain != nullptr)
		{
			delete swapChain;
			swapChain = nullptr;
		}

		cleanUpSyncObjects();
	}

	void Application::setupDebugCallback()
	{
		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

		createInfo.messageSeverity =
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

		createInfo.messageType =
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

		createInfo.pfnUserCallback = VkLogger::vkDebugCallback;
		createInfo.pUserData = nullptr;

		if (VkInstanceLayerConfiguration::createDebugUtilsMessengerEXT(vulkanInstance, &createInfo, nullptr) != VK_SUCCESS)
			throw std::runtime_error("failed to set up debug callback!");
	}

	void Application::window_OnClose()
	{
		isRunning = false;
	}

	void Application::window_OnResize(int width, int height)
	{
		renderer->resize(width, height);
	}

	void Application::exit()
	{
		if (renderer != nullptr)
		{
			delete renderer;
			renderer = nullptr;
		}

		cleanUpRenderer();

		if (device != nullptr)
		{
			delete device;
			device = nullptr;
		}

		if (surface != VK_NULL_HANDLE)
		{
			vkDestroySurfaceKHR(vulkanInstance, surface, nullptr);
		}

#if DEBUG
		VkInstanceLayerConfiguration::destroyDebugUtilsMessengerEXT(vulkanInstance, nullptr);
#endif

		if (vulkanInstance != nullptr)
		{
			vkDestroyInstance(vulkanInstance, nullptr);
			vulkanInstance = nullptr;
		}

		if (window != nullptr)
		{
			delete window;
			window = nullptr;
		}
	}

	Application::~Application()
	{
		exit();
	}

}