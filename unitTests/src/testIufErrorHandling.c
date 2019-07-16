#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <memory.h>

#include "iuf.h"
static char *pTestFileName = "nonexistingfilename.hdf5";
static char *pErrorFilename = "nonexistingfilename.errlog";

TEST_GROUP(IufErrorHandling);

TEST_SETUP(IufErrorHandling)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
}

TEST_TEAR_DOWN(IufErrorHandling)
{
    remove(pTestFileName);
    remove(pErrorFilename);
    iufErrorSetStream(stderr);
}


TEST(IufErrorHandling, testIufErrorManualReportScenario)
{
    iufErrorAutoReportSet(IUF_FALSE);

    // Force error condition
    int numIufErrors = iufErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIufErrors);

    iuf_t inputFile = iufFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);

    // Assert error stack is filled
    TEST_ASSERT_EQUAL(1,iufErrorGetCount());



    // Validate some error has been reported by
    // opening an errorlogfile and
    // checking whether file position has been
    // changed.
    FILE *fp = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fp);
    long filePos = ftell(fp);
    inputFile = iufFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_EQUAL(filePos,ftell(fp));

    int status = iufErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));

    iufErrorLogClear();
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    fclose(fp);
    iufFileDelete(inputFile);
}



TEST(IufErrorHandling, testIufErrorUsageScenarioStderr)
{
    // Force error condition
    int numIufErrors = iufErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIufErrors);

    iuf_t inputFile = iufFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iufErrorGetCount());

    inputFile = iufFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iufErrorGetCount());

    int status = iufErrorPrint(stderr);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    iufErrorLogClear();
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    iufFileDelete(inputFile);
}


TEST(IufErrorHandling, testIufErrorLogDisabledEnabledScenario)
{
    // Test:
    // After disabling error logging, we want to verify
    // nothing has been reported to the error FILE stream
    iufErrorLog(IUF_FALSE);
    int numIufErrors = iufErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIufErrors);

    iuf_t inputFile = iufFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iufErrorGetCount());

    inputFile = iufFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iufErrorGetCount());


    FILE *fp = fopen(pErrorFilename, "w+");
    long filePos = ftell(fp);
    int status = iufErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL(filePos,ftell(fp));

    iufErrorLogClear();
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    // Enable logging again
    iufErrorLog(IUF_TRUE);
    inputFile = iufFileLoad(pTestFileName);
    status = iufErrorPrint(fp);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));

    iufErrorLogClear();
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());
    fclose(fp);
    iufFileDelete(inputFile);
}


TEST(IufErrorHandling, testIufErrorAutoReport)
{
    iufErrorAutoReportSet(IUF_TRUE);
    FILE *fp = fopen(pErrorFilename, "w+");
    long filePos = ftell(fp);
    iufErrorSetStream(fp);
    iuf_t inputFile = iufFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(1,iufErrorGetCount());

    // Ok, let's try again, with auto-reporting disabled
    iufErrorAutoReportSet(IUF_FALSE);
    filePos = ftell(fp);
    inputFile = iufFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    fclose(fp);
    iufFileDelete(inputFile);
}


TEST_GROUP_RUNNER(IufErrorHandling)
{
    RUN_TEST_CASE(IufErrorHandling, testIufErrorUsageScenarioStderr);
    RUN_TEST_CASE(IufErrorHandling, testIufErrorLogDisabledEnabledScenario);
    RUN_TEST_CASE(IufErrorHandling, testIufErrorManualReportScenario);
    RUN_TEST_CASE(IufErrorHandling, testIufErrorAutoReport);
}
