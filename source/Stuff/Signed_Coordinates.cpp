#include <Stuff/Signed_Coordinates.h>

using namespace LST;


bool Signed_Coordinates::operator<(const Signed_Coordinates& _other) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        if(m_data[i] < _other[i])
            return true;
    }
    return false;
}

bool Signed_Coordinates::operator>(const Signed_Coordinates& _other) const
{
    for(unsigned int i = 0; i < 3; ++i)
    {
        if(m_data[i] > _other[i])
            return true;
    }
    return false;
}


Signed_Coordinates Signed_Coordinates::operator+(const Signed_Coordinates& _other) const
{
    L_ASSERT(valid());
    L_ASSERT(_other.valid());

    Signed_Coordinates result;
    for(unsigned int i = 0; i < 3; ++i)
        result.m_data[i] = m_data[i] + _other.m_data[i];

    return result;
}

Signed_Coordinates Signed_Coordinates::operator-(const Signed_Coordinates& _other) const
{
    L_ASSERT(valid());
    L_ASSERT(_other.valid());

    Signed_Coordinates result;
    for(unsigned int i = 0; i < 3; ++i)
    {
        L_ASSERT(_other.m_data[i] <= m_data[i]);
        result.m_data[i] = m_data[i] - _other.m_data[i];
    }

    return result;
}


Signed_Coordinates Signed_Coordinates::center(const Signed_Coordinates& _first, const Signed_Coordinates& _second)
{
    L_ASSERT(_first.valid());
    L_ASSERT(_second.valid());

    Signed_Coordinates result = _first + _second;
    for(unsigned int i = 0; i < 3; ++i)
        result[i] /= 2;

    return result;
}
