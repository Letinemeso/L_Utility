#include <Stuff/Thread_Pool.h>

using namespace LST;


Thread_Pool::Thread_Pool(unsigned int _amount)
{
    if(_amount == 0)
        _amount = std::thread::hardware_concurrency();

    m_threads.resize(_amount);
    for(unsigned int i=0; i<m_threads.capacity(); ++i)
        m_threads.push(new std::thread([this](){ M_thread_cycle(); }));
}

Thread_Pool::~Thread_Pool()
{
    m_mutex.lock();
    m_should_stop = true;
    m_mutex.unlock();

    m_wait_condition.notify_all();

    for(unsigned int i=0; i<m_threads.capacity(); ++i)
    {
        std::thread* thread = m_threads[i];
        thread->join();
        delete thread;
    }
}



void Thread_Pool::M_thread_cycle()
{
    while(true)
    {
        Task current_task;

        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_wait_condition.wait(lock, [this](){ return m_should_stop || m_tasks.size() > 0; });

            if(m_should_stop)
                return;

            current_task = *m_tasks.begin();
            m_tasks.pop_front();

            ++m_active_threads;
        }

        current_task();

        m_mutex.lock();
        --m_active_threads;
        if(m_active_threads == 0)
            m_wait_for_completion_condition.notify_all();
        m_mutex.unlock();
    }
}



void Thread_Pool::add_task(const Task& _task)
{
    m_mutex.lock();
    m_tasks.push_back(_task);
    m_mutex.unlock();

    m_wait_condition.notify_one();
}

void Thread_Pool::wait_for_completion()
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_wait_for_completion_condition.wait(lock, [this]()
    {
        return m_tasks.size() == 0 && m_active_threads == 0;
    });
}
