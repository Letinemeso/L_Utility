#include <Stuff/Quantized_Vector.h>

using namespace LST;


Quantized_Vector::Quantized_Vector()
{
    for(unsigned int i = 0; i < 3; ++i)
        m_storage[i] = 0;
}

Quantized_Vector::Quantized_Vector(const glm::vec3& _from)
{
    for(unsigned int i = 0; i < 3; ++i)
        m_storage[i] = _from[i] * s_factor_inverted;
}

Quantized_Vector::Quantized_Vector(const Quantized_Vector& _other)
{
    for(unsigned int i = 0; i < 3; ++i)
        m_storage[i] = _other.m_storage[i];
}

void Quantized_Vector::operator=(const Quantized_Vector& _other)
{
    for(unsigned int i = 0; i < 3; ++i)
        m_storage[i] = _other.m_storage[i];
}



glm::vec3 Quantized_Vector::to_vec() const
{
    glm::vec3 result;
    for(unsigned int i = 0; i < 3; ++i)
        result[i] = (float)(m_storage[i]) * s_factor;
    return result;
}


bool Quantized_Vector::operator==(const Quantized_Vector& _other) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        if(m_storage[i] != _other.m_storage[i])
            return false;
    }
    return true;
}

bool Quantized_Vector::operator<(const Quantized_Vector& _other) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        if(m_storage[i] < _other.m_storage[i])
            return true;
        if(m_storage[i] > _other.m_storage[i])
            return false;
    }
    return false;
}

bool Quantized_Vector::operator>(const Quantized_Vector& _other) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        if(m_storage[i] > _other.m_storage[i])
            return true;
        if(m_storage[i] < _other.m_storage[i])
            return false;
    }
    return false;
}
