#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "shamean.hpp"

using namespace CppUnit;

//-----------------------------------------------------------------------------

struct s_test_data {
  char expected_checksum[41];
  char character;
  long length;
};

//-----------------------------------------------------------------------------

class TestShamean : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestShamean);
    CPPUNIT_TEST(testChecksumFile);
    CPPUNIT_TEST(testChecksumNonExistFile);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testChecksumFile(void);
    void testChecksumNonExistFile(void);
private:
    void testChecksum(s_test_data test_data);
};


//-----------------------------------------------------------------------------
void
TestShamean::testChecksumFile(void)
{
    // Test zero size file and boundary test
    s_test_data test_data_z1 = {"7156ECD78C70FC3349EBB604C9B934018EB1CBB2", 'a', 0};
    TestShamean::testChecksum(test_data_z1);
    /*s_test_data test_data_z2 = {"5F521D87B6D36D1EA5AC3F7CAC210AD82BCE5EDF", 'a', 1};
    TestShamean::testChecksum(test_data_z2);
 

    // Test small file
    s_test_data test_data_sf1 = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 8};
    TestShamean::testChecksum(test_data_sf1);

    // Boundary test first boundary 
    s_test_data test_data_fb0 = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 204799};
    TestShamean::testChecksum(test_data_fb0);
    s_test_data test_data_fb1 = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 204800};
    TestShamean::testChecksum(test_data_fb1);
    s_test_data test_data_fb2 = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 204801};
    TestShamean::testChecksum(test_data_fb2);


    // Boundary test second boundary
    {s_test_data test_data = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 409599};
    TestShamean::testChecksum(test_data);}
    {s_test_data test_data = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 409600};
    TestShamean::testChecksum(test_data);}
    {s_test_data test_data = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 409601};
    TestShamean::testChecksum(test_data);}*/

}

void
TestShamean::testChecksum(s_test_data test_data)
{
    char in_data[test_data.length];
    std::cout << test_data.length;
    for (long i = 0; i < test_data.length; i++)
    {
        std::cout << "adding character";
        in_data[i] = test_data.character;
    }

    // Create test file
    char test_filename[] = "test_file";
    std::ofstream test_file(test_filename, std::ofstream::trunc);
    test_file << in_data;
    test_file.close();

    // Create variables to pass to checksum_file
    unsigned char checksum[20];
    char out_checksum[40];
    bool open_err = false;

    checksum_file(test_filename, checksum, open_err);
    convert_to_hex(checksum, out_checksum);
    std::cout << out_checksum << std::endl;
    CPPUNIT_ASSERT(strcmp(out_checksum, test_data.expected_checksum) == 0);
    CPPUNIT_ASSERT(open_err == false);
    std::remove(test_filename);
}

void
TestShamean::testChecksumNonExistFile(void)
{
    unsigned char checksum[20];
    bool open_err = false;
    checksum_file("file_does_not_exist", checksum, open_err);
    CPPUNIT_ASSERT(open_err == true);
}

void TestShamean::setUp(void)
{
}

void TestShamean::tearDown(void)
{
}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( TestShamean );

int main(int argc, char* argv[])
{
    // informs test-listener about testresults
    CPPUNIT_NS::TestResult testresult;

    // register listener for collecting the test-results
    CPPUNIT_NS::TestResultCollector collectedresults;
    testresult.addListener (&collectedresults);

    // register listener for per-test progress output
    CPPUNIT_NS::BriefTestProgressListener progress;
    testresult.addListener (&progress);

    // insert test-suite at test-runner by registry
    CPPUNIT_NS::TestRunner testrunner;
    testrunner.addTest (CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
    testrunner.run(testresult);

    // output results in compiler-format
    CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
    compileroutputter.write ();

    // Output XML for Jenkins CPPunit plugin
    std::ofstream xmlFileOut("cppTestShameanResults.xml");
    XmlOutputter xmlOut(&collectedresults, xmlFileOut);
    xmlOut.write();

    // return 0 if tests were successful
    return collectedresults.wasSuccessful() ? 0 : 1;
}

