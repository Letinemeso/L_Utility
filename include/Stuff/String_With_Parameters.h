#pragma once

#include <string>


namespace LST
{

    class String_With_Parameters
    {
    private:
        std::string m_string;

    public:
        String_With_Parameters(const std::string& _string);
        String_With_Parameters(std::string&& _string);

    public:
        inline operator const std::string&() { return m_string; }

    public:
        String_With_Parameters& replace_parameter(unsigned int _parameter, const std::string& _with);

    };

}
