//
// Created by nlv09165 on 15/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iusFrame.h>
#include <iusFrameListPrivate.h>
#include <ius.h>

TEST_GROUP(IusFrameList);

TEST_SETUP(IusFrameList)
{
}

TEST_TEAR_DOWN(IusFrameList)
{
}

TEST(IusFrameList, testIusCreateFrameList)
{
  // Put your test code here
  int numFrames = 100;
  iufl_t patternList = iusFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, patternList);
  TEST_ASSERT_EQUAL(numFrames, iusFrameListGetSize(patternList));

  patternList = iusFrameListCreate(-1);
  TEST_ASSERT_EQUAL(IUFL_INVALID, patternList);
  iusFrameListDelete(patternList);
}

TEST(IusFrameList, testIusCompareFrameList)
{
  // Put your test code here
  int numFrames = 100;
  int status;
  IUS_BOOL equal;

  // Empty lists should be equal
  iufl_t frameList = iusFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);
  iufl_t notherFrameList = iusFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, notherFrameList);
  equal = iusFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iufr_t obj = iusFrameCreate("test",2,0.01f);
  iufr_t notherObj = iusFrameCreate("test",4,0.11f);


  // Change one list..add bmode
  status = iusFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusFrameListSet(notherFrameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);


  // Change one list..add doppler
  status = iusFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusFrameListSet(notherFrameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iusFrameListDelete(frameList);
  iusFrameDelete(obj);
  iusFrameDelete(notherObj);
}

TEST(IusFrameList, testIusSerialization)
{
  int numFrames = 2;
  int status;
  IUS_BOOL equal;
  char *pFilename = "testIusFrameListSerialization.hdf5";
  //char *pFrameListPath = "/FrameList";

  // fill list
  iufl_t frameList = iusFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);

  iufr_t obj = iusFrameCreate("test",2,0.01f);
  iufr_t notherObj = iusFrameCreate("test",4,0.11f);


  // Change one list..add bmode
  status = iusFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = iusFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);
  status = iusFrameListSave(frameList, handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // read back
  handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
  iufl_t savedFrameList = iusFrameListLoad(handle);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, savedFrameList);
  status |= H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // compare
  equal = iusFrameListCompare(frameList, savedFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  // TODO: Non-happy flow
  // for lists it should not be posible to save, if not all elements have been
  // filled.
  iusFrameListDelete(frameList);
  iusFrameDelete(obj);
  iusFrameDelete(notherObj);
}

TEST_GROUP_RUNNER(IusFrameList)
{
  RUN_TEST_CASE(IusFrameList, testIusCreateFrameList);
  RUN_TEST_CASE(IusFrameList, testIusCompareFrameList);
  RUN_TEST_CASE(IusFrameList, testIusSerialization);
}
