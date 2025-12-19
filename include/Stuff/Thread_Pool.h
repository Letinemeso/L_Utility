#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>

#include <Data_Structures/Vector.h>
#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    class Thread_Pool
    {
    public:
        using Task = LST::Function<void()>;

    private:
        using Thread_Vector = LDS::Vector<std::thread*>;
        using Tasks_List = LDS::List<Task>;

    private:
        Thread_Vector m_threads;
        std::condition_variable m_wait_condition;
        std::mutex m_mutex;
        bool m_should_stop = false;

        unsigned int m_active_threads = 0;

        Tasks_List m_tasks;

        std::condition_variable m_wait_for_completion_condition;

    public:
        Thread_Pool(unsigned int _amount = 0);
        ~Thread_Pool();

        Thread_Pool(const Thread_Pool&) = delete;
        Thread_Pool(Thread_Pool&&) = delete;

    public:
        inline unsigned int threads_amount() const { return m_threads.size(); }

    private:
        void M_thread_cycle();

    public:
        void add_task(const Task& _task);
        void wait_for_completion();
    };

}
