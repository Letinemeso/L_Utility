#pragma once

#include <string>
#include <Data_Structures/List.h>


namespace LST
{

    class Mask
    {
    private:
        using String_List = LDS::List<std::string>;

    private:
        std::string m_raw_pattern;
        String_List m_necessary_parts;
        bool m_any_start = false;
        bool m_any_end = false;

    private:
        void M_process_pattern(const std::string& _raw_pattern);

    private:
        unsigned int M_substr_offset(const std::string& _raw, unsigned int _offset, const std::string& _substr) const;

    public:
        Mask();
        Mask(const std::string& _raw_pattern);
        void operator=(const std::string& _raw_pattern);

    public:
        bool compare(const std::string& _text);

    };

}
