#pragma once

#include <sstream>

#include <L_Debug/L_Debug.h>

#include <Testing/Test.h>
#include <Testing/Test_Controller.h>

namespace __LTest_Utility
{

    template<typename _Test_Type>
    class Test_Registrator
    {
    public:
        Test_Registrator(const std::string& _test_name)
        {
            LTest::Test_Controller::instance().add_test(_test_name, new _Test_Type);
        }

    };

}


#define RUN_TESTS \
    int main() \
    { \
        LTest::Test_Controller::instance().run_tests(); \
    }

#define INIT_TEST(_Test_Name) \
    class _Test_Name : public LTest::Test \
    { \
    public: \
        void run() override; \
    }; \
    using Test_Name__ ## _Test_Name = _Test_Name; \
    \
    __LTest_Utility::Test_Registrator<_Test_Name> test_registrator__ ## _Test_Name( #_Test_Name ); \
    \
    void Test_Name__ ## _Test_Name::run()

#define __PRINT_ERROR_MESSAGE(_first, _second, _condition_sign) \
    std::cout << "Check failed at "; \
    std::cout << __FILE__ << " (" << __LINE__ << "):\n"; \
    std::cout << "Condition: \"" << #_first << " " << _condition_sign << " " << #_second << "\""; \
    std::cout << "\n"; \
    std::cout << "where\n"; \
    std::cout << #_first << " == " << _first; \
    std::cout << "\n"; \
    std::cout << #_second << " == " << _second; \
    std::cout << std::endl; \

#define EXPECT_EQUAL(_first, _second) \
    if(_first != _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, "==") \
        mark_failed(); \
    }

#define EXPECT_NOT_EQUAL(_first, _second) \
    if(_first == _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, "!=") \
        mark_failed(); \
    }

#define EXPECT_LESS(_first, _second) \
    if(_first >= _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, "<") \
        mark_failed(); \
    }

#define EXPECT_MORE(_first, _second) \
    if(_first <= _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, ">") \
        mark_failed(); \
    }

#define EXPECT_LESS_OR_EQUAL(_first, _second) \
    if(_first > _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, "<=") \
        mark_failed(); \
    }

#define EXPECT_MORE_OR_EQUAL(_first, _second) \
    if(_first < _second) \
    { \
        __PRINT_ERROR_MESSAGE(_first, _second, ">=") \
        mark_failed(); \
    }
