#ifndef TIMESTEP_H
#define TIMESTEP_H

#include <Core/Log.h>
#include <iostream>

#include <GLFW/glfw3.h>

namespace Cober {

	class CB_API Timestep 
	{
	public:
		Timestep() 
		 : m_DeltaTime(0), m_NowTime(0), m_LastFrameTime(glfwGetTime()), m_Timer(0),
		  m_Frames(0), m_Updates(0), m_AccumulatedTime(0), m_LimitFPS(1.0f / 60.0f) 
		{ 
			LOG_CORE_INFO("Created Timer");
		}

		~Timestep()
		{
			LOG_CORE_INFO("Destroyed Timer");
		}

		inline double GetDeltaTime() const { return m_DeltaTime; }
		inline int GetUpdates() const { return m_UpdatesOneSec; }
		inline float GetDeltaTimePerSecond() const { return m_DeltaPerSecond; }

		inline void Start()
		{
			m_NowTime = glfwGetTime();
			m_DeltaTime = m_NowTime - m_LastFrameTime;
			m_LastFrameTime = m_NowTime;
			m_AccumulatedTime += m_DeltaTime;

			m_Frames++;
		}

		inline int GetFrames() const { return m_FramesOneSec; }
		inline double TimePassed() const { return m_NowTime; }
		inline float GetLimitFPS() const { return m_LimitFPS; }
		inline void SetLimitFPS(int limitFPS = 1) { m_LimitFPS = limitFPS; }

		inline void Update() 
		{
			m_Updates++; 
			m_DeltaTime--;
		}

		inline void ResetAfterOneSecond() 
		{ 
			if (glfwGetTime() - m_Timer > 1.0)
			{
				m_FramesOneSec = m_Frames;
				m_UpdatesOneSec = m_Updates;
				m_DeltaPerSecond = m_DeltaTime;
				m_Timer++, m_Updates = 0; m_Frames = 0;
			}
		}

		operator float() const { return m_DeltaTime; }
		float GetConsistentTimer() const { return m_DeltaTime; }
		float GetSecondsFromNow() const { return m_DeltaTime; }

		float GetTime() const { return m_Timer; }
		double& GetAccumulatedTime() { return m_AccumulatedTime; }

		float GetSeconds() const { return m_DeltaTime; }
		float GetMilliseconds() const { return m_DeltaTime * 1000.0f; }

	private:
		double m_DeltaTime, m_NowTime, m_LastFrameTime, m_Timer, m_AccumulatedTime;
		float m_LimitFPS;
		int m_Frames, m_Updates;
		float m_DeltaPerSecond = 0;
		int m_FramesOneSec = 0, m_UpdatesOneSec = 0;
	};
}

#endif;