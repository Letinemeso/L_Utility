#include <Stuff/Smooth_Rotator.h>

using namespace LST;


void Smooth_Rotator::update(float _dt)
{
    if(!active())
        return;

    m_timer.update(_dt);

    m_current_value = glm::slerp(m_start_rotation, m_end_rotation, m_timer.current_time_ratio());

    if(m_on_update)
        m_on_update(m_current_value);
}
