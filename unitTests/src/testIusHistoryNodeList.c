//
// Created by nlv09165 on 13/09/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusHistoryNodeListPrivate.h>


static char *pErrorFilename = "IusHistoryNodeList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusHistoryNodeList);

TEST_SETUP(IusHistoryNodeList)
{
  iusErrorLogClear();
  iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
  fpErrorLogging = fopen(pErrorFilename, "w+");
  iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusHistoryNodeList)
{
  iusErrorLogClear();
  iusErrorLog(IUS_FALSE);
  if (fpErrorLogging != NULL)
    fclose(fpErrorLogging);
  fpErrorLogging=stderr;
  iusErrorSetStream(fpErrorLogging);
  remove(pErrorFilename);
}

TEST(IusHistoryNodeList, testIusCreateHistoryNodeList)
{
  // Put your test code here
  int numHistoryNodes = 100;
  iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);
  TEST_ASSERT_EQUAL(numHistoryNodes, iusHistoryNodeListGetSize(nodeList));
  iusHistoryNodeListDelete(nodeList);

  long filePos = ftell(fpErrorLogging);
  TEST_ASSERT_EQUAL(0,iusErrorGetCount());

  nodeList = iusHistoryNodeListCreate(-1);
  TEST_ASSERT_EQUAL(IUHNL_INVALID, nodeList);
  nodeList = iusHistoryNodeListCreate(0);
  TEST_ASSERT_EQUAL(IUHNL_INVALID, nodeList);

  TEST_ASSERT_EQUAL(2,iusErrorGetCount());
  TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
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

  iuhn_t obj = iusHistoryNodeCreate("TYPE1");
  iuhn_t notherObj = iusHistoryNodeCreate("TYPE2");

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

  iusHistoryNodeDelete(obj);
  iusHistoryNodeDelete(notherObj);
  iusHistoryNodeListDelete(nodeList);
  iusHistoryNodeListDelete(notherHistoryNodeList);
}

TEST(IusHistoryNodeList, testIusSerialization)
{
  int numHistoryNodes = 2;
  int status;
  IUS_BOOL equal;
  char *pFilename = "testIusHistoryNodeListSerialization.hdf5";

  // fill list
  iuhnl_t nodeList = iusHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);

  iuhn_t obj = iusHistoryNodeCreate("TYPE1");
  iuhn_t notherObj = iusHistoryNodeCreate("TYPE2");


  // Change one list..add bmode
  status = iusHistoryNodeListSet(nodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);

  // Not all list elements have been configured => error.
  status = iusHistoryNodeListSave(nodeList, handle);
  TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

  status = iusHistoryNodeListSet(nodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUS_E_OK, status);

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

  iusHistoryNodeDelete(obj);
  iusHistoryNodeDelete(notherObj);
  iusHistoryNodeListDelete(nodeList);
  iusHistoryNodeListDelete(savedHistoryNodeList);
}

TEST_GROUP_RUNNER(IusHistoryNodeList)
{
  RUN_TEST_CASE(IusHistoryNodeList, testIusCreateHistoryNodeList);
  RUN_TEST_CASE(IusHistoryNodeList, testIusCompareHistoryNodeList);
  RUN_TEST_CASE(IusHistoryNodeList, testIusSerialization);
}
