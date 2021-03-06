#ifndef VK_DEMOS_HEADER_H
#define VK_DEMOS_HEADER_H

#ifdef WINDOWS
	#define VK_USE_PLATFORM_WIN32_KHR

	#ifndef NOMINMAX
		#define NOMINMAX
	#endif 

	#include <Windows.h>
#endif

#ifdef LINUX
	#define VK_USE_PLATFORM_XLIB_KHR
#endif

#ifdef ANDROID
	#define VK_USE_PLATFORM_ANDROID_KHR
#endif

#include <limits>
#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include <set>
#include <vector>
#include <vulkan/vulkan.h>

#include "VkLogger.hpp"
#include "VectorHelper.hpp"
#include "SetHelper.hpp"
#include "VkHelper.hpp"

#include <OpenML.h>

namespace VkBootstrap 
{
	class Renderer;
	class CommandManager;
	class Command;
	class CopyCommand;
}

#endif