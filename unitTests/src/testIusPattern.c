

//
// Created by nlv09165 on 13/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusPatternPrivate.h"


static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";

TEST_GROUP(IusPattern);

TEST_SETUP(IusPattern)
{
}

TEST_TEAR_DOWN(IusPattern)
{
}


TEST(IusPattern, testIusPatternCreate)
{
    iupa_t obj = iusPatternCreate( 0.01f,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pReceivesettingsLabel);

    iupa_t notherObj = iusPatternCreate( 0.01f,
                                         pPulseLabel,
                                         pSourceLabel,
                                         pChannelMapLabel,
                                         pApodizationLabel,
                                         pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    TEST_ASSERT(notherObj != IUPA_INVALID);
    iusPatternDelete(obj);
    iusPatternDelete(notherObj);

    // invalid params
    obj = iusPatternCreate(  0.01f,
                             "",
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusPatternCreate(  0.01f,
                             pPulseLabel,
                             "",
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             "",
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             "",
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusPatternCreate(  0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                               "");
    TEST_ASSERT(obj == IUPA_INVALID);

}

TEST(IusPattern, testIusPatternDelete)
{
    iupa_t obj = iusPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    int status = iusPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


int createAndCompare
(
    iupa_t reference,
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
)
{
  iupa_t actual = iusPatternCreate( timeInFrame,
                                    pPulseLabel,
                                    pSourceLabel,
                                    pChannelMapLabel,
                                    pApodizationLabel,
                                    pReceiveSettingsLabel);
  if(actual == IUPA_INVALID) return IUS_ERR_VALUE;
  int equal = iusPatternCompare(reference,actual);
  iusPatternDelete(actual);
  return equal;
}

TEST(IusPattern, testIusPatternCompare)
{
    IUS_BOOL equal;
    iupa_t obj = iusPatternCreate(0.01f,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
#define CREATE_AND_COMPARE(e,a,o,t,pul,sl,cml,al,rl) a = createAndCompare(o, t, pul,sl,cml,al,rl); \
  TEST_ASSERT_EQUAL(e,a)


    CREATE_AND_COMPARE(IUS_TRUE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.02f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                      "MyPulseLabel", pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, "MySourceLabel", pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, "MyChannelMapLabel", pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, "MyApodizationLabel", pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, "MyReceivesettingsLabel");

    // invalid params
    equal = iusPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusPatternDelete(obj);
}


TEST(IusPattern, testIusPatternSetGet)
{
    float timeInFrame = 0.01f;
    iupa_t obj = iusPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    
    // label
    TEST_ASSERT_EQUAL_STRING(pPulseLabel, iusPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pSourceLabel, iusPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pChannelMapLabel, iusPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pApodizationLabel, iusPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pReceivesettingsLabel, iusPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(timeInFrame, iusPatternGetTimeInFrame(obj) );


    // invalid param
    iusPatternDelete(obj);
    obj = IUPA_INVALID;
    TEST_ASSERT_EQUAL(NULL, iusPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusPatternGetTimeInFrame(obj) );

    iusPatternDelete(obj);
}

TEST(IusPattern, testIusSerialization)
{
    char *filename = "testIusPatternSerialization.hdf5";

    // create and save
    float timeInFrame = 0.01f;
    iupa_t obj = iusPatternCreate(timeInFrame,
                                  pPulseLabel,
                                  pSourceLabel,
                                  pChannelMapLabel,
                                  pApodizationLabel,
                                  pReceivesettingsLabel);

    iupa_t notherObj = iusPatternCreate(0.02f,
                                        pPulseLabel,
                                        pSourceLabel,
                                        pChannelMapLabel,
                                        pApodizationLabel,
                                        pReceivesettingsLabel);
    // fill
    TEST_ASSERT(obj != IUPA_INVALID);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusPatternSave( obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupa_t savedObj = iusPatternLoad(handle);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusPatternCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusPatternCompare(notherObj,savedObj));
    iusPatternDelete((iupa_t)obj);
    iusPatternDelete(savedObj);
}

TEST_GROUP_RUNNER(IusPattern)
{
    RUN_TEST_CASE(IusPattern, testIusPatternCreate);
    RUN_TEST_CASE(IusPattern, testIusPatternDelete);
    RUN_TEST_CASE(IusPattern, testIusPatternCompare);
    RUN_TEST_CASE(IusPattern, testIusPatternSetGet);
    RUN_TEST_CASE(IusPattern, testIusSerialization);
}
