#pragma once

#include <string>


#define MESSAGE_TYPE_DECLARATION_START(MESSAGE_NAME)                                            \
    struct MESSAGE_NAME                                                                         \
    {                                                                                           \
        static const std::string& __message_name_str() { static std::string str(#MESSAGE_NAME); return str; }

#define MESSAGE_TYPE_DECLARATION_END };
