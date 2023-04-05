#ifndef TIMER_H
#define TIMER_H

namespace LST
{

    class Timer
    {
    private:
        float m_alarm_time = 0.0f, m_current_time = 0.0f;
        bool m_active = false;

    public:
        Timer() { }
        ~Timer() { }

    public:
        void start(float _alarm_time);
        void reset();

        void update(float _time_passed);

        inline bool is_active() const { return m_active; }
        inline float time_left() const { return m_alarm_time - m_current_time; }

    };

}

#endif // TIMER_H
