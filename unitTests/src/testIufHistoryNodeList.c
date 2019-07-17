#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufHistoryNodeListPrivate.h>


static char *pErrorFilename = "IufHistoryNodeList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufHistoryNodeList);

TEST_SETUP(IufHistoryNodeList)
{
  iufErrorLogClear();
  iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
  fpErrorLogging = fopen(pErrorFilename, "w+");
  iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufHistoryNodeList)
{
  iufErrorLogClear();
  iufErrorLog(IUF_FALSE);
  if (fpErrorLogging != NULL)
    fclose(fpErrorLogging);
  fpErrorLogging=stderr;
  iufErrorSetStream(fpErrorLogging);
  remove(pErrorFilename);
}

TEST(IufHistoryNodeList, testIufCreateHistoryNodeList)
{
  // Put your test code here
  int numHistoryNodes = 100;
  iuhnl_t nodeList = iufHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);
  TEST_ASSERT_EQUAL(numHistoryNodes, iufHistoryNodeListGetSize(nodeList));
  iufHistoryNodeListDelete(nodeList);

  long filePos = ftell(fpErrorLogging);
  TEST_ASSERT_EQUAL(0,iufErrorGetCount());

  nodeList = iufHistoryNodeListCreate(-1);
  TEST_ASSERT_EQUAL(IUHNL_INVALID, nodeList);
  nodeList = iufHistoryNodeListCreate(0);
  TEST_ASSERT_EQUAL(IUHNL_INVALID, nodeList);

  TEST_ASSERT_EQUAL(2,iufErrorGetCount());
  TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(IufHistoryNodeList, testIufCompareHistoryNodeList)
{
  // Put your test code here
  int numHistoryNodes = 100;
  int status;
  IUF_BOOL equal;

  // Empty lists should be equal
  iuhnl_t nodeList = iufHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);
  iuhnl_t notherHistoryNodeList = iufHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, notherHistoryNodeList);
  equal = iufHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iuhn_t obj = iufHistoryNodeCreate("TYPE1");
  iuhn_t notherObj = iufHistoryNodeCreate("TYPE2");

  // Change one list..add bmode
  status = iufHistoryNodeListSet(nodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_FALSE, equal);

  // Change other
  status = iufHistoryNodeListSet(notherHistoryNodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);


  // Change one list..add doppler
  status = iufHistoryNodeListSet(nodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_FALSE, equal);

  // Change other
  status = iufHistoryNodeListSet(notherHistoryNodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufHistoryNodeListCompare(nodeList, notherHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iufHistoryNodeDelete(obj);
  iufHistoryNodeDelete(notherObj);
  iufHistoryNodeListDelete(nodeList);
  iufHistoryNodeListDelete(notherHistoryNodeList);
}

TEST(IufHistoryNodeList, testIufSerialization)
{
  int numHistoryNodes = 2;
  int status;
  IUF_BOOL equal;
  char *pFilename = "testIufHistoryNodeListSerialization.hdf5";

  // fill list
  iuhnl_t nodeList = iufHistoryNodeListCreate(numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, nodeList);

  iuhn_t obj = iufHistoryNodeCreate("TYPE1");
  iuhn_t notherObj = iufHistoryNodeCreate("TYPE2");


  // Change one list..add bmode
  status = iufHistoryNodeListSet(nodeList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);

  // Not all list elements have been configured => error.
  status = iufHistoryNodeListSave(nodeList, handle);
  TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

  status = iufHistoryNodeListSet(nodeList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  status = iufHistoryNodeListSave(nodeList, handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  status = H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  // read back
  handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
  iuhnl_t savedHistoryNodeList = iufHistoryNodeListLoad(handle, numHistoryNodes);
  TEST_ASSERT_NOT_EQUAL(IUHNL_INVALID, savedHistoryNodeList);
  status |= H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  // compare
  equal = iufHistoryNodeListCompare(nodeList, savedHistoryNodeList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iufHistoryNodeDelete(obj);
  iufHistoryNodeDelete(notherObj);
  iufHistoryNodeListDelete(nodeList);
  iufHistoryNodeListDelete(savedHistoryNodeList);
}

TEST_GROUP_RUNNER(IufHistoryNodeList)
{
  RUN_TEST_CASE(IufHistoryNodeList, testIufCreateHistoryNodeList);
  RUN_TEST_CASE(IufHistoryNodeList, testIufCompareHistoryNodeList);
  RUN_TEST_CASE(IufHistoryNodeList, testIufSerialization);
}
