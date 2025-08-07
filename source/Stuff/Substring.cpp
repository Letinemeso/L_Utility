#include <Stuff/Substring.h>

#include <L_Debug/L_Debug.h>

using namespace LST;


Substring::Substring(std::string& _string, unsigned int _offset, unsigned int _size)
    : m_raw_string(_string), m_offset(_offset), m_size(_size)
{
    L_ASSERT(m_offset < m_raw_string.size());

    if(m_size == Unlimited_Size)
        m_size = m_raw_string.size() - m_offset;

    L_ASSERT(m_offset + m_size <= m_raw_string.size());
}


Substring::~Substring()
{

}



void Substring::set_parameters(unsigned int _offset, unsigned int _size)
{
    m_offset = _offset;
    m_size = _size;

    L_ASSERT(m_offset < m_raw_string.size());

    if(m_size == Unlimited_Size)
        m_size = m_raw_string.size() - m_offset;

    L_ASSERT(m_offset + m_size <= m_raw_string.size());
}

void Substring::move_offset(int _increment)
{
    if(_increment == 0)
        return;

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(_increment > 0)
            L_ASSERT(_increment < m_size);
        if(_increment < 0)
            L_ASSERT(-_increment <= m_offset);
    });

    m_offset += _increment;
    m_size -= _increment;
}



char& Substring::operator[](unsigned int _index)
{
    L_ASSERT(_index < m_size);

    unsigned int raw_index = _index + m_offset;
    L_ASSERT(raw_index < m_raw_string.size());

    return m_raw_string[raw_index];
}

char Substring::operator[](unsigned int _index) const
{
    L_ASSERT(_index < m_size);

    unsigned int raw_index = _index + m_offset;
    L_ASSERT(raw_index < m_raw_string.size());

    return m_raw_string[raw_index];
}


std::string Substring::copy_substring() const
{
    L_ASSERT(m_offset < m_raw_string.size());
    L_ASSERT(m_offset + m_size <= m_raw_string.size());

    return m_raw_string.substr(m_offset, m_size);
}





Const_Substring::Const_Substring(const std::string& _string, unsigned int _offset, unsigned int _size)
    : m_raw_string(_string), m_offset(_offset), m_size(_size)
{
    L_ASSERT(m_offset < m_raw_string.size());

    if(m_size == Unlimited_Size)
        m_size = m_raw_string.size() - m_offset;

    L_ASSERT(m_offset + m_size <= m_raw_string.size());
}


Const_Substring::~Const_Substring()
{

}



void Const_Substring::set_parameters(unsigned int _offset, unsigned int _size)
{
    m_offset = _offset;
    m_size = _size;

    L_ASSERT(m_offset < m_raw_string.size());

    if(m_size == Unlimited_Size)
        m_size = m_raw_string.size() - m_offset;

    L_ASSERT(m_offset + m_size <= m_raw_string.size());
}

void Const_Substring::move_offset(int _increment)
{
    if(_increment == 0)
        return;

    L_DEBUG_FUNC_NOARG([&]()
    {
        if(_increment > 0)
            L_ASSERT(_increment < m_size);
        if(_increment < 0)
            L_ASSERT(-_increment <= m_offset);
    });

    m_offset += _increment;
    m_size -= _increment;
}



char Const_Substring::operator[](unsigned int _index) const
{
    L_ASSERT(_index < m_size);

    unsigned int raw_index = _index + m_offset;
    L_ASSERT(raw_index < m_raw_string.size());

    return m_raw_string[raw_index];
}


std::string Const_Substring::copy_substring() const
{
    L_ASSERT(m_offset < m_raw_string.size());
    L_ASSERT(m_offset + m_size <= m_raw_string.size());

    return m_raw_string.substr(m_offset, m_size);
}
