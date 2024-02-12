#pragma once

#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    class Command_Queue
    {
    private:
        using Command = LST::Function<bool()>;
        using Commands_List = LDS::List<Command>;

    private:
        Commands_List m_commands_list;

    public:
        void call(const LST::Function<void()>& _func);
        void wait(const LST::Function<bool()>& _func, bool _expected_result = true);

        void clear();

    public:
        inline bool empty() const { return m_commands_list.size() == 0; }

    public:
        void process();

    };

}
