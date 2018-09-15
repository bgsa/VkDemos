#ifndef TIMER_HEADER
#define TIMER_HEADER

#include <chrono>

namespace VkBootstrap
{
	typedef std::chrono::high_resolution_clock Clock;

	class Timer
	{
	private:
		Clock::time_point timeLastFrame;
		std::chrono::milliseconds deltaTime;
		int framesPerSecond;

		void calculateFramesPerSecond();

	public:
		void start();
		void update();

		long long getElapsedTime();
		int getFramesPerSecond();
	};
}

#endif