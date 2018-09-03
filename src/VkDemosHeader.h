#ifndef VK_DEMOS_HEADER_H
#define VK_DEMOS_HEADER_H

#ifdef WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif 

	#include <Windows.h>
#endif

#include <limits>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <vulkan/vulkan.h>

#include "VkLogger.hpp"
#include "VectorHelper.hpp"
#include "SetHelper.hpp"
#include "VkHelper.hpp"

using namespace std;

#endif