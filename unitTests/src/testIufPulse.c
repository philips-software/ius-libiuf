

//
// Created by nlv09165 on 28/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufPulsePrivate.h>

static char *pErrorFilename = "IufPulse.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufPulse);

TEST_SETUP(IufPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufPulse, testIufPulseDelete)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iufNonParametricPulseCreate(numPulseValues);

    int status = iufPulseDelete(obj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);


    status = iufPulseDelete(notherObj);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // invalid params
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));
}



TEST(IufPulse, testIufPulseCompare)
{
    IUF_BOOL equal;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=2;               /**< number of cycles that the pulse represents */


    // create and save
    iup_t obj = (iup_t) iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iufNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT(obj != IUP_INVALID);
    TEST_ASSERT(notherObj != IUP_INVALID);

    equal = iufPulseCompare(obj,obj);
    TEST_ASSERT_EQUAL(IUF_TRUE,equal);
    equal = iufPulseCompare(obj,notherObj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    // invalid params
    equal = iufPulseCompare(obj,NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    equal = iufPulseCompare(NULL,obj);
    TEST_ASSERT_EQUAL(IUF_FALSE,equal);
    
    iufPulseDelete(obj);
    iufPulseDelete(notherObj);
}


TEST(IufPulse, testIufPulseGet)
{
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */

    // create and save
    iup_t obj = (iup_t) iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iup_t notherObj = (iup_t) iufNonParametricPulseCreate(numPulses);

    // int param
    TEST_ASSERT_EQUAL(IUF_PARAMETRIC_PULSETYPE,iufPulseGetType( (iup_t) obj));
    TEST_ASSERT_EQUAL(IUF_NON_PARAMETRIC_PULSETYPE,iufPulseGetType( (iup_t) notherObj));
    // invalid param
    TEST_ASSERT_EQUAL(IUF_INVALID_PULSETYPE,iufPulseGetType(NULL));
   
    iufPulseDelete(obj);
    iufPulseDelete(notherObj);
}


TEST(IufPulse, testIufSerialization)
{
  char *filename = "testIufPulseSerialization.hdf5";

  float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
  float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
  int     numPulses=10;               /**< number of cycles that the pulse represents */
  int     numPulseValues=2;            /**< number of cycles that the pulse represents */

  // create and save
  iupp_t obj = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
  iunpp_t notherObj = iufNonParametricPulseCreate(numPulseValues);

  // fill
  int status = iufNonParametricPulseSetValue(notherObj,0,0.1f,10.0f);
  TEST_ASSERT_EQUAL(IUF_E_OK,status);
  status = iufNonParametricPulseSetValue(notherObj,1,0.2f,10.0f);
  TEST_ASSERT_EQUAL(IUF_E_OK,status);


  hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
  TEST_ASSERT(handle > 0);
  status = iufPulseSave( (iup_t) obj, handle);
  H5Fclose(handle);
  TEST_ASSERT_EQUAL(IUF_E_OK,status);

  // read back
  handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
  iup_t savedObj = iufPulseLoad(handle);
  TEST_ASSERT_NOT_EQUAL(NULL, savedObj);
  H5Fclose(handle);

  TEST_ASSERT_EQUAL(IUF_TRUE, iufPulseCompare((iup_t)obj,savedObj));
  TEST_ASSERT_EQUAL(IUF_FALSE, iufPulseCompare((iup_t)notherObj, savedObj));
  iufPulseDelete((iup_t)obj);
  iufPulseDelete(savedObj);
  iufPulseDelete((iup_t)notherObj);
}


TEST_GROUP_RUNNER(IufPulse)
{
    RUN_TEST_CASE(IufPulse, testIufPulseDelete);
    RUN_TEST_CASE(IufPulse, testIufPulseCompare);
    RUN_TEST_CASE(IufPulse, testIufPulseGet);
    RUN_TEST_CASE(IufPulse, testIufSerialization);
}
