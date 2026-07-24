#include <Stuff/Coordinates.h>

using namespace LST;


Coordinates Coordinates::operator+(const Coordinates& _other) const
{
    L_ASSERT(valid());
    L_ASSERT(_other.valid());

    Coordinates result;
    for(unsigned int i = 0; i < 3; ++i)
        result.m_data[i] = m_data[i] + _other.m_data[i];

    return result;
}

Coordinates Coordinates::operator-(const Coordinates& _other) const
{
    L_ASSERT(valid());
    L_ASSERT(_other.valid());

    Coordinates result;
    for(unsigned int i = 0; i < 3; ++i)
    {
        L_ASSERT(_other.m_data[i] <= m_data[i]);
        result.m_data[i] = m_data[i] - _other.m_data[i];
    }

    return result;
}


Coordinates Coordinates::center(const Coordinates& _first, const Coordinates& _second)
{
    L_ASSERT(_first.valid());
    L_ASSERT(_second.valid());

    Coordinates result = _first + _second;
    for(unsigned int i = 0; i < 3; ++i)
        result[i] /= 2;

    return result;
}
