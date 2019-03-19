//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufPulsePrivate.h>
#include <iufParametricPulsePrivate.h>

static char *pErrorFilename = "IufParametricPulse.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufParametricPulse);

TEST_SETUP(IufParametricPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufParametricPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufParametricPulse, testIufCreatePulse)
{
    int status = 0;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    iupp_t  parametricPulse;

    // Parametric transmit pulse
    parametricPulse =
    iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    TEST_ASSERT(parametricPulse != IUPP_INVALID);


    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL,
                      iufParametricPulseCreate(pulseFrequency,
                                               pulseAmplitude,
                                               -1));
    TEST_ASSERT_EQUAL(NULL,
                      iufParametricPulseCreate(-1.0f,
                                               pulseAmplitude,
                                               numPulses));
    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}


TEST(IufParametricPulse, testIufDeletePulse)
{
    int status = 0;
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Parametric transmit pulse
    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufParametricPulseDelete((iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = iufParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}


TEST(IufParametricPulse, testIufComparePulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse,identicalPulse;
    iunpp_t nonParametricPulse;
    IUF_BOOL isEqual;


    // Parametric transmit pulse
    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    identicalPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);

    isEqual = iufParametricPulseCompare(parametricPulse, parametricPulse);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);
    isEqual = iufParametricPulseCompare(parametricPulse, identicalPulse);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);

    // Invalid arguments
    isEqual = iufParametricPulseCompare(parametricPulse, (iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);
    isEqual = iufParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);
    isEqual = iufParametricPulseCompare(NULL, parametricPulse);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);
    isEqual = iufParametricPulseCompare(parametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);

    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufParametricPulseDelete(identicalPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
}


TEST(IufParametricPulse, testIufSetGetPulse)
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
    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    TEST_ASSERT_EQUAL_FLOAT( iufParametricPulseGetFrequency(parametricPulse), pulseFrequency);
    TEST_ASSERT_EQUAL_FLOAT( iufParametricPulseGetPulseAmplitude(parametricPulse), pulseAmplitude);
    TEST_ASSERT_EQUAL_FLOAT( iufParametricPulseGetNumPulses(parametricPulse),numPulses);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    float freq = iufParametricPulseGetFrequency( (iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL_FLOAT(freq, NAN);
    float amp = iufParametricPulseGetPulseAmplitude( (iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL_FLOAT(amp, NAN);
    int num = iufParametricPulseGetNumPulses((iupp_t) nonParametricPulse);
    TEST_ASSERT_EQUAL(-1, num);
    freq = iufParametricPulseGetFrequency( (iupp_t) NULL);
    TEST_ASSERT_EQUAL_FLOAT(freq, NAN);
    amp = iufParametricPulseGetPulseAmplitude( (iupp_t) NULL);
    TEST_ASSERT_EQUAL_FLOAT(amp, NAN);
    TEST_ASSERT_EQUAL(-1, iufParametricPulseGetNumPulses((iupp_t) NULL));

    TEST_ASSERT_EQUAL(6,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}

TEST(IufParametricPulse, testIufSerialization)
{
    char *filename = "testIufParametricPulseSerialization.hdf5";
    //char *pulsePath =  "/ParametricPulse";
    
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */


    // create and save
    iupp_t parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    iupp_t notherParametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    int status = iufPulseSave((iup_t)parametricPulse, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iupp_t savedObj = iufParametricPulseLoad(handle);
    H5Fclose(handle);
	TEST_ASSERT_EQUAL(IUF_TRUE, iufParametricPulseCompare(parametricPulse, savedObj));
	iufParametricPulseDelete(savedObj);
    
    iufParametricPulseDelete(parametricPulse);
    iufParametricPulseDelete(notherParametricPulse);

}

TEST_GROUP_RUNNER(IufParametricPulse)
{
    RUN_TEST_CASE(IufParametricPulse, testIufCreatePulse);
    RUN_TEST_CASE(IufParametricPulse, testIufDeletePulse);
    RUN_TEST_CASE(IufParametricPulse, testIufComparePulse);
    RUN_TEST_CASE(IufParametricPulse, testIufSetGetPulse);
    RUN_TEST_CASE(IufParametricPulse, testIufSerialization);
}
