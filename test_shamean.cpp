/*
 * shamean
 * Copyright (C) 2020 Matt Comben matthew@dockstudios.co.uk
 */

#include <iostream>
#include <string>
#include <list>
#include <unistd.h>
#include <utime.h>


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
  long timestamp = 0;
  bool force_disable_timestamp = false;
};


//-----------------------------------------------------------------------------

class TestShamean : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestShamean);
    CPPUNIT_TEST(testChecksumFile);
    CPPUNIT_TEST(testTimestampChecksum);
    CPPUNIT_TEST(testNoneTimestampChecksum);
    CPPUNIT_TEST(testChecksumNonExistFile);
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp(void);
    void tearDown(void);

protected:
    void testChecksumFile(void);
    void testTimestampChecksum(void);
    void testChecksumNonExistFile(void);
    void testNoneTimestampChecksum(void);
    void setTimestamp(s_options* options, s_test_data* test_data);
private:
    void testChecksum(s_test_data &test_data);
};


//-----------------------------------------------------------------------------
void TestShamean::testChecksumFile(void)
{
    // Test zero size file and boundary test
    s_test_data test_data_z1 = {"7156ECD78C70FC3349EBB604C9B934018EB1CBB2", 'a', 0};
    TestShamean::testChecksum(test_data_z1);
    s_test_data test_data_z2 = {"4FB276174E7C6357851669D70C66975ED4F32E30", 'a', 1};
    TestShamean::testChecksum(test_data_z2);
 

    // Test small file
    s_test_data test_data_sf1 = {"D35CD2A09CB225209679C56B42C60D9440A3B8BB", 'a', 8};
    TestShamean::testChecksum(test_data_sf1);

    // Boundary test first boundary 
    s_test_data test_data_fb0 = {"79CBA16DF55089FB8B132A7D407DD85A9EC20835", 'a', 204799};
    TestShamean::testChecksum(test_data_fb0);
    s_test_data test_data_fb1 = {"BB7F02D1FB88F7DA7A3967370DECB72539C5A220", 'a', 204800};
    TestShamean::testChecksum(test_data_fb1);
    s_test_data test_data_fb2 = {"7FEEBBE67D6425DE4759CB0E200238DAE753946A", 'a', 204801};
    TestShamean::testChecksum(test_data_fb2);


    // Boundary test second boundary
    s_test_data test_data_sb0 = {"4164F110D6B5399E67DB9063054D388D504CCA4D", 'a', 409599};
    TestShamean::testChecksum(test_data_sb0);
    s_test_data test_data_sb1 = {"18B3E22788571B66F97D0E12309E62D57201E2C7", 'a', 409600};
    TestShamean::testChecksum(test_data_sb1);
    s_test_data test_data_sb2 = {"16DD903090ED8BE4E257E5F9BC0F54D88CC02ED8", 'a', 409601};
    TestShamean::testChecksum(test_data_sb2);

}

void TestShamean::testTimestampChecksum(void)
{
    s_test_data test_data_ts1 = {"EB5FB18E5BFA97F58403FB424F843A8BD25F61EA", 'a', 1, 1000};
    TestShamean::testChecksum(test_data_ts1);

    // Test same file with different timestamp
    s_test_data test_data_ts2 = {"8C1E723343EA0A5846F692F6024EAA5FE29939B1", 'a', 1, 5000};
    TestShamean::testChecksum(test_data_ts2);
    // Repeat test with same file and timestamp
    s_test_data test_data_ts3 = {"8C1E723343EA0A5846F692F6024EAA5FE29939B1", 'a', 1, 5000};
    TestShamean::testChecksum(test_data_ts3);
}

void TestShamean::testNoneTimestampChecksum(void)
{
    // Test file with given timestamp, but do not enable timestamp
    s_test_data test_data_ts1 = {"4FB276174E7C6357851669D70C66975ED4F32E30", 'a', 1, 1000};
    test_data_ts1.force_disable_timestamp = true;
    TestShamean::testChecksum(test_data_ts1);

    // Test same file with different timestamp
    s_test_data test_data_ts2 = {"4FB276174E7C6357851669D70C66975ED4F32E30", 'a', 1, 5000};
    test_data_ts2.force_disable_timestamp = true;
    TestShamean::testChecksum(test_data_ts2);

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

    // If timestamp has been specified, setup test for this
    if (test_data.timestamp > 0)
    {
        TestShamean::setTimestamp(&options, &test_data);
    }

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

void TestShamean::setTimestamp(s_options* options, s_test_data* test_data)
{
    struct stat stats;
    struct utimbuf new_times;
    stat(options->filename, &stats);

    new_times.actime = stats.st_atime;
    new_times.modtime = test_data->timestamp;

    utime(options->filename, &new_times);

    // Set timestamp to be used for checksum
    options->include_timestamp = ! test_data->force_disable_timestamp;
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

