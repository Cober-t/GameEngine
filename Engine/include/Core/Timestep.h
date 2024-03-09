#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <iostream>

#include <GLFW/glfw3.h>

namespace Cober {

	class Timestep {
		
	public:
		Timestep() 
		 : m_DeltaTime(0), m_NowTime(0), m_LastFrameTime(glfwGetTime()), m_Timer(0),
		  m_Frames(0), m_Updates(0), m_LimitFPS(1.0f / 120.0f) 
		{ 
			LOG_CORE_INFO("Created Timer");
		}

		~Timestep()
		{
			LOG_CORE_INFO("Destroyed Timer");
		}

		inline double GetDeltaTime() const { return m_DeltaTime; }

		inline void Start()
		{
			m_NowTime = glfwGetTime();
			m_DeltaTime += (m_NowTime - m_LastFrameTime) / m_LimitFPS;
			m_LastFrameTime = m_NowTime;

			m_Frames++;
		}

		inline double GetLimitFPS() const { return m_LimitFPS; }
		inline void SetLimitFPS(int limitFPS = 1) { m_LimitFPS = limitFPS; }

		inline void Update() 
		{
			m_Updates++; 
			m_DeltaTime--;
		}

		inline void ResetAfterOneSecond() 
		{ 
			if (glfwGetTime() - m_Timer > 1.0)
				m_Timer++, m_Updates = 0; m_Frames = 0;
		}

		operator float() const { return m_DeltaTime; }
		float GetSeconds() const { return m_DeltaTime; }
		float GetMilliseconds() const { return m_DeltaTime * 1000.0f; }

	private:
		double m_DeltaTime, m_NowTime, m_LastFrameTime, m_Timer;
		double m_LimitFPS;
		int m_Frames, m_Updates;
	};
}

#endif