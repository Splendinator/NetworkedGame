#include "Timer.h"
#include <chrono>

using namespace std::chrono;

Timer::Timer() {
	resetTimer();
}

Timer::~Timer()
{
}

Timer & Timer::operator-=(float seconds)
{
	_last += std::chrono::microseconds(int(seconds * std::micro::den));
	return *this;
}

void Timer::resetTimer()
{
	_last = system_clock::now();
}

float Timer::getDelta() {

	std::chrono::duration dif = system_clock::now() - _last;
	return dif.count() / float(system_clock::period::den);
}



