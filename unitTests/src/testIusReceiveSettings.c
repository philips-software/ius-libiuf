

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusReceiveSettingsPrivate.h>

TEST_GROUP(IusReceiveSettings);

TEST_SETUP(IusReceiveSettings)
{
}

TEST_TEAR_DOWN(IusReceiveSettings)
{
}


TEST(IusReceiveSettings, testIusReceiveSettingsCreate)
{
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine+10, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    iusReceiveSettingsDelete(obj);
    iusReceiveSettingsDelete(notherObj);

    // invalid params
    obj = iusReceiveSettingsCreate((float)-1.0, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusReceiveSettingsCreate(sampleFrequency, 0, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, -1, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, -1);
    TEST_ASSERT(obj == IURS_INVALID);
}

TEST(IusReceiveSettings, testIusReceiveSettingsDelete)
{
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    int status = iusReceiveSettingsDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusReceiveSettingsDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusReceiveSettings, testIusReceiveSettingsCompare)
{
    IUS_BOOL equal;
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries+1);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    equal = iusReceiveSettingsCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // Get a handle to the TGC ADT stored in the Receive settings
    // and alter it.
    iutgc_t tgc = iusReceiveSettingsGetTGC(obj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iusTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUS_E_OK);
    // So now obj has been changed...and should not B equal
    equal = iusReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    tgc = iusReceiveSettingsGetTGC(notherObj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iusTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUS_E_OK);
    // So now both notherobj and obj have been changed... equal
    equal = iusReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = iusReceiveSettingsCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusReceiveSettingsCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusReceiveSettingsCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusReceiveSettingsDelete(obj);
    iusReceiveSettingsDelete(notherObj);
}


TEST(IusReceiveSettings, testIusReceiveSettingsSetGet)
{
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int i;
    int status;

    // Constructor injected parameters
    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT_EQUAL_FLOAT(sampleFrequency,iusReceiveSettingsGetSampleFrequency(obj));
    TEST_ASSERT_EQUAL(numDelays,iusReceiveSettingsGetNumDelays(obj));
    TEST_ASSERT_EQUAL(numSamplesPerLine,iusReceiveSettingsGetNumSamplesPerLine(obj));
    TEST_ASSERT_EQUAL(numTGCentries,iusReceiveSettingsGetNumTGCentries(obj));

    // TGC
    iutgc_t tgc = iusReceiveSettingsGetTGC(obj);
    TEST_ASSERT( tgc != IUTGC_INVALID );

    // Delays
    for(i=0;i<numDelays;i++)
    {
        float delay = i*2.0f;
        status = iusReceiveSettingsSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(delay, iusReceiveSettingsGetStartDelay(obj, i));
    }
}

TEST(IusReceiveSettings, testIusSerialization)
{
    char *filename = "testIusReceiveSettings.hdf5";
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int i;
    int status;

    // Create
    iurs_t obj = iusReceiveSettingsCreate(sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);

    // Delays
    for(i=0;i<numDelays;i++)
    {
        float delay = i*3.14f;
        status = iusReceiveSettingsSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(delay, iusReceiveSettingsGetStartDelay(obj, i));
    }

    iutgc_t tgc = iusReceiveSettingsGetTGC(obj);
    for (i=0;i<numTGCentries;i++)
    {
        status = iusTGCSet(tgc,i,i*1.314f,i*2.314f);
        TEST_ASSERT_EQUAL(IUS_E_OK,status);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusReceiveSettingsSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iurs_t savedObj = iusReceiveSettingsLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusReceiveSettingsCompare(obj, savedObj));
    iusReceiveSettingsDelete(obj);
    iusReceiveSettingsDelete(savedObj);
}

TEST_GROUP_RUNNER(IusReceiveSettings)
{

    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsCreate);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsDelete);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsCompare);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsSetGet);
    RUN_TEST_CASE(IusReceiveSettings, testIusSerialization);
}
