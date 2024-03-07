#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <iostream>

#include <GLFW/glfw3.h>

namespace Cober {

	class Timestep {
		
	public:
		Timestep(float time = 0.0f)
			: m_FpsLimit(60), frames(0), m_Updates(0), m_AuxTime(0), m_LimitedFrames(false), deltaTime(0) 
		{
			m_LastFrameTime = glfwGetTime();
			m_Timer = m_LastFrameTime;
		}

		inline void Start(uint32_t framesLimit = 0) 
		{
			if (framesLimit == 0)
				framesLimit = m_FpsLimit;

			m_AuxTime = glfwGetTime();
			deltaTime += (m_AuxTime - m_LastFrameTime) / framesLimit;
			m_LastFrameTime = m_AuxTime;
		}

		inline void Reset(uint32_t framesLimit = 0) 
		{
			frames++;

			if (glfwGetTime() - m_Timer > 1.0) {

				LOG_CORE_TRACE("Frames: {0}", frames);
				m_Timer++;
				m_Updates = 0, frames = 0;
			}
		}

		operator float() const { return deltaTime; }

		inline void CountUpdate() { m_Updates++; }

		inline int GetFrames() { return frames; }
		inline void LockFrameRate(double frameRateLimit) { m_FpsLimit = frameRateLimit; }
		inline void UnlockFrameRate() { m_FpsLimit = 1; }

		inline double GetUpdates() { return m_Updates; }
		inline void IncreaseUpdate() { m_Updates++; }

		inline double DeltaTime() { return deltaTime; }
		inline void DecreaseDeltaTime() { deltaTime--; }

	private:
		int m_AuxTime, m_Updates;
		double m_Timer, m_FpsLimit, m_LastFrameTime;
		bool m_LimitedFrames;
		int frames;
		double deltaTime;
	};
}

#endif