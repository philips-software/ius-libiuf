//
// Created by nlv09165 on 15/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iusHLFrame.h>
#include <iusHLFrameListImp.h>
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
  iufl_t patternList = iusHLFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, patternList);
  TEST_ASSERT_EQUAL(numFrames, iusHLFrameListGetSize(patternList));

  patternList = iusHLFrameListCreate(-1);
  TEST_ASSERT_EQUAL(IUFL_INVALID, patternList);
  iusHLFrameListDelete(patternList);
}

TEST(IusFrameList, testIusCompareFrameList)
{
  // Put your test code here
  int numFrames = 100;
  int status;
  IUS_BOOL equal;

  // Empty lists should be equal
  iufl_t frameList = iusHLFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);
  iufl_t notherFrameList = iusHLFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, notherFrameList);
  equal = iusHLFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iuf_t obj = iusHLFrameCreate(1,2,0.01f);
  iuf_t notherObj = iusHLFrameCreate(3,4,0.11f);


  // Change one list..add bmode
  status = iusHLFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusHLFrameListSet(notherFrameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);


  // Change one list..add doppler
  status = iusHLFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusHLFrameListSet(notherFrameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHLFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iusHLFrameListDelete(frameList);
  iusHLFrameDelete(obj);
  iusHLFrameDelete(notherObj);
}

TEST(IusFrameList, testIusSerialization)
{
  int numFrames = 2;
  int status;
  IUS_BOOL equal;
  char *pFilename = "testIusFrameListSerialization.hdf5";
  char *pFrameListPath = "/FrameList";

  // fill list
  iufl_t frameList = iusHLFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);

  iuf_t obj = iusHLFrameCreate(1,2,0.01f);
  iuf_t notherObj = iusHLFrameCreate(3,4,0.11f);


  // Change one list..add bmode
  status = iusHLFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = iusHLFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);
  status = iusHLFrameListSave(frameList, pFrameListPath, handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // read back
  handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
  iufl_t savedFrameList = iusHLFrameListLoad(handle, pFrameListPath);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, savedFrameList);
  status |= H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // compare
  equal = iusHLFrameListCompare(frameList, savedFrameList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iusHLFrameListDelete(frameList);
  iusHLFrameDelete(obj);
  iusHLFrameDelete(notherObj);
}

TEST_GROUP_RUNNER(IusFrameList)
{
  RUN_TEST_CASE(IusFrameList, testIusCreateFrameList);
  RUN_TEST_CASE(IusFrameList, testIusCompareFrameList);
  RUN_TEST_CASE(IusFrameList, testIusSerialization);
}
