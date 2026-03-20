#pragma once

#include <Data_Structures/Vector.h>


namespace LTest
{

    class Test;

    class Test_Controller
    {
    private:
        struct Test_Data
        {
            std::string name;
            Test* test;
        };

        LDS::Vector<Test_Data> m_tests;

        std::string m_tests_mask;

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
        inline void set_tests_mask(const std::string& _value) { m_tests_mask = _value; }

    public:
        void add_test(const std::string& _name, Test* _test);

    public:
        void run_tests();

    };

}
