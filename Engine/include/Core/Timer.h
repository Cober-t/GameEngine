#ifndef TIMER_H
#define TIMER_H

#include <chrono>

namespace Cober {

	class Timer
	{
	public:
		Timer()
		{
			Reset();
		}

		inline void Timer::Reset()
		{
			m_Start = std::chrono::high_resolution_clock::now();
		}

		inline float Timer::Elapsed()
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - m_Start).count() * 0.001f * 0.001f * 0.001f;
		}

		inline float Timer::ElapsedMillis()
		{
			return Elapsed() * 1000.0f;
		}

	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
	};

	class Time
	{
	public:
		static float GetTime();
	};
}

#endif