

//
// Created by nlv09165 on 24/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
#include <include/iusHL3DSize.h>
#include <include/iusHL3DParametricSource.h>
#include <include/iusHL3DNonParametricSource.h>
#include <include/iusHL2DParametricSource.h>
#include <include/iusHL2DNonParametricSource.h>
#include "include/iusHLSource.h"



TEST_GROUP(IusSource);

TEST_SETUP(IusSource)
{
}

TEST_TEAR_DOWN(IusSource)
{
}


TEST(IusSource, testIusSourceCreate)
{
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";
    char *_2d_non_parametric_label = "label for 2d non parametric source";
    char *_2d_parametric_label = "label for 2d parametric source";

    // Happy flow
    iu3dps_t _3dps = (iu3dps_t) iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,_3d_parametric_label);
    TEST_ASSERT(_3dps != IU3DPS_INVALID);
    iu3dnps_t _3dnps = (iu3dnps_t) iusHLSourceCreate(IUS_3D_NON_PARAMETRIC_SOURCE,_3d_non_parametric_label);
    TEST_ASSERT(_3dnps != IU3DNPS_INVALID);
    iu2dps_t _2dps = (iu2dps_t) iusHLSourceCreate(IUS_2D_PARAMETRIC_SOURCE,_2d_parametric_label);
    TEST_ASSERT(_2dps != IU2DPS_INVALID);
    iu2dnps_t _2dnps = (iu2dnps_t) iusHLSourceCreate(IUS_2D_NON_PARAMETRIC_SOURCE,_2d_non_parametric_label);
    TEST_ASSERT(_2dnps != IU2DNPS_INVALID);


    // invalid params
    _3dps = (iu3dps_t) iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,"");
    TEST_ASSERT(_3dps == IU3DPS_INVALID);
    _3dps = (iu3dps_t) iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,NULL);
    TEST_ASSERT(_3dps == IU3DPS_INVALID);
    _3dnps = (iu3dnps_t) iusHLSourceCreate(IUS_3D_NON_PARAMETRIC_SOURCE,"");
    TEST_ASSERT(_3dnps == IU3DNPS_INVALID);
    _3dnps = (iu3dnps_t) iusHLSourceCreate(IUS_3D_NON_PARAMETRIC_SOURCE,NULL);
    TEST_ASSERT(_3dnps == IU3DNPS_INVALID);
    _2dps = (iu2dps_t) iusHLSourceCreate(IUS_2D_PARAMETRIC_SOURCE,"");
    TEST_ASSERT(_2dps == IU2DPS_INVALID);
    _2dps = (iu2dps_t) iusHLSourceCreate(IUS_2D_PARAMETRIC_SOURCE,NULL);
    TEST_ASSERT(_2dps == IU2DPS_INVALID);
    _2dnps = (iu2dnps_t) iusHLSourceCreate(IUS_2D_NON_PARAMETRIC_SOURCE,"");
    TEST_ASSERT(_2dnps == IU2DNPS_INVALID);
    _2dnps = (iu2dnps_t) iusHLSourceCreate(IUS_2D_NON_PARAMETRIC_SOURCE,NULL);
    TEST_ASSERT(_2dnps == IU2DNPS_INVALID);
    _3dps = (iu3dps_t) iusHLSourceCreate( 2139, "invalid type");
    TEST_ASSERT(_3dps == IU3DPS_INVALID);

}

TEST(IusSource, testIusSourceDelete)
{
    char *_3d_parametric_label = "label for 3d parametric source";

    iu3dps_t _3dps = (iu3dps_t) iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,_3d_parametric_label);
    TEST_ASSERT(_3dps != IU3DPS_INVALID);
    int status = iusHLSourceDelete((ius_t)_3dps);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    ius_t obj = iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,_3d_parametric_label);
    TEST_ASSERT(obj != IUS_INVALID);
    status = iusHLSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLSourceDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusSource, testIusSourceCompare)
{
    IUS_BOOL equal;
    char *_3d_non_parametric_label = "label for 3d non parametric source";
    char *_3d_parametric_label = "label for 3d parametric source";

    // Happy flow
    ius_t obj = iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,_3d_parametric_label);
    TEST_ASSERT(obj != IUS_INVALID);
    ius_t notherObj = iusHLSourceCreate(IUS_3D_PARAMETRIC_SOURCE,_3d_parametric_label);
    TEST_ASSERT(notherObj != IUS_INVALID);
    ius_t differentObj = iusHLSourceCreate(IUS_3D_NON_PARAMETRIC_SOURCE,_3d_non_parametric_label);
    TEST_ASSERT(differentObj != IUS_INVALID);
    equal = iusHLSourceCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLSourceCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLSourceCompare(obj,differentObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHLSourceCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLSourceCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    int status = iusHLSourceDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHLSourceDelete(notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
    status = iusHLSourceDelete(differentObj);
    TEST_ASSERT_EQUAL(IUS_E_OK, status);
}


TEST(IusSource, testIusSourceGetSet)
{
    IUS_BOOL equal;
    char *_3d_non_parametric_label = "label for 3d non parametric source";

    // Happy flow
    ius_t obj = iusHLSourceCreate(IUS_3D_NON_PARAMETRIC_SOURCE,_3d_non_parametric_label);
    TEST_ASSERT(obj != IUS_INVALID);

    TEST_ASSERT_EQUAL(IUS_3D_NON_PARAMETRIC_SOURCE,iusHLSourceGetType(obj));
    TEST_ASSERT_EQUAL_STRING(_3d_non_parametric_label,iusHLSourceGetLabel(obj));


    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_SOURCE_TYPE,iusHLSourceGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusHLSourceGetLabel(NULL));
    iusHLSourceDelete(obj);
}

#if 0

TEST(IusSource, testIusSerialization)
{
    char *filename = "testIusPulseSerialization.hdf5";
    char *pulsePath =  "/Pulse";

    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iupp_t obj = iusHLParametricPulseCreate("Parametric Pulse Created_in_IusPulse_testIusSerialization", pulseFrequency, pulseAmplitude, pulseCount);
    iunpp_t notherObj = iusHLNonParametricPulseCreate("Non Parametric Pulse Created_in_IusPulse_testIusSerialization", numPulseValues);

    // fill
    int status = iusHLNonParametricPulseSetValue(notherObj,0,0.1f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLNonParametricPulseSetValue(notherObj,1,0.2f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);


    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLPulseSave( (iup_t) obj, pulsePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iup_t savedObj = iusHLPulseLoad(handle, pulsePath);
    TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLPulseCompare((iup_t)obj,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusHLPulseCompare((iup_t)notherObj,savedObj));
    iusHLPulseDelete((iup_t)obj);
    iusHLPulseDelete(savedObj);
}

#endif

TEST_GROUP_RUNNER(IusSource)
{
    RUN_TEST_CASE(IusSource, testIusSourceCreate);
    RUN_TEST_CASE(IusSource, testIusSourceDelete);
    RUN_TEST_CASE(IusSource, testIusSourceCompare);
    RUN_TEST_CASE(IusSource, testIusSourceGetSet);
//    RUN_TEST_CASE(IusSource, testIusSerialization);
}
