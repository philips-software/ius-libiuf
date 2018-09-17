//
// Created by nlv09165 on 13/09/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iusHistoryNode.h>
#include <iusHistoryNodeListImp.h>
#include <ius.h>

TEST_GROUP(IusHistoryNodeList);

TEST_SETUP(IusHistoryNodeList)
{
}

TEST_TEAR_DOWN(IusHistoryNodeList)
{
}

TEST(IusHistoryNodeList, testIusCreateHistoryNodeList)
{
  // Put your test code here
  int numHistoryNodes = 100;
  iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);
  TEST_ASSERT_EQUAL(numHistoryNodes, iusHistoryNodeListGetSize(nodeList));

  nodeList = iusHistoryNodeListCreate(-1);
  TEST_ASSERT_EQUAL(IUHNL_INVALID, nodeList);
  iusHistoryNodeListDelete(nodeList);
}

TEST(IusHistoryNodeList, testIusCompareHistoryNodeList)
{
  // Put your test code here
  int numHistoryNodes = 100;
  int status;
  IUS_BOOL equal;

  // Empty lists should be equal
  iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);
  iuhnl_t notherHistoryNodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, notherHistoryNodeList);
  equal = iusHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iuhn_t obj = iusHistoryNodeCreate("TYPE1",0);
  iuhn_t notherObj = iusHistoryNodeCreate("TYPE2",0);

  // Change one list..add bmode
  status = iusHistoryNodeListSet(nodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusHistoryNodeListSet(notherHistoryNodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);


  // Change one list..add doppler
  status = iusHistoryNodeListSet(nodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_FALSE, equal);

  // Change other
  status = iusHistoryNodeListSet(notherHistoryNodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  equal = iusHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  iusHistoryNodeListDelete(nodeList);
  iusHistoryNodeDelete(obj);
  iusHistoryNodeDelete(notherObj);
}

TEST(IusHistoryNodeList, testIusSerialization)
{
  int numHistoryNodes = 2;
  int status;
  IUS_BOOL equal;
  char *pFilename = "testIusHistoryNodeListSerialization.hdf5";
  char *pHistoryNodeListPath = "/HistoryNodeList";

  // fill list
  iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);

  iuhn_t obj = iusHistoryNodeCreate("TYPE1",0);
  iuhn_t notherObj = iusHistoryNodeCreate("TYPE2",0);


  // Change one list..add bmode
  status = iusHistoryNodeListSet(nodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = iusHistoryNodeListSet(nodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);
  status = iusHistoryNodeListSave(nodeList, handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);
  status = H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // read back
  handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
  iuhnl_t savedHistoryNodeList = iusHistoryNodeListLoad(handle, numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, savedHistoryNodeList);
  status |= H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // compare
  equal = iusHistoryNodeListCompare(nodeList, savedHistoryNodeList);
  TEST_ASSERT_EQUAL(IUS_TRUE, equal);

  // TODO: Non-happy flow
  // for lists it should not be posible to save, if not all elements have been
  // filled.
  iusHistoryNodeListDelete(nodeList);
  iusHistoryNodeDelete(obj);
  iusHistoryNodeDelete(notherObj);
}

TEST_GROUP_RUNNER(IusHistoryNodeList)
{
  RUN_TEST_CASE(IusHistoryNodeList, testIusCreateHistoryNodeList);
  RUN_TEST_CASE(IusHistoryNodeList, testIusCompareHistoryNodeList);
  RUN_TEST_CASE(IusHistoryNodeList, testIusSerialization);
}
