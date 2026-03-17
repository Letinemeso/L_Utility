#include <Testing/Test_Controller.h>

#include <thread>
#include <chrono>

#include <Data_Structures/Vector.h>

#include <Testing/Test.h>


using namespace LTest;

Test_Controller::Test_Controller()
{

}

Test_Controller::~Test_Controller()
{
    for(Tests_Map::Iterator it = m_tests.iterator(); !it.end_reached(); ++it)
        delete *it;
}



void Test_Controller::add_test(const std::string& _name, Test* _test)
{
    L_ASSERT(!m_tests.find(_name).is_ok());
    m_tests.insert(_name, _test);
}



void Test_Controller::run_tests()
{
    LDS::Vector<std::string> failed_tests(m_tests.size());

    for(Tests_Map::Iterator it = m_tests.iterator(); !it.end_reached(); ++it)
    {
        std::cout << "Running \"" << it.key() << "\"" << std::endl;

        Test* test = *it;
        test->reset();
        test->run();

        if(!test->success())
            failed_tests.push(it.key());
    }
    std::cout << std::endl;

    std::cout << "Ran " << m_tests.size() << " tests" << std::endl
              << "Succeeded: " << m_tests.size() - failed_tests.size() << std::endl
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
