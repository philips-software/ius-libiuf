
//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iuf3DNonParametricSourcePrivate.h>

static char *pErrorFilename = "Iuf3DNonParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Iuf3DNonParametricSource);

TEST_SETUP(Iuf3DNonParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Iuf3DNonParametricSource)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Iuf3DNonParametricSource, testIuf3DNonParametricSourceCreate)
{
    int numLocations = 5 ;

    iu3dnps_t obj = iuf3DNonParametricSourceCreate(numLocations);
    iu3dnps_t notherObj = iuf3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    iuf3DNonParametricSourceDelete(obj);
    iuf3DNonParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    obj = iuf3DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);
    obj = iuf3DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IU3DNPS_INVALID);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Iuf3DNonParametricSource, testIuf3DNonParametricSourceDelete)
{
    int numLocations = 5;

    iu3dnps_t obj = iuf3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    int status = iuf3DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iuf3DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}



TEST(Iuf3DNonParametricSource, testIuf3DNonParametricSourceCompare)
{
    IUF_BOOL equal;
    int numLocations = 5;

    iu3dnps_t obj = iuf3DNonParametricSourceCreate(numLocations);
    iu3dnps_t notherObj = iuf3DNonParametricSourceCreate(numLocations);
    iu3dnps_t differentObj =
    iuf3DNonParametricSourceCreate(numLocations+1);
    TEST_ASSERT(obj != IU3DNPS_INVALID);
    TEST_ASSERT(notherObj != IU3DNPS_INVALID);
    equal = iuf3DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iuf3DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iu3dp_t pos = iuf3DPositionCreate(1.0,2.0,3.0);
    iuf3DNonParametricSourceSetPosition(obj,pos,0);
    equal = iuf3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);

    iuf3DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = iuf3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);


    // invalid params
    equal = iuf3DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iuf3DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iuf3DNonParametricSourceDelete(obj);
    iuf3DNonParametricSourceDelete(notherObj);
    iuf3DNonParametricSourceDelete(differentObj);
    iuf3DPositionDelete(pos);
}

TEST(Iuf3DNonParametricSource, testIuf3DNonParametricSourceSetGet)
{
    int p,numLocations = 5;

    iu3dnps_t obj = iuf3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT_EQUAL(numLocations, iuf3DNonParametricSourceGetNumLocations(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = iuf3DPositionCreate(p*1.0f,p*2.0f,p*3.0f);
        iuf3DNonParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = iuf3DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DPositionCompare(pos,get));
        iuf3DPositionDelete(pos);
    }

    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(IU3DP_INVALID, iuf3DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, iuf3DNonParametricSourceGetPosition(obj,-1));
    TEST_ASSERT_EQUAL(-1, iuf3DNonParametricSourceGetNumLocations(NULL));

    TEST_ASSERT_EQUAL(3,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    iuf3DNonParametricSourceDelete(obj);
}

TEST(Iuf3DNonParametricSource, testIuf3DNonParametricSourceSerialization)
{
    char *filename = "testIuf3DNonParametricSourceSerialization.hdf5";
    int p, numLocations = 5, status;


    // create
    iu3dnps_t
    obj = iuf3DNonParametricSourceCreate(numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = iuf3DPositionCreate(p * 1.0f, p * 2.0f, p * 3.0f);
        iuf3DNonParametricSourceSetPosition(obj, pos, p);
        iu3dp_t get = iuf3DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DPositionCompare(pos, get));
        iuf3DPositionDelete(pos);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iuf3DNonParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu3dnps_t savedObj = iuf3DNonParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iuf3DNonParametricSourceCompare(obj,savedObj));
    iuf3DNonParametricSourceDelete(obj);
    iuf3DNonParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Iuf3DNonParametricSource)
{
    RUN_TEST_CASE(Iuf3DNonParametricSource, testIuf3DNonParametricSourceCreate);
    RUN_TEST_CASE(Iuf3DNonParametricSource, testIuf3DNonParametricSourceDelete);
    RUN_TEST_CASE(Iuf3DNonParametricSource, testIuf3DNonParametricSourceCompare);
    RUN_TEST_CASE(Iuf3DNonParametricSource, testIuf3DNonParametricSourceSetGet);
    RUN_TEST_CASE(Iuf3DNonParametricSource, testIuf3DNonParametricSourceSerialization);
}
