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
#include <iusHLParametricPulseImp.h>
#include <iusHLNonParametricPulse.h>

TEST_GROUP(IusParametricPulse);

TEST_SETUP(IusParametricPulse)
{
}

TEST_TEAR_DOWN(IusParametricPulse)
{
}



TEST(IusParametricPulse, testIusCreatePulse)
{
    int status = 0;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    iupp_t  parametricPulse;

    // Parametric transmit pulse
    parametricPulse =
    iusHLParametricPulseCreate("Created_in_testIusCreatePulse", pulseFrequency, pulseAmplitude, pulseCount);
    TEST_ASSERT(parametricPulse != IUPP_INVALID);


    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL,
                      iusHLParametricPulseCreate("Also_Created_in_testIusCreatePulse",
                                                 pulseFrequency,
                                                 pulseAmplitude,
                                                 -1));
    TEST_ASSERT_EQUAL(NULL,
                      iusHLParametricPulseCreate("Also_Created_in_testIusCreatePulse",
                                                 -1.0f,
                                                 pulseAmplitude,
                                                 pulseCount));
    TEST_ASSERT_EQUAL(NULL, iusHLParametricPulseCreate("", pulseFrequency, pulseAmplitude, pulseCount));
    TEST_ASSERT_EQUAL(NULL, iusHLParametricPulseCreate(NULL, pulseFrequency, pulseAmplitude, pulseCount));

    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusParametricPulse, testIusDeletePulse)
{
    int status = 0;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);

    // Invalid operation on nonparametric dta type
    status = iusHLNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusHLParametricPulseDelete((iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusHLParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusParametricPulse, testIusComparePulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse,notherParametricPulse,identicalPulse;
    iunpp_t nonParametricPulse;
    IUS_BOOL isEqual;


    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    identicalPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    notherParametricPulse =
    iusHLParametricPulseCreate("notherParametricPulse", pulseFrequency, pulseAmplitude, pulseCount);


    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);

    isEqual = iusHLParametricPulseCompare(parametricPulse, parametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusHLParametricPulseCompare(parametricPulse, identicalPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusHLParametricPulseCompare(parametricPulse, notherParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);

    // Invalid arguments
    isEqual = iusHLParametricPulseCompare(parametricPulse, (iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusHLParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusHLParametricPulseCompare(NULL, parametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusHLParametricPulseCompare(parametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(IusParametricPulse, testIusSetGetPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=30;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;


    // Parametric transmit pulse
    parametricPulse = iusHLParametricPulseCreate("parametricPulse", pulseFrequency, pulseAmplitude, pulseCount);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetFrequency(parametricPulse), pulseFrequency);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT( iusHLParametricPulseGetCount(parametricPulse),pulseCount);

    // Invalid operation on nonparametric dta type
    nonParametricPulse = iusHLNonParametricPulseCreate("nonParametricPulse", numPulseValues);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetFrequency( (iupp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetPulseAmplitude( (iupp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHLParametricPulseGetCount( (iupp_t) nonParametricPulse));

    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetFrequency( (iupp_t) NULL), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusHLParametricPulseGetPulseAmplitude( (iupp_t) NULL), NAN);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusHLParametricPulseGetCount( (iupp_t) NULL));

    status = iusHLNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusHLParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusParametricPulse, testIusSerialization)
{
    char *filename = "testIusParametricPulseSerialization.hdf5";
    char *pulsePath =  "/ParametricPulse";
    char *label = "label for IUS_PARAMETRIC_PULSETYPE";
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount=10;               /**< number of cycles that the pulse represents */


    // create and save
    iupp_t parametricPulse = iusHLParametricPulseCreate(label, pulseFrequency, pulseAmplitude, pulseCount);
    iupp_t notherParametricPulse = iusHLParametricPulseCreate("notherParametricPulse", pulseFrequency, pulseAmplitude, pulseCount);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusHLPulseSave((iup_t)parametricPulse, pulsePath, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupp_t savedObj = iusHLParametricPulseLoad(handle, pulsePath, label);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusHLParametricPulseCompare(parametricPulse,savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusHLParametricPulseCompare(notherParametricPulse,savedObj));
    iusHLParametricPulseDelete(parametricPulse);
    iusHLParametricPulseDelete(notherParametricPulse);
    iusHLParametricPulseDelete(savedObj);
}

TEST_GROUP_RUNNER(IusParametricPulse)
{
    RUN_TEST_CASE(IusParametricPulse, testIusCreatePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusDeletePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusComparePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusSetGetPulse);
    RUN_TEST_CASE(IusParametricPulse, testIusSerialization);
}
