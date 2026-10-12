#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>

#include <Data_Structures/Vector.h>


namespace LST
{

    class Border_Construction_Helper final
    {
    private:
        glm::vec3 m_min = glm::vec3(std::numeric_limits<float>::max());
        glm::vec3 m_max = glm::vec3(std::numeric_limits<float>::lowest());

    public:
        void reset();
        void consider_point_component(float _value, unsigned int _index);
        void consider_point(const glm::vec3& _point);

    public:
        inline const glm::vec3& min() { return m_min; }
        inline const glm::vec3& max() { return m_max; }

        inline glm::vec3 calculate_center() const { return (m_min + m_max) * 0.5f; }
        inline glm::vec3 calculate_extents() const { return (m_max - m_min) * 0.5f; }

        inline bool valid() const { return m_min[0] <= m_max[0]; }

    };


    class Border final
    {
    private:
        glm::vec3 m_local_center;
        glm::vec3 m_local_extents;

        glm::vec3 m_global_center;
        glm::vec3 m_global_extents;

    public:
        Border();
        Border(const Border_Construction_Helper& _helper);
        Border(const LDS::Vector<float>& _raw_data);
        Border(const LDS::Vector<glm::vec3>& _points);
        ~Border();

    public:
        inline const glm::vec3& center() const { return m_global_center; }
        inline const glm::vec3& extents() const { return m_global_extents; }

        inline void invalidate() { m_global_extents[0] = -1.0f; }
        inline bool valid() const { return m_global_extents[0] >= 0.0f; }

        inline glm::vec3 calculate_offset() const { return m_global_center - m_global_extents; }
        inline glm::vec3 calculate_limit() const { return m_global_center + m_global_extents; }

    public:
        void init(const Border_Construction_Helper& _helper);
        void init(const float* _raw_data, unsigned int _raw_data_size);
        void init(const LDS::Vector<float>& _raw_data);
        void init(const LDS::Vector<glm::vec3>& _points);

        void set_values(const glm::vec3& _center, const glm::vec3& _extents);

        void update(const glm::mat4x4& _transformation);

    public:
        bool point_is_inside(const glm::vec3& _point) const;
        bool intersects_with(const Border& _other) const;

    public:
        Border& expand_with(const Border& _other);

        Border operator&&(const Border& _other) const;
        Border operator||(const Border& _other) const;

    };

}
