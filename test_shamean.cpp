/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <string>
#include <list>
#include <unistd.h>


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
    void testChecksum(s_test_data &test_data);
};


//-----------------------------------------------------------------------------
void TestShamean::testChecksumFile(void)
{
    // Test zero size file and boundary test
    s_test_data test_data_z1 = {"3C97D0B9804A1AA8E22EAEDD8C31B1D06D7A30DF", 'a', 0};
    TestShamean::testChecksum(test_data_z1);
    s_test_data test_data_z2 = {"FC82ACBEB9A0191373318AB6495D7A1FAAC56310", 'a', 1};
    TestShamean::testChecksum(test_data_z2);
 

    // Test small file
    s_test_data test_data_sf1 = {"A50C171BDD4603CCCBA7B56C093C1A3F023944B3", 'a', 8};
    TestShamean::testChecksum(test_data_sf1);

    // Boundary test first boundary 
    s_test_data test_data_fb0 = {"EE086E808FC9DB1A0409B5AAEDC0536A68551BDE", 'a', 204799};
    TestShamean::testChecksum(test_data_fb0);
    s_test_data test_data_fb1 = {"0A691FA5DC1656BA8C04266206F46CA1E56CB02C", 'a', 204800};
    TestShamean::testChecksum(test_data_fb1);
    s_test_data test_data_fb2 = {"21F85AF2A955A71202945BC0D27B39B19982A82E", 'a', 204801};
    TestShamean::testChecksum(test_data_fb2);


    // Boundary test second boundary
    s_test_data test_data_sb0 = {"BA7DEC711D8D867153A69CF5BC39E01B497C33D3", 'a', 409599};
    TestShamean::testChecksum(test_data_sb0);
    s_test_data test_data_sb1 = {"11BF3C0F361820E841A8439D409CFB8B7C9E99F9", 'a', 409600};
    TestShamean::testChecksum(test_data_sb1);
    s_test_data test_data_sb2 = {"629B1459AFD6D9EA88AF369CB2821403F3806400", 'a', 409601};
    TestShamean::testChecksum(test_data_sb2);

}

void TestShamean::testChecksum(s_test_data &test_data)
{
    char in_data[test_data.length + 1];
    for (long i = 0; i < test_data.length; i++)
    {
        in_data[i] = test_data.character;
    }
    in_data[test_data.length] = '\0';

    // Create options object
    s_options options;
    options.include_timestamp = false;
    strcpy(options.filename, "test_file");

    // Create test file
    std::ofstream test_file(options.filename, std::ofstream::binary | std::ofstream::trunc);
    test_file << in_data;
    test_file.close();
    sync();

    // Create variables to pass to checksum_file
    unsigned char checksum[21];
    char out_checksum[41];
    bool open_err = false;

    checksum_file(&options, checksum, open_err);
    convert_to_hex(checksum, out_checksum);
    CPPUNIT_ASSERT(strcmp(out_checksum, test_data.expected_checksum) == 0);
    CPPUNIT_ASSERT(open_err == false);
    std::remove(options.filename);
}

void TestShamean::testChecksumNonExistFile(void)
{
    unsigned char checksum[21];
    bool open_err = false;

    s_options options;
    options.include_timestamp = false;
    strcpy(options.filename, "file_does_not_exist");

    checksum_file(&options, checksum, open_err);
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

