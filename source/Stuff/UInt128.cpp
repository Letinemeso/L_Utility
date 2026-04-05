#include <Stuff/UInt128.h>

using namespace LST;


UInt128::UInt128()
{

}

UInt128::UInt128(Part_Type _upper_part, Part_Type _lower_part)
{
    from_uint(_upper_part, _lower_part);
}

UInt128::UInt128(const UInt128& _other)
{
    m_raw[0] = _other.m_raw[0];
    m_raw[1] = _other.m_raw[1];
}

UInt128::~UInt128()
{

}


void UInt128::operator=(const UInt128& _other)
{
    m_raw[0] = _other.m_raw[0];
    m_raw[1] = _other.m_raw[1];
}


void UInt128::from_uint(Part_Type _upper_part, Part_Type _lower_part)
{
    m_raw[0] = _lower_part;
    m_raw[1] = _upper_part;
}



bool UInt128::operator==(const UInt128& _other) const
{
    return m_raw[0] == _other.m_raw[0] && m_raw[1] == _other.m_raw[1];
}

bool UInt128::operator<(const UInt128& _other) const
{
    if(m_raw[1] < _other.m_raw[1])
        return true;
    if(m_raw[1] > _other.m_raw[1])
        return false;

    return m_raw[0] < _other.m_raw[0];
}

bool UInt128::operator>(const UInt128& _other) const
{
    if(m_raw[1] > _other.m_raw[1])
        return true;
    if(m_raw[1] < _other.m_raw[1])
        return false;

    return m_raw[0] > _other.m_raw[0];
}

bool UInt128::operator<=(const UInt128& _other) const
{
    return (*this < _other) || (*this == _other);
}

bool UInt128::operator>=(const UInt128& _other) const
{
    return (*this > _other) || (*this == _other);
}
