#include <Stuff/Mask.h>

using namespace LST;


void Mask::M_process_pattern(const std::string &_raw_pattern)
{
    m_necessary_parts.clear();
    m_any_start = false;
    m_any_end = false;

    if(_raw_pattern.size() == 0 || _raw_pattern == "*")
    {
        m_any_start = true;
        m_any_end = true;
        return;
    }

    unsigned int offset = 0;

    if(_raw_pattern[0] == '*')
    {
        ++offset;
        m_any_start = true;
    }
    for(unsigned int i=offset + 1; i<_raw_pattern.size(); ++i)
    {
        if(i > offset && _raw_pattern[i] == '*')
        {
            m_necessary_parts.push_back(_raw_pattern.substr(offset, i - offset));
            offset = i + 1;
        }
        if(_raw_pattern[i] == '*')
            offset = i + 1;
    }

    if(_raw_pattern[_raw_pattern.size() - 1] == '*')
        m_any_end = true;
    else if(offset < _raw_pattern.size())
        m_necessary_parts.push_back(_raw_pattern.substr(offset, _raw_pattern.size()));

    return;
}



unsigned int Mask::M_substr_offset(const std::string &_raw, unsigned int _offset, const std::string &_substr) const
{
    if(_raw.size() - _offset < _substr.size())
        return _raw.size();

    for(unsigned int i=_offset; i<_raw.size() - _substr.size() + 1; ++i)
    {
        if(_raw[i] != _substr[0])
            continue;

        unsigned int is = 1;
        for(; is<_substr.size(); ++is)
        {
            if(_substr[is] != _raw[is + i])
                break;
        }

        if(is == _substr.size())
            return i;

        i += is;
    }

    return _raw.size();
}



Mask::Mask()
{

}

Mask::Mask(const std::string& _raw_pattern)
    : m_raw_pattern(_raw_pattern)
{
    M_process_pattern(m_raw_pattern);
}

void Mask::operator=(const std::string& _raw_pattern)
{
    m_raw_pattern = _raw_pattern;
    M_process_pattern(m_raw_pattern);
}



bool Mask::compare(const std::string &_text) const
{
    if(m_necessary_parts.size() == 0 && m_any_start && m_any_end)
        return true;
    if(m_necessary_parts.size() == 0)
        return false;

    unsigned int offset = 0;

    String_List::Const_Iterator it = m_necessary_parts.begin();
    offset = M_substr_offset(_text, offset, *it);
    if(!m_any_start && offset > 0)
        return false;
    if(offset == _text.size())
        return false;

    offset += it->size();
    ++it;

    for(; !it.end_reached(); ++it)
    {
        offset = M_substr_offset(_text, offset, *it);

        if(offset == _text.size())
            return false;

        offset += it->size();
    }

    if(offset < _text.size() && !m_any_end)
        return false;

    return true;
}
