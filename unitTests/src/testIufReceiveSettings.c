

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufReceiveSettingsPrivate.h>

static char *pErrorFilename = "IufReceiveSettings.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufReceiveSettings);

TEST_SETUP(IufReceiveSettings)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufReceiveSettings)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufReceiveSettings, testIufReceiveSettingsCreate)
{
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine + 10, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iufReceiveSettingsCreate((float) -1.0, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iufReceiveSettingsCreate(sampleFrequency, -1, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, -1);
    TEST_ASSERT(obj == IURS_INVALID);

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufReceiveSettings, testIufReceiveSettingsDelete)
{
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    int status = iufReceiveSettingsDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufReceiveSettingsDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufReceiveSettings, testIufReceiveSettingsCompare)
{
    IUF_BOOL equal;
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries + 1);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    equal = iufReceiveSettingsCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    // Get a handle to the TGC ADT stored in the Receive settings
    // and alter it.
    iutgc_t tgc = iufReceiveSettingsGetTGC(obj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iufTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUF_E_OK);
    // So now obj has been changed...and should not B equal
    equal = iufReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    tgc = iufReceiveSettingsGetTGC(notherObj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iufTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUF_E_OK);
    // So now both notherobj and obj have been changed... equal
    equal = iufReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    equal = iufReceiveSettingsCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    // invalid params
    equal = iufReceiveSettingsCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufReceiveSettingsCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDelete(notherObj);
    iufReceiveSettingsDelete(differentObj);
}


TEST(IufReceiveSettings, testIufReceiveSettingsSetGet)
{
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 1;

    // Constructor injected parameters
    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);
    TEST_ASSERT_EQUAL_FLOAT(sampleFrequency,iufReceiveSettingsGetSampleFrequency(obj));
    TEST_ASSERT_EQUAL(numSamplesPerLine,iufReceiveSettingsGetNumSamplesPerLine(obj));
    TEST_ASSERT_EQUAL(numTGCentries,iufReceiveSettingsGetNumTGCentries(obj));

    // TGC
    iutgc_t tgc = iufReceiveSettingsGetTGC(obj);
    TEST_ASSERT( tgc != IUTGC_INVALID );

    iufReceiveSettingsDelete(obj);
}

TEST(IufReceiveSettings, testIufSerialization)
{
    char *filename = "testIufReceiveSettings.hdf5";
    float sampleFrequency=4000;
    int numSamplesPerLine=10;
    int numTGCentries = 2;
    int i;
    int status;

    // Create
    iurs_t obj = iufReceiveSettingsCreate(sampleFrequency, numSamplesPerLine, numTGCentries);

    iutgc_t tgc = iufReceiveSettingsGetTGC(obj);
    for (i=0;i<numTGCentries;i++)
    {
        status = iufTGCSet(tgc,i,i*1.314f,i*2.314f);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufReceiveSettingsSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iurs_t savedObj = iufReceiveSettingsLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufReceiveSettingsCompare(obj, savedObj));
    iufReceiveSettingsDelete(obj);
    iufReceiveSettingsDelete(savedObj);
}

TEST_GROUP_RUNNER(IufReceiveSettings)
{
    RUN_TEST_CASE(IufReceiveSettings, testIufReceiveSettingsCreate);
    RUN_TEST_CASE(IufReceiveSettings, testIufReceiveSettingsDelete);
    RUN_TEST_CASE(IufReceiveSettings, testIufReceiveSettingsCompare);
    RUN_TEST_CASE(IufReceiveSettings, testIufReceiveSettingsSetGet);
    RUN_TEST_CASE(IufReceiveSettings, testIufSerialization);
}
