
//
// Created by nlv09165 on 26/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <ius3DNonParametricSourcePrivate.h>

static char *pErrorFilename = "Ius3DNonParametricSource.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(Ius3DNonParametricSource);

TEST_SETUP(Ius3DNonParametricSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(Ius3DNonParametricSource)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceCreate)
{
    int numLocations = 5 ;

    ius_t obj = ius3DNonParametricSourceCreate(numLocations);
    ius_t notherObj = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IUS_INVALID);
    TEST_ASSERT(notherObj != IUS_INVALID);
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(notherObj);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    obj = ius3DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IUS_INVALID);
    obj = ius3DNonParametricSourceCreate(0);
    TEST_ASSERT(obj == IUS_INVALID);

    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete)
{
    int numLocations = 5;

    ius_t obj = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT(obj != IUS_INVALID);
    int status = ius3DNonParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = ius3DNonParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare)
{
    IUS_BOOL equal;
    int numLocations = 5;

    ius_t obj = ius3DNonParametricSourceCreate(numLocations);
    ius_t notherObj = ius3DNonParametricSourceCreate(numLocations);
    ius_t differentObj = ius3DNonParametricSourceCreate(numLocations+1);
    TEST_ASSERT(obj != IUS_INVALID);
    TEST_ASSERT(notherObj != IUS_INVALID);
    equal = ius3DNonParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = ius3DNonParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu3dp_t pos = ius3DPositionCreate(1.0,2.0,3.0);
    ius3DNonParametricSourceSetPosition(obj,pos,0);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    ius3DNonParametricSourceSetPosition(notherObj,pos,0);
    equal = ius3DNonParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);

    // invalid params
    equal = ius3DNonParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = ius3DNonParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(notherObj);
    ius3DNonParametricSourceDelete(differentObj);
    ius3DPositionDelete(pos);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet)
{
    int p,numLocations = 5;

    ius_t obj = ius3DNonParametricSourceCreate(numLocations);
    TEST_ASSERT_EQUAL(numLocations, ius3DNonParametricSourceGetNumLocations(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p*1.0f,p*2.0f,p*3.0f);
        ius3DNonParametricSourceSetPosition(obj,pos,p);
        iu3dp_t get = ius3DNonParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius3DPositionCompare(pos,get));
        ius3DPositionDelete(pos);
    }
    // invalid param
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DNonParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU3DP_INVALID, ius3DNonParametricSourceGetPosition(obj,-1));
    TEST_ASSERT_EQUAL(-1, ius3DNonParametricSourceGetNumLocations(NULL));

    TEST_ASSERT_EQUAL(3,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    ius3DNonParametricSourceDelete(obj);
}

TEST(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization)
{
    char *filename = "testIus3DNonParametricSourceSerialization.hdf5";
    int p, numLocations = 5, status;

    // create
    ius_t obj = ius3DNonParametricSourceCreate(numLocations);

    // fill
    for (p = 0; p < numLocations; p++)
    {
        iu3dp_t pos = ius3DPositionCreate(p * 1.0f, p * 2.0f, p * 3.0f);
        ius3DNonParametricSourceSetPosition(obj, pos, p);
        iu3dp_t get = ius3DNonParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, ius3DPositionCompare(pos, get));
        ius3DPositionDelete(pos);
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = ius3DNonParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    ius_t savedObj = ius3DNonParametricSourceLoad(handle);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, ius3DNonParametricSourceCompare(obj,savedObj));
    ius3DNonParametricSourceDelete(obj);
    ius3DNonParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Ius3DNonParametricSource)
{
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCreate);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceDelete);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceCompare);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSetGet);
    RUN_TEST_CASE(Ius3DNonParametricSource, testIus3DNonParametricSourceSerialization);
}
