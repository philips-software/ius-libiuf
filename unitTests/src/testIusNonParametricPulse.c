//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusHLPulseImp.h>
#include <iusHLParametricPulse.h>
#include <iusHLNonParametricPulseImp.h>

#include <testDataGenerators.h>

TEST_GROUP(IusNonParametricPulse);

TEST_SETUP(IusNonParametricPulse)
{
}

TEST_TEAR_DOWN(IusNonParametricPulse)
{
}



TEST(IusNonParametricPulse, testIusCreateNonParametricPulse)
{
    int status = 0;
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iunpp_t nonParametricPulse;

    numPulseValues = 20;
    nonParametricPulse = iusHLNonParametricPulseCreate("Created_in_testIusCreateNonParametricPulse", numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);

    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate("Also_Created_in_testIusCreateNonParametricPulse", -1));
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate("", numPulseValues));
    TEST_ASSERT_EQUAL(NULL, iusHLNonParametricPulseCreate(NULL, numPulseValues));

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusNonParametricPulse, testIusDeleteNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse =
    iusHLNonParametricPulseCreate("NonParametricPulse_in_testIusDeleteNonParametricPulse", numPulseValues);

    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("ParametricPulse_in_testIusDeleteNonParametricPulse",
                                                 pulseFrequency,
                                                 pulseAmplitude,
                                                 pulseCount);

    // Invalid operation on nonparametric dta type
    status = iusHLNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusHLNonParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}



TEST(IusNonParametricPulse, testIusCompareNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse,notherNonParametricPulse;
    IUS_BOOL isEqual;

    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);
    notherNonParametricPulse = iusHLNonParametricPulseCreate("notherNonParametricPulse", numPulseValues);

    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);

    isEqual = iusHLNonParametricPulseCompare(nonParametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusHLNonParametricPulseCompare(nonParametricPulse, notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);

    // Invalid arguments
    isEqual = iusHLNonParametricPulseCompare((iunpp_t) parametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusHLNonParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusHLNonParametricPulseCompare(NULL, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusHLNonParametricPulseCompare(nonParametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);


    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(IusNonParametricPulse, testIusSetGetNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseTime=0.00001f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=40.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;


    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);
    TEST_ASSERT_EQUAL(numPulseValues,iusHLNonParametricPulseGetNumValues(nonParametricPulse));

    status = iusHLNonParametricPulseSetValue(nonParametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude, iusHLNonParametricPulseGetValueAmplitude(nonParametricPulse, 0));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iusHLNonParametricPulseGetValueTime(nonParametricPulse, 0));

    pulseTime *= numPulseValues;
    pulseAmplitude *= numPulseValues;
    status = iusHLNonParametricPulseSetValue(nonParametricPulse, numPulseValues - 1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude,
                            iusHLNonParametricPulseGetValueAmplitude(nonParametricPulse, numPulseValues - 1));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iusHLNonParametricPulseGetValueTime(nonParametricPulse, numPulseValues - 1));

    // Invalid params
    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    status = iusHLNonParametricPulseSetValue((iunpp_t) parametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetValue(NULL, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetValue(nonParametricPulse, 0, -1.0f, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetValue(nonParametricPulse, -1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusHLNonParametricPulseSetValue(nonParametricPulse, numPulseValues, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);


    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusNonParametricPulse, testIusSerialization)
{
  char *filename = "testIusNonParametricPulseSerialization.hdf5";
  char *pulsePath =  "/NonParametricPulse";
  char *label = "label for IUS_NON_PARAMETRIC_PULSETYPE";

    int numPulseValues = 20;

    // create
    iunpp_t nonParametricPulse = iusHLNonParametricPulseCreate("Created_in_testIusCreateNonParametricPulse", numPulseValues);
    iunpp_t notherNonParametricPulse = iusHLNonParametricPulseCreate("Created_in_testIusCreateNonParametricPulse", numPulseValues+10);

    // fill
    int status = iusHLNonParametricPulseSetValue(nonParametricPulse,0,0.1f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLNonParametricPulseSetValue(nonParametricPulse,1,0.2f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusHLPulseSave((iup_t)nonParametricPulse, pulsePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iunpp_t savedObj = iusHLNonParametricPulseLoad(handle, pulsePath, label);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLNonParametricPulseCompare(nonParametricPulse,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusHLNonParametricPulseCompare(notherNonParametricPulse,savedObj));
    iusHLNonParametricPulseDelete(nonParametricPulse);
    iusHLNonParametricPulseDelete(notherNonParametricPulse);
    iusHLNonParametricPulseDelete(savedObj);
}

TEST_GROUP_RUNNER(IusNonParametricPulse)
{
    RUN_TEST_CASE(IusNonParametricPulse, testIusCreateNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusDeleteNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusCompareNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusSetGetNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusSerialization);
}
