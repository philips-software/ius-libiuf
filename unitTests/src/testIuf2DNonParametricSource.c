
//
// Created by nlv09165 on 31/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf2DNonParametricSourcePrivate.h>

static char *pErrorFilename = "Iuf2DNonParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf2DNonParametricSource);

TEST_SETUP(Iuf2DNonParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf2DNonParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Iuf2DNonParametricSource, testIuf2DNonParametricSourceCreate)
{
    int numLocations = 5 ;

    iu2dnps_t obj = iuf2DNonParametricSourceCreate(numLocations);
    iu2dnps_t notherObj = iuf2DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    iuf2DNonParametricSourceDelete(obj);
    iuf2DNonParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iuf2DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);
    obj = iuf2DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU2DNPS_INVALID);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf2DNonParametricSource, testIuf2DNonParametricSourceDelete)
{
    int numLocations = 5;

    iu2dnps_t obj = iuf2DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    int status = iuf2DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf2DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(Iuf2DNonParametricSource, testIuf2DNonParametricSourceCompare)
{
    IUF_BOOL equal;
    int numLocations = 5;

    iu2dnps_t obj = iuf2DNonParametricSourceCreate(numLocations);
    iu2dnps_t notherObj = iuf2DNonParametricSourceCreate(numLocations);
    iu2dnps_t differentObj = iuf2DNonParametricSourceCreate(numLocations+2);
    TEST_ASSERT(obj != IU2DNPS_INVALID);
    TEST_ASSERT(notherObj != IU2DNPS_INVALID);
    equal = iuf2DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf2DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iu2dp_t pos = iuf2DPositionCreate(1.0,2.0);
    iuf2DNonParametricSourceSetPosition(obj,pos,0);
    equal = iuf2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iuf2DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = iuf2DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);


    // invalid params
    equal = iuf2DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf2DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iuf2DNonParametricSourceDelete(obj);
    iuf2DNonParametricSourceDelete(notherObj);
    iuf2DNonParametricSourceDelete(differentObj);
    iuf2DPositionDelete(pos);
}

TEST(Iuf2DNonParametricSource, testIuf2DNonParametricSourceSetGet)
{
    int p,numLocations = 5;

    iu2dnps_t obj = iuf2DNonParametricSourceCreate(numLocations);
    TEST_ASSERT_EQUAL(numLocations, iuf2DNonParametricSourceGetNumLocations(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu2dp_t pos = iuf2DPositionCreate(p*1.0f,p*3.0f);
        iuf2DNonParametricSourceSetPosition(obj,pos,p);
        iu2dp_t get = iuf2DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DPositionCompare(pos,get));
        iuf2DPositionDelete(pos);
    }

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(IU2DP_INVALID, iuf2DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU2DP_INVALID, iuf2DNonParametricSourceGetPosition(obj,-1));
    TEST_ASSERT_EQUAL(-1, iuf2DNonParametricSourceGetNumLocations(NULL));

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    iuf2DNonParametricSourceDelete(obj);
}

TEST(Iuf2DNonParametricSource, testIuf2DNonParametricSourceSerialization)
{
    char *filename = "testIuf2DNonParametricSourceSerialization.hdf5";
    int p, numLocations = 5, status;


    // create
    iu2dnps_t
    obj = iuf2DNonParametricSourceCreate(numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu2dp_t pos = iuf2DPositionCreate(p * 1.0f, p * 3.0f);
        iuf2DNonParametricSourceSetPosition(obj, pos, p);
        iu2dp_t get = iuf2DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DPositionCompare(pos, get));
        iuf2DPositionDelete(pos);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iuf2DNonParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dnps_t savedObj = iuf2DNonParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf2DNonParametricSourceCompare(obj,savedObj));
    iuf2DNonParametricSourceDelete(obj);
    iuf2DNonParametricSourceDelete(savedObj);
}

TEST_GROUP_RUNNER(Iuf2DNonParametricSource)
{
    RUN_TEST_CASE(Iuf2DNonParametricSource, testIuf2DNonParametricSourceCreate);
    RUN_TEST_CASE(Iuf2DNonParametricSource, testIuf2DNonParametricSourceDelete);
    RUN_TEST_CASE(Iuf2DNonParametricSource, testIuf2DNonParametricSourceCompare);
    RUN_TEST_CASE(Iuf2DNonParametricSource, testIuf2DNonParametricSourceSetGet);
    RUN_TEST_CASE(Iuf2DNonParametricSource, testIuf2DNonParametricSourceSerialization);
}
