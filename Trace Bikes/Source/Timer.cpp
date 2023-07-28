#include "Timer.h"

namespace GameDev2D
{
	Timer::Timer(double duration, bool isrunning) :
		m_Elapsed(0),
		m_Duration(duration),
		m_IsRunning(isrunning) { }

	Timer::Timer(const Timer& timer) :
		m_Elapsed(timer.m_Elapsed),
		m_Duration(timer.m_Duration),
		m_IsRunning(timer.m_IsRunning) { }

	void Timer::Update(double delta)
	{
		if (m_IsRunning == true)
		{
			m_Elapsed += delta;

			if (m_Elapsed >= m_Duration)
				m_Elapsed = m_Duration;
		}
	}

	void Timer::Start()
	{
		m_IsRunning = true;
	}

	void Timer::Stop()
	{
		m_IsRunning = false;
	}

	void Timer::Reset()
	{
		m_Elapsed = 0.0;
		m_IsRunning = false;
	}

	void Timer::Restart()
	{
		m_Elapsed = 0.0;
		m_IsRunning = true;
	}

	void Timer::SetDuration(double duration)
	{
		m_Duration = duration;
	}

	float Timer::GetPercentage()
	{
		return (float)(m_Elapsed / m_Duration);
	}

	bool Timer::IsRunning()
	{
		return m_IsRunning;
	}
}