#pragma once

namespace LST
{

    class Timer
    {
    private:
        float m_alarm_time = 0.0f, m_current_time = 0.0f;
        bool m_active = false;

    public:
        Timer() { }
        Timer(float _alarm_time) { start(_alarm_time); }
        ~Timer() { }

    public:
        void start(float _alarm_time);
        void reset();

        void update(float _time_passed);

        inline bool is_active() const { return m_active; }
        inline float current_time() const { return m_current_time; }
        inline float time_left() const { return m_alarm_time - m_current_time; }
        inline float current_time_ratio() const { return m_current_time / m_alarm_time; }
        inline float time_left_ratio() const { return 1.0f - current_time_ratio(); }

    };

}
