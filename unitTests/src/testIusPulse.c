

//
// Created by nlv09165 on 28/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusPulsePrivate.h>

static char *pErrorFilename = "IusPulse.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusPulse);

TEST_SETUP(IusPulse)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReport(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusPulse)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusPulse, testIusPulseDelete)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(numPulseValues);

    int status = iusPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);


    status = iusPulseDelete(notherObj);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IusPulse, testIusPulseCompare)
{
    IUS_BOOL equal;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(numPulseValues);
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
    int     numPulses=10;               /**< number of cycles that the pulse represents */

    // create and save
    iup_t obj = (iup_t) iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iusNonParametricPulseCreate(numPulses);

    // int param
    TEST_ASSERT_EQUAL(IUS_PARAMETRIC_PULSETYPE,iusPulseGetType( (iup_t) obj));
    TEST_ASSERT_EQUAL(IUS_NON_PARAMETRIC_PULSETYPE,iusPulseGetType( (iup_t) notherObj));
    // invalid param
    TEST_ASSERT_EQUAL(IUS_INVALID_PULSETYPE,iusPulseGetType(NULL));
   
    iusPulseDelete(obj);
    iusPulseDelete(notherObj);
}


TEST(IusPulse, testIusSerialization)
{
  char *filename = "testIusPulseSerialization.hdf5";

  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     numPulses=10;               /**< number of cycles that the pulse represents */
  int     numPulseValues=2;            /**< number of cycles that the pulse represents */

  // create and save
  iupp_t obj = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
  iunpp_t notherObj = iusNonParametricPulseCreate(numPulseValues);

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
  iusPulseDelete((iup_t)notherObj);
}


TEST_GROUP_RUNNER(IusPulse)
{
    RUN_TEST_CASE(IusPulse, testIusPulseDelete);
    RUN_TEST_CASE(IusPulse, testIusPulseCompare);
    RUN_TEST_CASE(IusPulse, testIusPulseGet);
    RUN_TEST_CASE(IusPulse, testIusSerialization);
}
