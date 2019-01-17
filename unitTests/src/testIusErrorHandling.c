//
// Created by Ruijzendaal on 26/11/2018.
//

#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <memory.h>

#include "ius.h"
static char *pTestFileName = "nonexistingfilename.hdf5";
static char *pErrorFilename = "nonexistingfilename.errlog";

TEST_GROUP(IusErrorHandling);

TEST_SETUP(IusErrorHandling)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
}

TEST_TEAR_DOWN(IusErrorHandling)
{
    remove(pTestFileName);
    remove(pErrorFilename);
    iusErrorSetStream(stderr);
}


TEST(IusErrorHandling, testIusErrorManualReportScenario)
{
    iusErrorAutoReport(IUS_FALSE);

    // Force error condition
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);

    // Assert error stack is filled
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());



    // Validate some error has been reported by
    // opening an errorlogfile and
    // checking whether file position has been
    // changed.
    FILE *fp = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fp);
    long filePos = ftell(fp);
    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_EQUAL(filePos,ftell(fp));

    int status = iusErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));

    iusErrorLogClear();
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}



TEST(IusErrorHandling, testIusErrorUsageScenarioStderr)
{
    // Force error condition
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());

    int status = iusErrorPrint(stderr);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    iusErrorLogClear();
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    iusFileDelete(inputFile);
}


TEST(IusErrorHandling, testIusErrorLogDisabledEnabledScenario)
{
    // Test:
    // After disabling error logging, we want to verify
    // nothing has been reported to the error FILE stream
    iusErrorLog(IUS_FALSE);
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());


    FILE *fp = fopen(pErrorFilename, "w+");
    long filePos = ftell(fp);
    int status = iusErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(filePos,ftell(fp));

    iusErrorLogClear();
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    // Enable logging again
    iusErrorLog(IUS_TRUE);
    inputFile = iusFileLoad(pTestFileName);
    status = iusErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));

    iusErrorLogClear();
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}


TEST(IusErrorHandling, testIusErrorAutoReport)
{
    iusErrorAutoReport(IUS_TRUE);
    FILE *fp = fopen(pErrorFilename, "w+");
    long filePos = ftell(fp);
    iusErrorSetStream(fp);
    iuf_t inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    // Ok, let's try again, with auto-reporting disabled
    iusErrorAutoReport(IUS_FALSE);
    filePos = ftell(fp);
    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}


TEST_GROUP_RUNNER(IusErrorHandling)
{
    RUN_TEST_CASE(IusErrorHandling, testIusErrorUsageScenarioStderr);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorLogDisabledEnabledScenario);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorManualReportScenario);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorAutoReport);
}
