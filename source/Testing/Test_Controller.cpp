#include <Testing/Test_Controller.h>

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
    for(Tests_Map::Iterator it = m_tests.iterator(); !it.end_reached(); ++it)
    {
        Test* test = *it;
        test->run();
    }
}
