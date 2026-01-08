#include <Stuff/Timer.h>

using namespace LST;


void Timer::start(float _alarm_time)
{
    m_alarm_time = _alarm_time;
    m_current_time = 0.0f;

    if(m_alarm_time < 1e-6f)
        return;

    m_active = true;
}

void Timer::reset()
{
    m_current_time = 0.0f;
    m_active = false;
    m_just_stopped = false;
}


void Timer::update(float _time_passed)
{
    m_just_stopped = false;

    if(!m_active)
        return;

    m_current_time += _time_passed;
    if(m_current_time < m_alarm_time)
        return;

    m_active = false;
    m_current_time = m_alarm_time;
    m_just_stopped = true;
}
