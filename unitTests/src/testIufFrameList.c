#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufFrameListPrivate.h>

static char *pErrorFilename = "IufFrameList.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufFrameList);

TEST_SETUP(IufFrameList)
{
  iufErrorLogClear();
  iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
  fpErrorLogging = fopen(pErrorFilename, "w+");
  iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufFrameList)
{
  iufErrorLogClear();
  iufErrorLog(IUF_FALSE);
  if (fpErrorLogging != NULL)
    fclose(fpErrorLogging);
  fpErrorLogging=stderr;
  iufErrorSetStream(fpErrorLogging);
  remove(pErrorFilename);
}

TEST(IufFrameList, testIufCreateFrameList)
{
  // Put your test code here
  int numFrames = 100;
  iufl_t frameList = iufFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);
  TEST_ASSERT_EQUAL(numFrames, iufFrameListGetSize(frameList));
  iufFrameListDelete(frameList);


  long filePos = ftell(fpErrorLogging);
  TEST_ASSERT_EQUAL(0,iufErrorGetCount());

  frameList = iufFrameListCreate(-1);
  TEST_ASSERT_EQUAL(IUFL_INVALID, frameList);
  frameList = iufFrameListCreate(0);
  TEST_ASSERT_EQUAL(IUFL_INVALID, frameList);
  
  TEST_ASSERT_EQUAL(2,iufErrorGetCount());
  TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
  

}

TEST(IufFrameList, testIufCompareFrameList)
{
  // Put your test code here
  int numFrames = 100;
  int status;
  IUF_BOOL equal;

  // Empty lists should be equal
  iufl_t frameList = iufFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);
  iufl_t notherFrameList = iufFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, notherFrameList);
  equal = iufFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iufr_t obj = iufFrameCreate("test",2,0.01f);
  iufr_t notherObj = iufFrameCreate("test",4,0.11f);


  // Change one list..add bmode
  status = iufFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUF_FALSE, equal);

  // Change other
  status = iufFrameListSet(notherFrameList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);


  // Change one list..add doppler
  status = iufFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUF_FALSE, equal);

  // Change other
  status = iufFrameListSet(notherFrameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);
  equal = iufFrameListCompare(frameList, notherFrameList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iufFrameListDelete(frameList);
  iufFrameListDelete(notherFrameList);
  iufFrameDelete(obj);
  iufFrameDelete(notherObj);
}


TEST(IufFrameList, testIufSerialization)
{
  int numFrames = 2;
  int status;
  IUF_BOOL equal;
  char *pFilename = "testIufFrameListSerialization.hdf5";

  // fill list
  iufl_t frameList = iufFrameListCreate(numFrames);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, frameList);

  iufr_t obj = iufFrameCreate("test",2,0.01f);
  iufr_t notherObj = iufFrameCreate("test",4,0.11f);


  // Change one list..add bmode
  status = iufFrameListSet(frameList, obj, 0);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);


  // save
  hid_t handle = H5Fcreate(pFilename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  TEST_ASSERT(handle > 0);

  status = iufFrameListSave(frameList, handle);
  TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

  status = iufFrameListSet(frameList, notherObj, 1);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  status = iufFrameListSave(frameList, handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  status = H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);


  // read back
  handle = H5Fopen(pFilename, H5F_ACC_RDONLY, H5P_DEFAULT);
  iufl_t savedFrameList = iufFrameListLoad(handle);
  TEST_ASSERT_NOT_EQUAL(IUFL_INVALID, savedFrameList);
  status |= H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUF_E_OK, status);

  // compare
  equal = iufFrameListCompare(frameList, savedFrameList);
  TEST_ASSERT_EQUAL(IUF_TRUE, equal);

  iufFrameListDelete(frameList);
  iufFrameListDelete(savedFrameList);
  iufFrameDelete(obj);
  iufFrameDelete(notherObj);
}

TEST_GROUP_RUNNER(IufFrameList)
{
  RUN_TEST_CASE(IufFrameList, testIufCreateFrameList);
  RUN_TEST_CASE(IufFrameList, testIufCompareFrameList);
  RUN_TEST_CASE(IufFrameList, testIufSerialization);
}
