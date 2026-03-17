#pragma once

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
