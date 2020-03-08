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

class TestShamean : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestShamean);
    CPPUNIT_TEST(testChecksumFile);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testChecksumFile(void);

};

//-----------------------------------------------------------------------------

void
TestShamean::testChecksumFile(void)
{

    // Create test file
    char test_filename[] = "test_file";
    std::ofstream test_file(test_filename);
    test_file << "aaaaaaaa";
    test_file.close();

    // Create variables to pass to checksum_file
    unsigned char checksum[20];
    char out_checksum[40];
    bool opened_ok = false;

    checksum_file(test_filename, checksum, opened_ok);
    convert_to_hex(checksum, out_checksum);

    std::cout << out_checksum;
    CPPUNIT_ASSERT(strcmp(out_checksum, "D35CD2A09CB225209679C56B42C60D9440A3B8BB") == 0);
    std::remove(test_filename);
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

