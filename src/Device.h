#ifndef DEVICE_HEADER
#define DEVICE_HEADER

#include "VkBootstrapHeader.h"
#include "VkPhysicalDeviceManager.h"
#include "VkInstanceLayerConfiguration.h"
#include "QueueManager.h"

namespace VkBootstrap
{
	class Device
	{
	private:
		void createDevice(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);

	public:
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkDevice logicalDevice = VK_NULL_HANDLE;
		QueueManager* queueManager = nullptr;

		Device(const VkInstance &vulkanInstance, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);
		Device(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions);

		~Device();
	};
}

#endif