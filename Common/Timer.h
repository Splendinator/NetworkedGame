#pragma once
#include <chrono>


class Timer {
public:
	Timer();
	~Timer();

	//Take away some time from the timer
	Timer &operator-=(float seconds);

	//Reset time to 0s
	void resetTimer();

	float getDelta();



private:
	std::chrono::system_clock::time_point _last;
};
