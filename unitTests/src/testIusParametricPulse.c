//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusPulsePrivate.h>
#include <iusParametricPulsePrivate.h>

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
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    iupp_t  parametricPulse;

    // Parametric transmit pulse
    parametricPulse =
    iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    TEST_ASSERT(parametricPulse != IUPP_INVALID);


    // Invalid operation on nonparametric dta type
    TEST_ASSERT_EQUAL(NULL,
                      iusParametricPulseCreate(pulseFrequency,
                                               pulseAmplitude,
                                               -1));
    TEST_ASSERT_EQUAL(NULL,
                      iusParametricPulseCreate(-1.0f,
                                               pulseAmplitude,
                                               numPulses));

    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusParametricPulse, testIusDeletePulse)
{
    int status = 0;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Parametric transmit pulse
    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);

    // Invalid operation on nonparametric dta type
    status = iusNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusParametricPulseDelete((iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusParametricPulse, testIusComparePulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse,identicalPulse;
    iunpp_t nonParametricPulse;
    IUS_BOOL isEqual;


    // Parametric transmit pulse
    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    identicalPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);

    isEqual = iusParametricPulseCompare(parametricPulse, parametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusParametricPulseCompare(parametricPulse, identicalPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);

    // Invalid arguments
    isEqual = iusParametricPulseCompare(parametricPulse, (iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusParametricPulseCompare(NULL, parametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusParametricPulseCompare(parametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);

    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusParametricPulseDelete(identicalPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(IusParametricPulse, testIusSetGetPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=30;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;


    // Parametric transmit pulse
    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    TEST_ASSERT_EQUAL_FLOAT( iusParametricPulseGetFrequency(parametricPulse), pulseFrequency);
    TEST_ASSERT_EQUAL_FLOAT( iusParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT( iusParametricPulseGetNumPulses(parametricPulse),numPulses);

    // Invalid operation on nonparametric dta type
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT_EQUAL_FLOAT(iusParametricPulseGetFrequency( (iupp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusParametricPulseGetPulseAmplitude( (iupp_t) nonParametricPulse), NAN);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusParametricPulseGetNumPulses((iupp_t) nonParametricPulse));

    TEST_ASSERT_EQUAL_FLOAT(iusParametricPulseGetFrequency( (iupp_t) NULL), NAN);
    TEST_ASSERT_EQUAL_FLOAT(iusParametricPulseGetPulseAmplitude( (iupp_t) NULL), NAN);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, iusParametricPulseGetNumPulses((iupp_t) NULL));

    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusParametricPulse, testIusSerialization)
{
    char *filename = "testIusParametricPulseSerialization.hdf5";
    //char *pulsePath =  "/ParametricPulse";
    
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */


    // create and save
    iupp_t parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iupp_t notherParametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iusPulseSave((iup_t)parametricPulse, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupp_t savedObj = iusParametricPulseLoad(handle);
    H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUS_TRUE, iusParametricPulseCompare(parametricPulse, savedObj));
	iusParametricPulseDelete(savedObj);
    
    iusParametricPulseDelete(parametricPulse);
    iusParametricPulseDelete(notherParametricPulse);

}

TEST_GROUP_RUNNER(IusParametricPulse)
{
    RUN_TEST_CASE(IusParametricPulse, testIusCreatePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusDeletePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusComparePulse);
    RUN_TEST_CASE(IusParametricPulse, testIusSetGetPulse);
    RUN_TEST_CASE(IusParametricPulse, testIusSerialization);
}
