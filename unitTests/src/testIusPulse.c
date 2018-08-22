

//
// Created by nlv09165 on 28/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLPulseImp.h>
#include <iusHLNonParametricPulse.h>
#include <iusHLParametricPulse.h>

TEST_GROUP(IusPulse);

TEST_SETUP(IusPulse)
{
}

TEST_TEAR_DOWN(IusPulse)
{
}


TEST(IusPulse, testIusPulseDelete)
{
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iusHLParametricPulseCreate(label, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusHLNonParametricPulseCreate(notherLabel, numPulseValues);

    int status = iusHLPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);


    status = iusHLPulseDelete(notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusHLPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
}



TEST(IusPulse, testIusPulseCompare)
{
    IUS_BOOL equal;
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    char *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iusHLParametricPulseCreate(label, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusHLNonParametricPulseCreate(notherLabel, numPulseValues);
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


TEST(IusPulse, testIusPulseGet)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    char    *pLabel="Parametric Pulse testIusPulseGet";
    char    *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    // create and save
    iup_t obj = (iup_t) iusHLParametricPulseCreate(pLabel, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusHLNonParametricPulseCreate(notherLabel, pulseCount);

    // int param
    TEST_ASSERT_EQUAL(IUS_PARAMETRIC_PULSETYPE,iusHLPulseGetType( (iup_t) obj));
    TEST_ASSERT_EQUAL_STRING(pLabel,iusHLPulseGetLabel((iup_t) obj));

    TEST_ASSERT_EQUAL(IUS_NON_PARAMETRIC_PULSETYPE,iusHLPulseGetType( (iup_t) notherObj));
    TEST_ASSERT_EQUAL_STRING(notherLabel,iusHLPulseGetLabel((iup_t) notherObj));

    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_PULSETYPE,iusHLPulseGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusHLPulseGetLabel(NULL));
   
    iusHLPulseDelete(obj);
    iusHLPulseDelete(notherObj);
}


TEST(IusPulse, testIusSerialization)
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


TEST_GROUP_RUNNER(IusPulse)
{
    RUN_TEST_CASE(IusPulse, testIusPulseDelete);
    RUN_TEST_CASE(IusPulse, testIusPulseCompare);
    RUN_TEST_CASE(IusPulse, testIusPulseGet);
    RUN_TEST_CASE(IusPulse, testIusSerialization);
}
