

//
// Created by nlv09165 on 08/08/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufTGCPrivate.h>


static char *pErrorFilename = "IufTGC.errlog";
static FILE *fpErrorLogging = NULL;


TEST_GROUP(IufTGC);

TEST_SETUP(IufTGC)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufTGC)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufTGC, testIufTGCCreate)
{
    int status = 0;
    int numTGCValues;
    iutgc_t tgc;

    numTGCValues = 20;
    tgc = iufTGCCreate(numTGCValues);
    TEST_ASSERT(tgc != IUTGC_INVALID);


    // Invalid operation on nonparametric dta type
    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iufTGCCreate(-1));

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufTGCDelete(tgc);
    TEST_ASSERT(status == IUF_E_OK);
}

TEST(IufTGC, testIufTGCDelete)
{
    int numTGCValues=2;
    iutgc_t obj = iufTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);
    int status = iufTGCDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufTGCDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufTGC, testIufTGCCompare)
{
    IUF_BOOL equal;
    int numTGCValues=2;
    iutgc_t obj = iufTGCCreate(numTGCValues);
    iutgc_t sameObj = iufTGCCreate(numTGCValues);
    iutgc_t notherObj = iufTGCCreate(numTGCValues + 10);
    TEST_ASSERT(obj != IUTGC_INVALID);
    TEST_ASSERT(notherObj != IUTGC_INVALID);

    equal = iufTGCCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufTGCCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    int status = iufTGCSet(obj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    status = iufTGCSet(sameObj,0,1.0f,2.0f);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    equal = iufTGCCompare(obj,sameObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);

    // invalid params
    equal = iufTGCCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufTGCCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iufTGCDelete(obj);
    iufTGCDelete(sameObj);
    iufTGCDelete(notherObj);
}


TEST(IufTGC, testIufTGCSetGet)
{
    int numTGCValues=2;
    iutgc_t obj = iufTGCCreate(numTGCValues);
    TEST_ASSERT(obj != IUTGC_INVALID);

    iufTGCSet(obj,0,1.0f,2.0f);
    // float param
    TEST_ASSERT_EQUAL_FLOAT(1.0f, iufTGCGetTime(obj, 0));
    TEST_ASSERT_EQUAL_FLOAT(2.0f, iufTGCGetGain(obj, 0));


    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTGCGetTime(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTGCGetGain(NULL, 1));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTGCGetTime(NULL, 0));
    TEST_ASSERT_EQUAL_FLOAT(NAN, iufTGCGetGain(NULL, 0));

    TEST_ASSERT_EQUAL(4,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iufTGCDelete(obj);
}


TEST(IufTGC, testIufSerialization)
{
    char *filename = "testIufTGCSerialization.hdf5";
    int numTGCValues = 20;

    // create
    iutgc_t TGC = iufTGCCreate(numTGCValues);

    // fill
    int i,status;
    for (i=0;i<numTGCValues;i++)
    {
        status = iufTGCSet(TGC,i,i*1.0f,i*2.0f);
        TEST_ASSERT_EQUAL(IUF_E_OK,status);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufTGCSave(TGC, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iutgc_t savedObj = iufTGCLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufTGCCompare(TGC, savedObj));
    iufTGCDelete(TGC);
    iufTGCDelete(savedObj);
}

TEST_GROUP_RUNNER(IufTGC)
{
    RUN_TEST_CASE(IufTGC, testIufTGCCreate);
    RUN_TEST_CASE(IufTGC, testIufTGCDelete);
    RUN_TEST_CASE(IufTGC, testIufTGCCompare);
    RUN_TEST_CASE(IufTGC, testIufTGCSetGet);
    RUN_TEST_CASE(IufTGC, testIufSerialization);
}
