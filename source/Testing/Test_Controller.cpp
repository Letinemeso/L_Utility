#include <Testing/Test_Controller.h>

#include <thread>
#include <chrono>

#include <Data_Structures/Vector.h>
#include <Stuff/Mask.h>

#include <Testing/Test.h>


using namespace LTest;

Test_Controller::Test_Controller()
{

}

Test_Controller::~Test_Controller()
{
    for(unsigned int i = 0; i < m_tests.size(); ++i)
        delete m_tests[i].test;
}



void Test_Controller::add_test(const std::string& _name, Test* _test)
{
    m_tests.push({_name, _test});
}



void Test_Controller::run_tests()
{
    LDS::Vector<Test_Data> tests_to_run(m_tests.size());

    if(m_tests_mask.empty())
        std::cout << "Running all tests" << std::endl;
    else
        std::cout << "Running tests with name mask " << m_tests_mask << std::endl;

    LST::Mask mask;
    if(m_tests_mask.empty())
        mask = "*";
    else
        mask = m_tests_mask;

    for(unsigned int i = 0; i < m_tests.size(); ++i)
    {
        if(mask.compare(m_tests[i].name))
            tests_to_run.push(m_tests[i]);
    }

    LDS::Vector<std::string> failed_tests(m_tests.size());

    for(unsigned int i = 0; i < tests_to_run.size(); ++i)
    {
        const std::string& test_name = tests_to_run[i].name;
        Test* test = tests_to_run[i].test;

        std::cout << "Running \"" << test_name << "\"" << std::endl;

        test->reset();
        test->run();

        if(!test->success())
            failed_tests.push(test_name);
    }
    std::cout << std::endl;

    std::cout << "Ran " << tests_to_run.size() << " tests" << std::endl
              << "Succeeded: " << tests_to_run.size() - failed_tests.size() << std::endl
              << "Failed: " << failed_tests.size() << std::endl;

    if(failed_tests.size() > 0)
    {
        std::cout << "\nFailed tests: " << std::endl;
        for(unsigned int i = 0; i < failed_tests.size(); ++i)
            std::cout << i + 1 << ": " << failed_tests[i] << std::endl;
        std::cout << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));     //  waiting for console to finish printing stuff
}
