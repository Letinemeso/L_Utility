#pragma once

#include <Stuff/Function_Wrapper.h>
#include <Data_Structures/List.h>
#include <Data_Structures/Map.h>


namespace LST
{

    class Message_Translator final
    {
    public:
        template<typename Message_Type>
        class Handle
        {
        private:
            unsigned int handle = 0;

        private:
            Handle(unsigned int _handle) : handle(_handle) { }

        public:
            Handle() { }
            Handle(const Handle& _other) : handle(_other.handle) { }
            Handle(Handle&& _other) : handle(_other.handle) { }
            void operator=(const Handle& _other) { handle = _other.handle; }
            void operator=(Handle&& _other) {  handle = _other.handle; }

        public:
            friend class Message_Translator;

        };

    private:
        struct Subscriber
        {
            unsigned int handle = 0;
            LST::Function<void(void*)> function;
        };

        using Subscribers_List = LDS::List<Subscriber>;
        using Registred_Message_Types = LDS::Map<std::string, Subscribers_List>;

    private:
        unsigned int m_handle_counter = 0;
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
        Handle<Message_Type> subscribe(const LST::Function<void(Message_Type&)>& _func);

        template<typename Message_Type>
        void unsubscribe(const Handle<Message_Type>& _handle);

        template<typename Message_Type>
        void translate(Message_Type& _msg);

    };


    template<typename Message_Type>
    void Message_Translator::register_message_type()
    {
        L_ASSERT(!m_registred_message_types.find(Message_Type::__message_name_str()).is_ok());

        m_registred_message_types.insert(Message_Type::__message_name_str(), {});
    }

    template<typename Message_Type>
    typename Message_Translator::Handle<Message_Type> Message_Translator::subscribe(const LST::Function<void(Message_Type&)>& _func)
    {
        Registred_Message_Types::Iterator maybe_registred_type_it = m_registred_message_types.find(Message_Type::__message_name_str());
        L_ASSERT(maybe_registred_type_it.is_ok());

        Subscribers_List& subscribers_list = *maybe_registred_type_it;

        subscribers_list.push_back(
            {
                m_handle_counter,
                [_func](void* _msg_voidptr)
                {
                    Message_Type* _msg = (Message_Type*)_msg_voidptr;
                    _func(*_msg);
                }
            });

        Handle<Message_Type> handle(m_handle_counter);
        ++m_handle_counter;
        return handle;
    }

    template<typename Message_Type>
    void Message_Translator::unsubscribe(const Handle<Message_Type>& _handle)
    {
        Registred_Message_Types::Iterator maybe_registred_type_it = m_registred_message_types.find(Message_Type::__message_name_str());
        L_ASSERT(maybe_registred_type_it.is_ok());

        Subscribers_List& subscribers_list = *maybe_registred_type_it;

        for(Subscribers_List::Iterator it = subscribers_list.begin(); !it.end_reached(); ++it)
        {
            if(it->handle != _handle.handle)
                continue;

            subscribers_list.erase(it);
            return;
        }

        L_ASSERT(false);    //  attempt to unsubscribe non-subscriber
    }

    template<typename Message_Type>
    void Message_Translator::translate(Message_Type& _msg)
    {
        Registred_Message_Types::Iterator maybe_registred_type_it = m_registred_message_types.find(Message_Type::__message_name_str());
        L_ASSERT(maybe_registred_type_it.is_ok());

        void* msg_voidptr = (void*)&_msg;

        Subscribers_List& subscribers_list = *maybe_registred_type_it;

        for(Subscribers_List::Iterator it = subscribers_list.begin(); !it.end_reached(); ++it)
            it->function(msg_voidptr);
    }

}
