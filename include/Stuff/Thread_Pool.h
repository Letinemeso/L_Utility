#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>

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
        Thread_Pool();
        ~Thread_Pool();

    private:
        void M_thread_cycle();

    public:
        void add_task(const Task& _task);
        void wait_for_completion();
    };

}
