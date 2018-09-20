
//
// Created by nlv09165 on 30/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHLPosition.h>
#include "include/iusHL2DParametricSourceImp.h"

TEST_GROUP(Ius2DParametricSource);

TEST_SETUP(Ius2DParametricSource)
{
}

TEST_TEAR_DOWN(Ius2DParametricSource)
{
}


TEST(Ius2DParametricSource, testIus2DParametricSourceCreate)
{
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5 ;

    iu2dps_t obj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);
    iu2dps_t notherObj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    iusHL2DParametricSourceDelete(obj);
    iusHL2DParametricSourceDelete(notherObj);

    // invalid params
    obj = iusHL2DParametricSourceCreate(NULL, 0, -1, 2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);
    obj = iusHL2DParametricSourceCreate(NULL, 0, 1, -2.0f, 0);
    TEST_ASSERT(obj == IU2DPS_INVALID);
}

TEST(Ius2DParametricSource, testIus2DParametricSourceDelete)
{
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5;

    iu2dps_t obj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    int status = iusHL2DParametricSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHL2DParametricSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(Ius2DParametricSource, testIus2DParametricSourceCompare)
{
    IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int numLocations = 5;

    iu2dps_t obj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);
    iu2dps_t notherObj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);
    iu2dps_t differentObj =
    iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber + 0.01f, angularDelta, startAngle);
    TEST_ASSERT(obj != IU2DPS_INVALID);
    TEST_ASSERT(notherObj != IU2DPS_INVALID);
    equal = iusHL2DParametricSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL2DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHL2DParametricSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iu2dp_t pos = iusHL2DPositionCreate(1.0,2.0);
    iusHL2DParametricSourceSetPosition(obj,pos,0);
    equal = iusHL2DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL2DParametricSourceSetPosition(notherObj,pos,0);
    equal = iusHL2DParametricSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);


    // invalid params
    equal = iusHL2DParametricSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHL2DParametricSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHL2DParametricSourceDelete(obj);
    iusHL2DParametricSourceDelete(notherObj);
    iusHL2DParametricSourceDelete(differentObj);
}

TEST(Ius2DParametricSource, testIus2DParametricSourceSetGet)
{
    //IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int p,numLocations = 5;

    iu2dps_t obj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);

    TEST_ASSERT_EQUAL_FLOAT(FNumber,iusHL2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta,iusHL2DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle,iusHL2DParametricSourceGetStartAngle(obj));

    // Set/Get location test
    for(p=0; p<numLocations; p++)
    {
        iu2dp_t pos = iusHL2DPositionCreate(p*1.0f,p*3.0f);
        iusHL2DParametricSourceSetPosition(obj,pos,p);
        iu2dp_t get = iusHL2DParametricSourceGetPosition(obj,p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DPositionCompare(pos,get));
    }



    // invalid param
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL2DParametricSourceGetFNumber(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL2DParametricSourceGetAngularDelta(NULL));
    TEST_ASSERT_EQUAL_FLOAT(NAN,iusHL2DParametricSourceGetStartAngle(NULL));
    TEST_ASSERT_EQUAL(IU2DP_INVALID, iusHL2DParametricSourceGetPosition(NULL,0));
    TEST_ASSERT_EQUAL(IU2DP_INVALID, iusHL2DParametricSourceGetPosition(obj,-1));

    iusHL2DParametricSourceDelete(obj);
}

TEST(Ius2DParametricSource, testIus2DParametricSourceSerialization)
{
    char *filename = "testIus2DParametricSourceSerialization.hdf5";
    //char *sourcePath =  "/2DParametricSource";

    //IUS_BOOL equal;
    float angularDelta = 0.13f;
    float FNumber = -0.955f;
    float startAngle = 3.14f;
    char *pLabel = "label for 2d parametric source";
    int p, numLocations = 5, status;


    // create
    iu2dps_t
    obj = iusHL2DParametricSourceCreate(pLabel, numLocations, FNumber, angularDelta, startAngle);

    // fill
    TEST_ASSERT_EQUAL_FLOAT(FNumber, iusHL2DParametricSourceGetFNumber(obj));
    TEST_ASSERT_EQUAL_FLOAT(angularDelta, iusHL2DParametricSourceGetAngularDelta(obj));
    TEST_ASSERT_EQUAL_FLOAT(startAngle, iusHL2DParametricSourceGetStartAngle(obj));

    for (p = 0; p < numLocations; p++)
    {
        iu2dp_t pos = iusHL2DPositionCreate(p * 1.0f, p * 3.0f);
        iusHL2DParametricSourceSetPosition(obj, pos, p);
        iu2dp_t get = iusHL2DParametricSourceGetPosition(obj, p);
        TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DPositionCompare(pos, get));
    }

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHL2DParametricSourceSave(obj, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iu2dps_t savedObj = iusHL2DParametricSourceLoad(handle, pLabel);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHL2DParametricSourceCompare(obj,savedObj));
    iusHL2DParametricSourceDelete(obj);
    iusHL2DParametricSourceDelete(savedObj);

}

TEST_GROUP_RUNNER(Ius2DParametricSource)
{
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceCreate);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceDelete);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceCompare);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceSetGet);
    RUN_TEST_CASE(Ius2DParametricSource, testIus2DParametricSourceSerialization);
}
