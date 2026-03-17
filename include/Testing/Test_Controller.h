#pragma once

#include <Data_Structures/Map.h>


namespace LTest
{

    class Test;

    class Test_Controller
    {
    private:
        using Tests_Map = LDS::Map<std::string, Test*>;
        LDS::Map<std::string, Test*> m_tests;

    private:
        Test_Controller();

    public:
        ~Test_Controller();

    public:
        Test_Controller(const Test_Controller&) = delete;
        Test_Controller(Test_Controller&&) = delete;
        void operator=(const Test_Controller&) = delete;
        void operator=(Test_Controller&&) = delete;

    public:
        static inline Test_Controller& instance() { static Test_Controller s_instance; return s_instance; }

    public:
        void add_test(const std::string& _name, Test* _test);

    public:
        void run_tests();

    };

}
