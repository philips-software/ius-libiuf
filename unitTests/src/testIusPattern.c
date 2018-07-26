

//
// Created by nlv09165 on 13/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include "include/iusHLPatternImp.h"


static const char *pBmodePatternLabel = "bmode";
static const char *pDopplerPatternLabel = "doppler";
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
    iupa_t obj = iusHLPatternCreate(pBmodePatternLabel,
                                   0.01f,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pReceivesettingsLabel);

    iupa_t notherObj = iusHLPatternCreate(pDopplerPatternLabel,
                                         0.01f,
                                         pPulseLabel,
                                         pSourceLabel,
                                         pChannelMapLabel,
                                         pApodizationLabel,
                                         pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    TEST_ASSERT(notherObj != IUPA_INVALID);
    iusHLPatternDelete(obj);
    iusHLPatternDelete(notherObj);

    // invalid params
    obj = iusHLPatternCreate(NULL,
                             0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate("",
                             0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate(pBmodePatternLabel,
                             0.01f,
                             "",
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate(pBmodePatternLabel,
                             0.01f,
                             pPulseLabel,
                             "",
                             pChannelMapLabel,
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate(pBmodePatternLabel,
                             0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             "",
                             pApodizationLabel,
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate(pBmodePatternLabel,
                             0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             "",
                             pReceivesettingsLabel);
    TEST_ASSERT(obj == IUPA_INVALID);

    obj = iusHLPatternCreate(pBmodePatternLabel,
                             0.01f,
                             pPulseLabel,
                             pSourceLabel,
                             pChannelMapLabel,
                             pApodizationLabel,
                               "");
    TEST_ASSERT(obj == IUPA_INVALID);

}

TEST(IusPattern, testIusPatternDelete)
{
    iupa_t obj = iusHLPatternCreate(pBmodePatternLabel,
                                 0.01f,
                                 pPulseLabel,
                                 pSourceLabel,
                                 pChannelMapLabel,
                                 pApodizationLabel,
                                 pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    int status = iusHLPatternDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPatternDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}


int createAndCompare
(
    iupa_t reference,
    const char *pLabel,
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
)
{
  iupa_t actual = iusHLPatternCreate(pLabel,
                                    timeInFrame,
                                    pPulseLabel,
                                    pSourceLabel,
                                    pChannelMapLabel,
                                    pApodizationLabel,
                                    pReceiveSettingsLabel);
  if(actual == IUPA_INVALID) return IUS_ERR_VALUE;
  int equal = iusHLPatternCompare(reference,actual);
  iusHLPatternDelete(actual);
  return equal;
}

TEST(IusPattern, testIusPatternCompare)
{
    IUS_BOOL equal;
    iupa_t obj = iusHLPatternCreate(pBmodePatternLabel,
                                 0.01f,
                                 pPulseLabel,
                                 pSourceLabel,
                                 pChannelMapLabel,
                                 pApodizationLabel,
                                 pReceivesettingsLabel);
#define CREATE_AND_COMPARE(e,a,o,pl,t,pul,sl,cml,al,rl) a = createAndCompare(o, pl, t, pul,sl,cml,al,rl); \
  TEST_ASSERT_EQUAL(e,a)


    CREATE_AND_COMPARE(IUS_TRUE,equal,obj,pBmodePatternLabel, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pDopplerPatternLabel, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.02f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.01f,
                      "MyPulseLabel", pSourceLabel, pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.01f,
                       pPulseLabel, "MySourceLabel", pChannelMapLabel, pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.01f,
                       pPulseLabel, pSourceLabel, "MyChannelMapLabel", pApodizationLabel, pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, "MyApodizationLabel", pReceivesettingsLabel);
    CREATE_AND_COMPARE(IUS_FALSE,equal,obj,pBmodePatternLabel, 0.01f,
                       pPulseLabel, pSourceLabel, pChannelMapLabel, pApodizationLabel, "MyReceivesettingsLabel");



    // invalid params
    equal = iusHLPatternCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPatternCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusHLPatternDelete(obj);
}


TEST(IusPattern, testIusPatternSetGet)
{
    float timeInFrame = 0.01f;
    iupa_t obj = iusHLPatternCreate(pBmodePatternLabel,
                                   timeInFrame,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pReceivesettingsLabel);
    TEST_ASSERT(obj != IUPA_INVALID);
    
    // label
    TEST_ASSERT_EQUAL_STRING(pBmodePatternLabel, iusHLPatternGetLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pPulseLabel, iusHLPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pSourceLabel, iusHLPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pChannelMapLabel, iusHLPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pApodizationLabel, iusHLPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL_STRING(pReceivesettingsLabel, iusHLPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(timeInFrame, iusHLPatternGetTimeInFrame(obj) );


    // invalid param
    iusHLPatternDelete(obj);
    obj = IUPA_INVALID;
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetPulseLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetSourceLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetChannelMapLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetApodizationLabel(obj));
    TEST_ASSERT_EQUAL(NULL, iusHLPatternGetReceivesettingsLabel(obj));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iusHLPatternGetTimeInFrame(obj) );

    iusHLPatternDelete(obj);
}

TEST(IusPattern, testIusSerialization)
{
    char *filename = "testIusPatternSerialization.hdf5";
    char *patternPath =  "/Pattern";



    // create and save
    float timeInFrame = 0.01f;
    iupa_t obj = iusHLPatternCreate(pBmodePatternLabel,
                                   timeInFrame,
                                   pPulseLabel,
                                   pSourceLabel,
                                   pChannelMapLabel,
                                   pApodizationLabel,
                                   pReceivesettingsLabel);

    iupa_t notherObj = iusHLPatternCreate(pDopplerPatternLabel,
                                          0.01f,
                                          pPulseLabel,
                                          pSourceLabel,
                                          pChannelMapLabel,
                                          pApodizationLabel,
                                          pReceivesettingsLabel);
    // fill
    TEST_ASSERT(obj != IUPA_INVALID);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusHLPatternSave( obj, patternPath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupa_t savedObj = iusHLPatternLoad(handle, patternPath);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLPatternCompare(obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusHLPatternCompare(notherObj,savedObj));
    iusHLPatternDelete((iupa_t)obj);
    iusHLPatternDelete(savedObj);
}

TEST_GROUP_RUNNER(IusPattern)
{
    RUN_TEST_CASE(IusPattern, testIusPatternCreate);
    RUN_TEST_CASE(IusPattern, testIusPatternDelete);
    RUN_TEST_CASE(IusPattern, testIusPatternCompare);
    RUN_TEST_CASE(IusPattern, testIusPatternSetGet);
    RUN_TEST_CASE(IusPattern, testIusSerialization);

}
