#pragma once

#include <Stuff/Function_Wrapper.h>
#include <Data_Structures/List.h>
#include <Data_Structures/Map.h>


namespace LST
{

    class Message_Translator final
    {
    private:
        using Subscribers_List = LDS::List<LST::Function<void(const void*)>>;
        using Registred_Message_Types = LDS::Map<std::string, Subscribers_List>;

    private:
        Registred_Message_Types m_registred_message_types;

    private:
        Message_Translator() { }

    public:
        Message_Translator(const Message_Translator&) = delete;
        Message_Translator(Message_Translator&&) = delete;
        void operator=(const Message_Translator&) = delete;
        void operator=(Message_Translator&&) = delete;

    public:
        inline static Message_Translator& instance() { static Message_Translator mt_instance; return mt_instance; }

    public:
        template<typename Message_Type>
        void register_message_type();

        template<typename Message_Type>
        void subscribe(const LST::Function<void(const Message_Type&)>& _func);

        template<typename Message_Type>
        void translate(const Message_Type& _msg);

    };



    template<typename Message_Type>
    void Message_Translator::register_message_type()
    {
        L_ASSERT(!m_registred_message_types.find(Message_Type::__message_name_str()).is_ok());

        m_registred_message_types.insert(Message_Type::__message_name_str(), {});
    }

    template<typename Message_Type>
    void Message_Translator::subscribe(const LST::Function<void(const Message_Type&)>& _func)
    {
        Registred_Message_Types::Iterator maybe_registred_type_it = m_registred_message_types.find(Message_Type::__message_name_str());
        L_ASSERT(maybe_registred_type_it.is_ok());

        Subscribers_List& subscribers_list = *maybe_registred_type_it;

        subscribers_list.push_back([_func](const void* _msg_voidptr)
        {
            const Message_Type* _msg = (const Message_Type*)_msg_voidptr;
            _func(*_msg);
        });
    }

    template<typename Message_Type>
    void Message_Translator::translate(const Message_Type& _msg)
    {
        Registred_Message_Types::Iterator maybe_registred_type_it = m_registred_message_types.find(Message_Type::__message_name_str());
        L_ASSERT(maybe_registred_type_it.is_ok());

        const void* msg_voidptr = (const void*)&_msg;

        Subscribers_List& subscribers_list = *maybe_registred_type_it;

        for(Subscribers_List::Iterator it = subscribers_list.begin(); !it.end_reached(); ++it)
            (*it)(msg_voidptr);
    }




}
