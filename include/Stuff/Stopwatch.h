#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <chrono>


namespace LST
{

	class Stopwatch
	{
	private:
		std::chrono::high_resolution_clock::time_point m_time_point;
		float m_duration = 0.0f;

		bool m_active = false;

	public:
		void start();
		void stop();

	public:
		float duration() const;

	};

}


#endif // STOPWATCH_H
