#pragma once

#include <thread>
#include <atomic>

#include <Stuff/Function_Wrapper.h>


namespace LST
{

    class Thread
    {
    private:
        std::thread* m_thread = nullptr;
        std::atomic<bool> m_completed = true;

    public:
        Thread();
        ~Thread();

        Thread(const Thread&) = delete;
        Thread(Thread&&) = delete;
        void operator=(const Thread&) = delete;
        void operator=(Thread&&) = delete;

    public:
        inline bool completed() const { return m_completed; }

    public:
        void start(const LST::Function<void()>& _task, const LST::Function<void()>& _callback_on_completion = {});
        void detach();
        void wait_for_completion();

    };

}
