#include <L_Debug/L_Debug.h>

#include <Data_Structures/AVL_Tree.h>

#if L_DEBUG == true

LDS::AVL_Tree<std::string> ___AVAILIBLE_LOG_LEVELS;

void ___L_Debug::___debug_create_log_level(const std::string& _level_name)
{
    if(!___AVAILIBLE_LOG_LEVELS.find(_level_name).is_ok())
        ___AVAILIBLE_LOG_LEVELS.insert(_level_name);
}

void ___L_Debug::___debug_remove_log_level(const std::string& _level_name)
{
    LDS::AVL_Tree<std::string>::Iterator it = ___AVAILIBLE_LOG_LEVELS.find(_level_name);
    if(it.is_ok())
		___AVAILIBLE_LOG_LEVELS.erase(it);
}

void ___L_Debug::___debug_log(const std::string& _level_name, const std::string& _message)
{
    if(!___AVAILIBLE_LOG_LEVELS.find(_level_name).is_ok())
        return;

    std::cout << '[' << _level_name << "]: " << _message << "\n";
    std::cout.flush();
}

#endif
