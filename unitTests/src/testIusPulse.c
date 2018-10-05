

//
// Created by nlv09165 on 28/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusPulsePrivate.h>
#include <iusNonParametricPulse.h>
#include <iusParametricPulse.h>

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
    iup_t obj = (iup_t) iusParametricPulseCreate(label, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(notherLabel, numPulseValues);

    int status = iusPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);


    status = iusPulseDelete(notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    status = iusPulseDelete(NULL);
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
    iup_t obj = (iup_t) iusParametricPulseCreate(label, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(notherLabel, numPulseValues);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);

    equal = iusPulseCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUS_TRUE,equal);
    equal = iusPulseCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    // invalid params
    equal = iusPulseCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    equal = iusPulseCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUS_FALSE,equal);
    
    iusPulseDelete(obj);
    iusPulseDelete(notherObj);
}


TEST(IusPulse, testIusPulseGet)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    char    *pLabel="Parametric Pulse testIusPulseGet";
    char    *notherLabel = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    // create and save
    iup_t obj = (iup_t) iusParametricPulseCreate(pLabel, pulseFrequency, pulseAmplitude, pulseCount);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(notherLabel, pulseCount);

    // int param
    TEST_ASSERT_EQUAL(IUS_PARAMETRIC_PULSETYPE,iusPulseGetType( (iup_t) obj));
    TEST_ASSERT_EQUAL_STRING(pLabel,iusPulseGetLabel((iup_t) obj));

    TEST_ASSERT_EQUAL(IUS_NON_PARAMETRIC_PULSETYPE,iusPulseGetType( (iup_t) notherObj));
    TEST_ASSERT_EQUAL_STRING(notherLabel,iusPulseGetLabel((iup_t) notherObj));

    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_PULSETYPE,iusPulseGetType(NULL));
    TEST_ASSERT_EQUAL(NULL,iusPulseGetLabel(NULL));
   
    iusPulseDelete(obj);
    iusPulseDelete(notherObj);
}


TEST(IusPulse, testIusSerialization)
{

  // TODO how to cope with changes/addition in the data type? 

  char *filename = "testIusPulseSerialization.hdf5";

  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     pulseCount=10;               /**< number of cycles that the pulse represents */
  int     numPulseValues=2;               /**< number of cycles that the pulse represents */

  // create and save
  char *pulseLabel = "Parametric Pulse Created_in_IusPulse_testIusSerialization";
  iupp_t obj = iusParametricPulseCreate(pulseLabel, pulseFrequency, pulseAmplitude, pulseCount);
  iunpp_t notherObj = iusNonParametricPulseCreate("Non Parametric Pulse Created_in_IusPulse_testIusSerialization", numPulseValues);

  // fill
  int status = iusNonParametricPulseSetValue(notherObj,0,0.1f,10.0f);
  TEST_ASSERT_EQUAL(IUS_E_OK,status);
  status = iusNonParametricPulseSetValue(notherObj,1,0.2f,10.0f);
  TEST_ASSERT_EQUAL(IUS_E_OK,status);


  hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  TEST_ASSERT(handle > 0);
  status = iusPulseSave( (iup_t) obj, handle);
  H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUS_E_OK,status);

  // read back
  handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
  iup_t savedObj = iusPulseLoad(handle);
  TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
  H5Fclose(handle);

  TEST_ASSERT_EQUAL(IUS_TRUE, iusPulseCompare((iup_t)obj,savedObj));
  TEST_ASSERT_EQUAL(IUS_FALSE, iusPulseCompare((iup_t)notherObj, savedObj));
  iusPulseDelete((iup_t)obj);
  iusPulseDelete(savedObj);
}


TEST_GROUP_RUNNER(IusPulse)
{
    RUN_TEST_CASE(IusPulse, testIusPulseDelete);
    RUN_TEST_CASE(IusPulse, testIusPulseCompare);
    RUN_TEST_CASE(IusPulse, testIusPulseGet);
    RUN_TEST_CASE(IusPulse, testIusSerialization);
}
