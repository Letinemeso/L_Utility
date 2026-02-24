#include <Stuff/Thread.h>

#include <L_Debug/L_Debug.h>

using namespace LST;


Thread::Thread()
{

}

Thread::~Thread()
{
    if(!m_thread)
        return;

    if(m_thread->joinable())
        m_thread->join();

    delete m_thread;
}



void Thread::start(const LST::Function<void()>& _task, const LST::Function<void()>& _callback_on_completion)
{
    L_ASSERT_WITH_INFO(m_completed, "Trying to create a new thread when old one is still running");

    if(m_thread)
        delete m_thread;

    m_completed = false;

    m_thread = new std::thread([this, _task, _callback_on_completion]()
    {
        _task();
        m_completed = true;

        if(_callback_on_completion)
            _callback_on_completion();
    });
}

void Thread::detach()
{
    L_ASSERT_WITH_INFO(m_thread, "Trying to detach a non-existing thread");
    L_ASSERT_WITH_INFO(m_thread->joinable(), "Trying to detach a thread that has already been detached");

    m_thread->detach();
}

void Thread::wait_for_completion()
{
    L_ASSERT_WITH_INFO(m_thread, "Trying to join a non-existing thread");
    L_ASSERT_WITH_INFO(m_thread->joinable(), "Trying to join a thread that has already been detached");

    m_thread->join();
}
