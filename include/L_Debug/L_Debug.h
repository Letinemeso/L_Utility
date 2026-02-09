#pragma once

#include <iostream>

//#ifdef L_DEBUG
//	#undef L_DEBUG
//#endif
//#define L_DEBUG true

#ifdef L_DEBUG

		//	USE THIS ONLY WITH MACRO BELOW
		namespace ___L_Debug
		{
			void ___debug_create_log_level(const std::string& _level_name);
			void ___debug_remove_log_level(const std::string& _level_name);
			void ___debug_log(const std::string& _level_name, const std::string& _message);
		}

		#ifdef L_LOG
			#undef L_LOG
		#endif
		#define L_LOG(level, message) ___L_Debug::___debug_log(level, message)

		#ifdef L_CREATE_LOG_LEVEL
			#undef L_CREATE_LOG_LEVEL
		#endif
		#define L_CREATE_LOG_LEVEL(level) ___L_Debug::___debug_create_log_level(level)

		#ifdef L_REMOVE_LOG_LEVEL
			#undef L_REMOVE_LOG_LEVEL
		#endif
		#define L_REMOVE_LOG_LEVEL(level) ___L_Debug::___debug_remove_log_level(level)

		#ifdef L_DEBUG_FUNC_NOARG
			#undef L_DEBUG_FUNC_NOARG
		#endif
		#define L_DEBUG_FUNC_NOARG(func) func()

		#ifdef L_DEBUG_FUNC_1ARG
			#undef L_DEBUG_FUNC_1ARG
		#endif
		#define L_DEBUG_FUNC_1ARG(func, arg) func(arg)

		#ifdef L_DEBUG_FUNC_2ARG
			#undef L_DEBUG_FUNC_2ARG
		#endif
		#define L_DEBUG_FUNC_2ARG(func, arg1, arg2) func(arg1, arg2)

		#ifdef L_ASSERT
			#undef L_ASSERT
		#endif
        #define L_ASSERT(condition) if(!(condition)) { std::cout << "Assert failed at\n" << __FILE__ << " (" << __LINE__ << ") \ncondition: " << #condition << std::endl; std::abort(); } 1 == 1

        #ifdef L_ASSERT_WITH_INFO
            #undef L_ASSERT_WITH_INFO
        #endif
        #define L_ASSERT_WITH_INFO(condition, info) if(!(condition)) { std::cout << "Assert failed at\n" << __FILE__ << " (" << __LINE__ << ") \ncondition: " << #condition << "\ninfo: " << info << std::endl; std::abort(); } 1 == 1


    #else	//L_DEBUG == true

		#ifdef L_LOG
			#undef L_LOG
		#endif
		#define L_LOG(level, message)

		#ifdef L_CREATE_LOG_LEVEL
			#undef L_CREATE_LOG_LEVEL
		#endif
		#define L_CREATE_LOG_LEVEL(level)

		#ifdef L_REMOVE_LOG_LEVEL
			#undef L_REMOVE_LOG_LEVEL
		#endif
		#define L_REMOVE_LOG_LEVEL(level)

		#ifdef L_DEBUG_FUNC_NOARG
			#undef L_DEBUG_FUNC_NOARG
		#endif
		#define L_DEBUG_FUNC_NOARG(func)

		#ifdef L_DEBUG_FUNC_1ARG
			#undef L_DEBUG_FUNC_1ARG
		#endif
		#define L_DEBUG_FUNC_1ARG(func, arg)

		#ifdef L_DEBUG_FUNC_2ARG
			#undef L_DEBUG_FUNC_2ARG
		#endif
		#define L_DEBUG_FUNC_2ARG(func, arg1, arg2)

		#ifdef L_ASSERT
			#undef L_ASSERT
		#endif
		#define L_ASSERT(condition)

        #ifdef L_ASSERT_WITH_INFO
            #undef L_ASSERT_WITH_INFO
        #endif
        #define L_ASSERT_WITH_INFO(condition, info)

#endif	//L_DEBUG
