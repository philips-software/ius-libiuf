

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLTGC.h>
#include "include/iusHLReceiveSettings.h"

TEST_GROUP(IusReceiveSettings);

TEST_SETUP(IusReceiveSettings)
{
}

TEST_TEAR_DOWN(IusReceiveSettings)
{
}


TEST(IusReceiveSettings, testIusReceiveSettingsCreate)
{
    char *pLabel = "Label for IusReceiveSettings, created in testIusReceiveSettingsCreate";
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    iusHLReceiveSettingsDelete(obj);
    iusHLReceiveSettingsDelete(notherObj);

    // invalid params
    obj = iusHLReceiveSettingsCreate(NULL, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusHLReceiveSettingsCreate(pLabel, -1.0, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, 0, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, -1, numTGCentries);
    TEST_ASSERT(obj == IURS_INVALID);
    obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, -1);
    TEST_ASSERT(obj == IURS_INVALID);
}

TEST(IusReceiveSettings, testIusReceiveSettingsDelete)
{
    char *pLabel = "Label for IusReceiveSettings, created in testIusReceiveSettingsCreate";
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    iurs_t obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    TEST_ASSERT(obj != IURS_INVALID);
    int status = iusHLReceiveSettingsDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLReceiveSettingsDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusReceiveSettings, testIusReceiveSettingsCompare)
{
    IUS_BOOL equal;
    char *pLabel = "Label for IusReceiveSettings, created in testIusReceiveSettingsCompare";
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int status;

    iurs_t obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t notherObj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    iurs_t differentObj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries+1);
    TEST_ASSERT(obj != IURS_INVALID);
    TEST_ASSERT(notherObj != IURS_INVALID);
    equal = iusHLReceiveSettingsCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // Get a handle to the TGC ADT stored in the Receive settings
    // and alter it.
    iutgc_t tgc = iusHLReceiveSettingsGetTGC(obj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iusHLTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUS_E_OK);
    // So now obj has been changed...and should not B equal
    equal = iusHLReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    tgc = iusHLReceiveSettingsGetTGC(notherObj);
    TEST_ASSERT(tgc != IUTGC_INVALID);
    status = iusHLTGCSet(tgc,0,1.0f,.0f);
    TEST_ASSERT(status == IUS_E_OK);
    // So now both notherobj and obj have been changed... equal
    equal = iusHLReceiveSettingsCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    equal = iusHLReceiveSettingsCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHLReceiveSettingsCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLReceiveSettingsCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLReceiveSettingsDelete(obj);
    iusHLReceiveSettingsDelete(notherObj);
}


TEST(IusReceiveSettings, testIusReceiveSettingsSetGet)
{

    char *pLabel = "Label for IusReceiveSettings, created in testIusReceiveSettingsSetGet";
    float sampleFrequency=4000;
    int numDelays=10;
    int numSamplesPerLine=10;
    int numTGCentries = 1;
    int i;
    int status;

    // Constructor injected parameters
    iurs_t obj = iusHLReceiveSettingsCreate(pLabel, sampleFrequency, numDelays, numSamplesPerLine, numTGCentries);
    char *pObjLabel = iusHLReceiveSettingsGetLabel(obj);
    TEST_ASSERT_EQUAL_STRING(pLabel,pObjLabel);
    TEST_ASSERT_EQUAL_FLOAT(sampleFrequency,iusHLReceiveSettingsGetSampleFrequency(obj));
    TEST_ASSERT_EQUAL(numDelays,iusHLReceiveSettingsGetNumDelays(obj));
    TEST_ASSERT_EQUAL(numSamplesPerLine,iusHLReceiveSettingsGetNumSamplesPerLine(obj));
    TEST_ASSERT_EQUAL(numTGCentries,iusHLReceiveSettingsGetNumTGCentries(obj));

    // TGC
    iutgc_t tgc = iusHLReceiveSettingsGetTGC(obj);
    TEST_ASSERT( tgc != IUTGC_INVALID );

    // Delays
    for(i=0;i<numDelays;i++)
    {
        float delay = i*2.0f;
        status = iusHLReceiveSettingsSetStartDelay(obj, i, delay);
        TEST_ASSERT(status == IUS_E_OK);
        TEST_ASSERT_EQUAL_FLOAT(delay, iusHLReceiveSettingsGetStartDelay(obj, i));
    }

}


TEST_GROUP_RUNNER(IusReceiveSettings)
{

    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsCreate);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsDelete);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsCompare);
    RUN_TEST_CASE(IusReceiveSettings, testIusReceiveSettingsSetGet);
}
