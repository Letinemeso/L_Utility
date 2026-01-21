#pragma once

#include <gtc/quaternion.hpp>

#include <L_Debug/L_Debug.h>
#include <Stuff/Timer.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    class Smooth_Rotator
    {
    private:
        glm::quat m_start_rotation;
        glm::quat m_end_rotation;

        LST::Function<void(const glm::quat&)> m_on_update;

        LST::Timer m_timer;

    private:
        glm::quat m_current_value;

    public:
        inline void set_start_rotation(const glm::quat& _value) { m_start_rotation = _value; }
        inline void set_end_rotation(const glm::quat& _value) { m_end_rotation = _value; }

        inline void set_on_update_func(const LST::Function<void(const glm::quat&)>& _func) { m_on_update = _func; }

        inline void start(float _duration) { L_ASSERT(_duration > 0.0f); m_timer.start(_duration); }

    public:
        inline bool active() const { return m_timer.is_active(); }

        inline const glm::quat& current_value() const { return m_current_value; }

    public:
        void update(float _dt);

    };

}
