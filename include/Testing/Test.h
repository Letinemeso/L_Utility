#pragma once


namespace LTest
{

    class Test
    {
    private:
        bool m_success = true;

    public:
        inline void reset() { m_success = true; }
        inline void mark_failed() { m_success = false; }

        inline bool success() const { return m_success; }

    public:
        virtual void run() = 0;

    };

}
