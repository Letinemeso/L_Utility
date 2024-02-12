#include <Stuff/Command_Queue.h>

using namespace LST;


void Command_Queue::call(const LST::Function<void()>& _func)
{
    m_commands_list.push_back([_func]()
    {
        _func();
        return true;
    });
}

void Command_Queue::wait(const LST::Function<bool()>& _func, bool _expected_result)
{
    m_commands_list.push_back([_func, _expected_result]()
    {
        return _func() == _expected_result;
    });
}


void Command_Queue::clear()
{
    m_commands_list.clear();
}



void Command_Queue::process()
{
    while(m_commands_list.size() > 0)
    {
        Commands_List::Iterator command_it = m_commands_list.begin();
        Command& command = *command_it;

        if(!command())
            break;

        m_commands_list.erase(command_it);
    }
}
