#include <Stuff/Stopwatch.h>

using namespace LST;


void Stopwatch::start()
{
	m_active = true;
	m_duration = 0.0f;
	m_time_point = std::chrono::high_resolution_clock::now();
}

void Stopwatch::stop()
{
	m_active = false;
	m_duration = (std::chrono::high_resolution_clock::now() - m_time_point).count();
}



float Stopwatch::duration() const
{
	if(m_active == true)
		return (std::chrono::high_resolution_clock::now() - m_time_point).count() / 1000000000.0f;
	else
		return m_duration / 1000000000.0f;
}
