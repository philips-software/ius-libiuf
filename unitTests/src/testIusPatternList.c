
//
// Created by nlv09165 on 18/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>
#include <include/iusHLPattern.h>
#include <include/iusHLPatternList.h>
#include <include/ius.h>

static const char *pBmodePatternLabel = "bmode";
static const char *pDopplerPatternLabel = "doppler";
static const char *pPulseLabel = "pulseLabel";
static const char *pSourceLabel = "sourceLabel";
static const char *pChannelMapLabel = "channelMapLabel";
static const char *pApodizationLabel = "apodizationLabel";
static const char *pReceivesettingsLabel = "receivesettingsLabel";

TEST_GROUP(IusPatternList);

TEST_SETUP(IusPatternList)
{
}

TEST_TEAR_DOWN(IusPatternList)
{
}

TEST(IusPatternList, testIusCreatePatternList)
{
    // Put your test code here
  int numPatterns = 100;
  int i, status;
  iupal_t patternList = iusHLPatternListCreate(numPatterns);
  TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID,patternList);
  TEST_ASSERT_EQUAL(numPatterns,iusHLPatternListGetSize(patternList));
  iusHLPatternListDelete(patternList);
}


TEST(IusPatternList, testIusComparePatternList)
{
  // Put your test code here
  int numPatterns = 100;
  int status;
  IUS_BOOL equal;

  // Empty lists should be equal
  iupal_t patternList = iusHLPatternListCreate(numPatterns);
  TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID,patternList);
  iupal_t notherPatternList = iusHLPatternListCreate(numPatterns);
  TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID,notherPatternList);
  equal = iusHLPatternListCompare(patternList,notherPatternList);
  TEST_ASSERT_EQUAL(IUS_TRUE,equal);


  iupa_t bmodePattern = iusHLPatternCreate(pBmodePatternLabel,
                                           0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

  iupa_t dopplerPattern = iusHLPatternCreate(pDopplerPatternLabel,
                                             0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);

  // Change one list..add bmode
  status = iusHLPatternListSet(patternList,bmodePattern,0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLPatternListCompare(patternList,notherPatternList);
  TEST_ASSERT_EQUAL(IUS_FALSE,equal);

  // Change other
  status = iusHLPatternListSet(notherPatternList,bmodePattern,0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLPatternListCompare(patternList,notherPatternList);
  TEST_ASSERT_EQUAL(IUS_TRUE,equal);


  // Change one list..add doppler
  status = iusHLPatternListSet(patternList,dopplerPattern,1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLPatternListCompare(patternList,notherPatternList);
  TEST_ASSERT_EQUAL(IUS_FALSE,equal);

  // Change other
  status = iusHLPatternListSet(notherPatternList,dopplerPattern,1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLPatternListCompare(patternList,notherPatternList);
  TEST_ASSERT_EQUAL(IUS_TRUE,equal);

  iusHLPatternListDelete(patternList);
  iusHLPatternDelete(bmodePattern);
  iusHLPatternDelete(dopplerPattern);
}

TEST(IusPatternList, testIusSerialization)
{
  int numPatterns = 100;
  int status;
  IUS_BOOL equal;

  // fill list
  iupal_t patternList = iusHLPatternListCreate(numPatterns);
  TEST_ASSERT_NOT_EQUAL(IUPAL_INVALID,patternList);

  iupa_t bmodePattern = iusHLPatternCreate(pBmodePatternLabel,
                                           0.01f,
                                           pPulseLabel,
                                           pSourceLabel,
                                           pChannelMapLabel,
                                           pApodizationLabel,
                                           pReceivesettingsLabel);

  iupa_t dopplerPattern = iusHLPatternCreate(pDopplerPatternLabel,
                                             0.01f,
                                             pPulseLabel,
                                             pSourceLabel,
                                             pChannelMapLabel,
                                             pApodizationLabel,
                                             pReceivesettingsLabel);
  status = iusHLPatternListSet(patternList,bmodePattern,0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = iusHLPatternListSet(patternList,dopplerPattern,1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // save

  // read back

  // compare

  iusHLPatternListDelete(patternList);
  iusHLPatternDelete(bmodePattern);
  iusHLPatternDelete(dopplerPattern);

}


TEST_GROUP_RUNNER(IusPatternList)
{
    RUN_TEST_CASE(IusPatternList, testIusCreatePatternList);
    RUN_TEST_CASE(IusPatternList, testIusComparePatternList);
//    RUN_TEST_CASE(IusPatternList, testIusSerialization);
}
