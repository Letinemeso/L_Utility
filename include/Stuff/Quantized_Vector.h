#pragma once

#include <vec3.hpp>

#include <L_Debug/L_Debug.h>


namespace LST
{

    class Quantized_Vector
    {
    private:
        inline static float s_factor = 1.0f;
        inline static float s_factor_inverted = 1.0f;

    private:
        int m_storage[3];

    public:
        Quantized_Vector();
        Quantized_Vector(const glm::vec3& _from);
        Quantized_Vector(const Quantized_Vector& _other);
        void operator=(const Quantized_Vector& _other);

    public:
        inline static void set_factor(float _value) { s_factor = _value; s_factor_inverted = 1.0f / s_factor; }
        inline static float factor() { return s_factor; }

    public:
        inline int& x() { return m_storage[0]; }
        inline int& y() { return m_storage[1]; }
        inline int& z() { return m_storage[2]; }

        inline int& operator[](unsigned int _index) { L_ASSERT(_index < 3); return m_storage[_index]; }


    public:
        glm::vec3 to_vec() const;

        bool operator==(const Quantized_Vector& _other) const;
        bool operator<(const Quantized_Vector& _other) const;
        bool operator>(const Quantized_Vector& _other) const;
    };

}
