#include "Application.h"

#include "MemoryBuffer.h"
#include "OpenML.h"
#include <array>

struct Vertex {
	OpenML::Vec2f pos;
	OpenML::Vec3f color;
};

const std::vector<Vertex> vertices = {
	{ {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{ {0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}},
	{ {-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};


namespace VkBootstrap
{
	const int MAX_FRAMEBUFFER = 2;

	void Application::run()
	{
		setupWindow();
		setupVulkan();
		setupSurface();
		setupDevice();

		start();
	}

	void Application::stop()
	{
		isRunning = false;
		vkDeviceWaitIdle(device->logicalDevice);
	}

	void Application::start()
	{
		setupSwapChain();

		size_t currentFrame = 0;
		uint32_t imageIndex = 0;
		uint32_t fenceCount = 1;
		VkBool32 waitAllSemaphores = VK_TRUE;
		uint64_t semaphoresTimeout = std::numeric_limits<uint64_t>::max();

		isRunning = true;

		//TODO: remover
		MemoryBuffer* memoryBuffer = new MemoryBuffer(device, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, sizeof(vertices[0]) * vertices.size(), (void*)vertices.data());

		VkVertexInputBindingDescription bindingDescription = {};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
		

		while (isRunning)
		{
			window->update(0);

			vkWaitForFences(device->logicalDevice, 1, &inFlightFences[currentFrame], waitAllSemaphores, semaphoresTimeout);
			vkResetFences(device->logicalDevice, 1, &inFlightFences[currentFrame]);

			VkSwapchainKHR swapChains[] = { swapChain->vulkanSwapChain };
			VkSemaphore imageAvailableSemaphores[] = { imageAvailableSemaphore[currentFrame] };
			VkSemaphore renderFinishedSemaphores[] = { renderFinishedSemaphore[currentFrame] };
			VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

			VkResult operationResult = vkAcquireNextImageKHR(device->logicalDevice, swapChain->vulkanSwapChain, semaphoresTimeout, imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to acquire image: " + VkHelper::getVkResultDescription(operationResult));
			

			graphicPipeline = new GraphicPipeline(device, shader, swapChain, viewport, &vertexInputInfo);


			Command *command = commandManager->createCommand(graphicPipeline, swapChain);
			command->begin(imageIndex);

			for (VkCommandBuffer commandBuffer : command->commandBuffers) 
			{
				VkBuffer vertexBuffers[] = { memoryBuffer->getBuffer() };
				VkDeviceSize offsets[] = { 0 };
				vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

				vkCmdDraw(commandBuffer, (uint32_t) vertices.size(), 1, 0, 0);
			}

			command->end();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.pWaitDstStageMask = waitStages;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = imageAvailableSemaphores;
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = renderFinishedSemaphores;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &command->commandBuffers[0];

			operationResult = vkQueueSubmit(device->queueManager->getGraphicQueueFamily()->getQueues()[0]->queue, 1, &submitInfo, inFlightFences[currentFrame]);

			if (operationResult != VK_SUCCESS)
				throw std::runtime_error("failed to submit draw command buffer: " + VkHelper::getVkResultDescription(operationResult));

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
					cleanUpSwapChain();
					setupSwapChain();
					continue;
				}

				throw std::runtime_error("failed to present command buffer: " + VkHelper::getVkResultDescription(operationResult));
			}

			currentFrame = (currentFrame + 1) % MAX_FRAMEBUFFER;

			vkQueueWaitIdle(device->queueManager->getPresentationQueueFamily()->getQueues()[0]->queue);

			commandManager->releaseCommands();
			delete graphicPipeline;
			graphicPipeline = nullptr;
		}

		vkDeviceWaitIdle(device->logicalDevice);

		cleanUpSwapChain();

		//TODO: remover
		delete memoryBuffer;
	}

	void Application::setupSwapChain()
	{
		swapChain = SwapChain::createSwapChain(device, surface, window);
		setupSyncObjects();

		shader = Shader::createShader(device, "resources/shaders/vert.spv", "resources/shaders/frag.spv");

		CommandManager::init(device);
		commandManager = CommandManager::getInstance();
	}

	void Application::setupDevice()
	{
		std::vector<const char *> requiredExtensions = VkPhysicalDeviceManager::getRequiredExtensionsForGraphic();
		device = new Device(vulkanInstance, surface, requiredExtensions);
	}

	void Application::setupSurface()
	{
		window->createSurface(vulkanInstance, &surface);
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

	void Application::setupWindow()
	{
		int width = 800;
		int height = 600;

		VkBootstrap::WindowInfo windowInfo("VkDemo", width, height);
		windowInfo.setResizable(true);

		window = new VkBootstrap::Window;
		window->setup(windowInfo);
		window->addHandler(this);

		viewport = new Viewport(width, height);
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
		viewport->setSize(width, height);
	}

	void Application::cleanUpSwapChain()
	{
		vkDeviceWaitIdle(device->logicalDevice);

		if (commandManager != nullptr)
		{
			delete commandManager;
			commandManager = nullptr;
		}

		if (graphicPipeline != nullptr)
		{
			delete graphicPipeline;
			graphicPipeline = nullptr;
		}

		if (shader != nullptr)
		{
			delete shader;
			shader = nullptr;
		}

		if (swapChain != nullptr)
		{
			delete swapChain;
			swapChain = nullptr;
		}

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

	void Application::exit()
	{
		cleanUpSwapChain();

		if (viewport != nullptr)
		{
			delete viewport;
			viewport = nullptr;
		}

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