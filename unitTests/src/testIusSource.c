

//
// Created by nlv09165 on 24/07/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusTypes.h>
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
    IUS_BOOL equal;
    TEST_ASSERT(0==0);
#if 0
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    iup_t notherObj = iusHLPulseCreate(IUS_NON_PARAMETRIC_PULSETYPE,notherLabel);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);
    iusHLPulseDelete(obj);
    iusHLPulseDelete(notherObj);

    // invalid params
    obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,NULL);
    TEST_ASSERT(obj == IUP_INVALID);
    iusHLPulseCreate(-1,label);
    TEST_ASSERT(obj == IUP_INVALID);
#endif

}

#if 0
TEST(IusSource, testIusSourceDelete)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    TEST_ASSERT(obj != IUP_INVALID);
    int status = iusHLPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusSource, testIusSourceCompare)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);
    iup_t notherObj = iusHLPulseCreate(IUS_NON_PARAMETRIC_PULSETYPE,notherLabel);
    TEST_ASSERT(obj != IUP_INVALID);

    TEST_ASSERT(notherObj != IUP_INVALID);
    equal = iusHLPulseCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusHLPulseCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    // invalid params
    equal = iusHLPulseCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusHLPulseCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);

    iusHLPulseDelete(obj);
    iusHLPulseDelete(notherObj);
}


TEST(IusSource, testIusSourceGet)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";

    iup_t obj = iusHLPulseCreate(IUS_PARAMETRIC_PULSETYPE,label);

    // int param
    TEST_ASSERT_EQUAL(IUS_PARAMETRIC_PULSETYPE,iusHLPulseGetType(obj));
    TEST_ASSERT_EQUAL_STRING(label,iusHLPulseGetLabel(obj));


    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_PULSETYPE,iusHLPulseGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusHLPulseGetLabel(NULL));

    iusHLPulseDelete(obj);
}


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
//    RUN_TEST_CASE(IusSource, testIusSourceDelete);
//    RUN_TEST_CASE(IusSource, testIusSourceCompare);
//    RUN_TEST_CASE(IusSource, testIusSourceGet);
//    RUN_TEST_CASE(IusSource, testIusSerialization);
}
