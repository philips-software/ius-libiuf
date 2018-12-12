//
// Created by Ruijzendaal on 26/11/2018.
//

#include <util.h>
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <memory.h>

#include "ius.h"

TEST_GROUP(IusErrorHandling);

TEST_SETUP(IusErrorHandling)
{
}

TEST_TEAR_DOWN(IusErrorHandling)
{
}


TEST(IusErrorHandling, testIusErrorUsageScenario)
{
    // Force error condition
    char *pTestFileName = "nonexistingfilename.hdf5";
    char *pErrprFileName = "nonexistingfilename.errlog";
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);

    // Assert error stack is filled
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());

    // Validate some error has been reported by
    // opening an errorlogfile and
    // checking whether file position has been
    // changed.
    FILE *fp = fopen(pErrprFileName, "w+");
    long filePos = ftell(fp);
    int status = iusErrorPrint(fp);
    remove(pTestFileName);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}




TEST(IusErrorHandling, testIusErrorUsageScenarioStderr)
{
    // Force error condition
    char *pTestFileName = "nonexistingfilename.hdf5";
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());

    FILE *fp = stderr;
    long filePos = ftell(fp);
    int status = iusErrorPrint(fp);
    remove(pTestFileName);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}


TEST(IusErrorHandling, testIusErrorLogDisabledScenario)
{
    // Test:
    // After disabling error logging, we want to verify
    // nothing has been reported to the error FILE stream
    iusErrorLogDisable();
    char *pTestFileName = "nonexistingfilename.hdf5";
    int numIusErrors = iusErrorGetCount();
    TEST_ASSERT_EQUAL(0,numIusErrors);

    iuf_t inputFile = iusFileLoad(pTestFileName);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(1,iusErrorGetCount());

    inputFile = iusFileLoad(NULL);
    TEST_ASSERT(inputFile == IUFI_INVALID);
    TEST_ASSERT_EQUAL(2,iusErrorGetCount());


    FILE *fp = stderr;
    long filePos = ftell(fp);
    int status = iusErrorPrint(fp);
    remove(pTestFileName);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL(filePos,ftell(fp));
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());
    fclose(fp);
    iusFileDelete(inputFile);
}

TEST_GROUP_RUNNER(IusErrorHandling)
{
//    RUN_TEST_CASE(IusErrorHandling, testIusErrorUsageScenarioStderr);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorUsageScenario);
    RUN_TEST_CASE(IusErrorHandling, testIusErrorLogDisabledScenario);
}
