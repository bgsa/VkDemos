#include "Device.h"

namespace VkBootstrap
{

	Device::Device(const VkInstance &vulkanInstance, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
	{
		VkPhysicalDeviceManager deviceManager(vulkanInstance);

		physicalDevice = deviceManager.findSuitableGraphicalDevice(extensions);

		createDevice(physicalDevice, surface, extensions);
	}

	Device::Device(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
	{
		createDevice(physicalDevice, surface, extensions);
	}

	void Device::createDevice(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const std::vector<const char *> &extensions)
	{
		this->physicalDevice = physicalDevice;

		std::vector<VkQueueFamilyProperties> queueFamilies = QueueManager::getQueueFamilies(physicalDevice);

		std::vector<VkDeviceQueueCreateInfo> queueInfos;
		for (uint32_t i = 0; i < queueFamilies.size(); i++)
		{
			float *queuePriority = new float[queueFamilies[i].queueCount];
			for (size_t index = 0; index < queueFamilies[i].queueCount; index++)
				queuePriority[index] = 1.0f;

			VkDeviceQueueCreateInfo queueCreateInfo = {};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = i;
			queueCreateInfo.queueCount = queueFamilies[i].queueCount;
			queueCreateInfo.pQueuePriorities = queuePriority;
			queueInfos.push_back(queueCreateInfo);
		}

		VkPhysicalDeviceFeatures deviceFeaturesRequired = {};

		VkDeviceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		createInfo.queueCreateInfoCount = 1;
		createInfo.pEnabledFeatures = &deviceFeaturesRequired;
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
		createInfo.pQueueCreateInfos = queueInfos.data();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = 0;

#if DEBUG
		const std::vector<const char *> validationLayers = { "VK_LAYER_LUNARG_standard_validation" };

		if (VkInstanceLayerConfiguration::isInstanceLayerSupported(validationLayers[0]))
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
#endif

		if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
			throw std::runtime_error("failed to create logical device!");

		queueManager = new QueueManager(physicalDevice, logicalDevice, surface);
	}


	Device::~Device()
	{
		if (queueManager != nullptr)
		{
			delete queueManager;
			queueManager = nullptr;
		}

		if (logicalDevice != VK_NULL_HANDLE)
		{
			vkDestroyDevice(logicalDevice, nullptr);
			logicalDevice = VK_NULL_HANDLE;
		}
	}

}
