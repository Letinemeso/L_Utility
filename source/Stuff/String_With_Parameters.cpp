#include <Stuff/String_With_Parameters.h>
#include <L_Debug/L_Debug.h>

#include <regex>

using namespace LST;


String_With_Parameters::String_With_Parameters(const std::string& _string)
{
    m_string = _string;
}

String_With_Parameters::String_With_Parameters(std::string&& _string)
{
    m_string = (std::string&&)_string;
}



String_With_Parameters& String_With_Parameters::replace_parameter(unsigned int _parameter, const std::string& _with)
{
    std::string parameter_string = "\\{" + std::to_string(_parameter) + "\\}";

    m_string = std::regex_replace(m_string, std::regex(parameter_string), _with);

    return *this;
}
