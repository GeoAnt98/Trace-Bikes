#pragma once

namespace GameDev2D
{
	class Timer
	{
	public:
		Timer(double duration, bool isrunning = false);
		Timer(const Timer& timer);

		void Update(double delta);

		void Start();
		void Stop();
		void Reset();
		void Restart();

		void SetDuration(double duration);

		float GetPercentage();

		bool IsRunning();

	private:
		double m_Duration;
		double m_Elapsed;
		bool m_IsRunning;
	};
}