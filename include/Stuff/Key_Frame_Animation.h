#pragma once

#include <Data_Structures/List.h>
#include <Stuff/Function_Wrapper.h>


namespace LST
{

    template<typename Type>
    class Key_Frame_Animation
    {
    private:
        struct Frame
        {
            float timestamp;
            Type target;
        };

    private:
        using Frames_List = LDS::List<Frame>;
        Frames_List m_frames;

        bool m_is_active = false;
        bool m_run_in_loop = false;

        typename Frames_List::Iterator m_offset_it;
        typename Frames_List::Iterator m_target_it;

        float m_offset_timestamp = 0.0f;
        float m_stride_timestamp = 0.0f;
        Type m_offset;
        Type m_stride;

        float m_current_timestamp = 0.0f;
        Type m_current_value;

        LST::Function<void(const Type&)> m_on_update;

    public:
        inline void set_on_update_func(const LST::Function<void(const Type&)>& _func) { m_on_update = _func; }
        inline void set_run_in_loop(bool _value) { m_run_in_loop = _value; }

        void add_frame(float _timestamp, const Type& _target);
        void clear();

    public:
        inline const Type& current_value() const { return m_current_value; }
        inline float current_timestamp() const { return m_current_timestamp; }

    public:
        void start();
        void stop();

    public:
        inline bool active() const { return m_is_active; }
        inline unsigned int frames_amount() const { return m_frames.size(); }

    private:
        void M_skip_to_current_frame();

    public:
        void update(float _dt);

    };


    template<typename Type>
    void Key_Frame_Animation<Type>::add_frame(float _timestamp, const Type& _target)
    {
        L_ASSERT(_timestamp >= -0.000001f);
        L_ASSERT( ! (m_frames.size() == 0 && _timestamp > 0.0001f) );   //  first frame must be at the start

        typename Frames_List::Iterator frame_it = m_frames.begin();
        for(; !frame_it.end_reached(); ++frame_it)
        {
            if(frame_it->timestamp > _timestamp)
                break;
        }

        m_frames.push_before(frame_it, {_timestamp, _target});
    }

    template<typename Type>
    void Key_Frame_Animation<Type>::clear()
    {
        m_frames.clear();
    }



    template<typename Type>
    void Key_Frame_Animation<Type>::start()
    {
        L_ASSERT(m_frames.size() >= 2);

        m_offset_it = m_frames.begin();
        m_target_it = m_frames.begin();
        ++m_target_it;

        m_offset_timestamp = 0.0f;
        m_stride_timestamp = m_target_it->timestamp;

        m_offset = m_offset_it->target;
        m_stride = m_target_it->target - m_offset_it->target;

        m_current_timestamp = 0.0f;
        m_current_value = m_offset;

        m_is_active = true;
    }

    template<typename Type>
    void Key_Frame_Animation<Type>::stop()
    {
        m_is_active = false;
    }



    template<typename Type>
    void Key_Frame_Animation<Type>::M_skip_to_current_frame()
    {
        if(m_current_timestamp <= m_stride_timestamp + m_offset_timestamp)
            return;

        for(; !m_target_it.end_reached(); ++m_target_it)
        {
            if(m_current_timestamp < m_target_it->timestamp)
                break;
        }

        if(m_target_it.end_reached())
        {
            stop();
            return;
        }

        m_offset_it = m_target_it;
        --m_offset_it;

        m_offset_timestamp = m_offset_it->timestamp;
        m_stride_timestamp = m_target_it->timestamp - m_offset_timestamp;

        m_offset = m_offset_it->target;
        m_stride = m_target_it->target - m_offset;
    }



    template<typename Type>
    void Key_Frame_Animation<Type>::update(float _dt)
    {
        if(!m_is_active)
            return;

        m_current_timestamp += _dt;

        M_skip_to_current_frame();

        if(!m_is_active && m_run_in_loop)
            return start();

        if(!m_is_active)
            return;

        float ratio = 1.0f;;
        if(m_stride_timestamp > 0.0f)
            ratio = (m_current_timestamp - m_offset_timestamp) / m_stride_timestamp;

        m_current_value = m_stride * ratio + m_offset;

        if(m_on_update)
            m_on_update(m_current_value);
    }

}
