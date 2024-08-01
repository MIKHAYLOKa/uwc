#ifndef PROCESS_TIME_COUNTER
#define PROCESS_TIME_COUNTER

#include <chrono>

using namespace std::chrono;

class ProcessTimeCounter
{
private:
	time_point<steady_clock> mTime;
	bool mIsRunning;
public:

	ProcessTimeCounter();
	~ProcessTimeCounter();

	bool Start();
	milliseconds Stop();
	bool isRunning();

};




#endif // !PROCESS_TIME_COUNTER
