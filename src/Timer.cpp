#include "Timer.h"

namespace VkBootstrap
{
	int timeCounter = 0;
	int fpsCounter = 0;

	void Timer::calculateFramesPerSecond()
	{
		bool oneSecondPassed = timeCounter > 1000;

		if (oneSecondPassed)
		{
			framesPerSecond = fpsCounter;
			timeCounter = 0;
			fpsCounter = 0;
		}

		timeCounter += (int)getElapsedTime();
		fpsCounter++;
	}

	void Timer::start()
	{
		timeLastFrame = Clock::now();
	}

	void Timer::update()
	{
		Clock::time_point currentTime = Clock::now();

		deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - timeLastFrame);

		timeLastFrame = currentTime;

		calculateFramesPerSecond();
	}

	inline long long Timer::getElapsedTime()
	{
		return deltaTime.count();
	}

	inline int Timer::getFramesPerSecond()
	{
		return framesPerSecond;
	}
}