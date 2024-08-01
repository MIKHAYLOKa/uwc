#include "ProcessTimeCounter.h"

ProcessTimeCounter::ProcessTimeCounter()
	:mIsRunning(false)
{
}

ProcessTimeCounter::~ProcessTimeCounter()
{
}

bool ProcessTimeCounter::Start()
{
	bool result{ false };

	if (!mIsRunning)
	{
		mTime = high_resolution_clock().now();
		mIsRunning = true;

		result = true;
	}

	return result;
}

milliseconds ProcessTimeCounter::Stop()
{
	milliseconds result{ 0 };

	if (mIsRunning)
	{
		mIsRunning = false;
		result = duration_cast<milliseconds>(high_resolution_clock().now() - mTime);
	}

	return result;
}

bool ProcessTimeCounter::isRunning()
{
	return mIsRunning;
}
