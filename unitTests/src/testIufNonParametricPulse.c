//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <iuf.h>
#include <iufNonParametricPulsePrivate.h>
#include <dg/dataGenerators.h>

static char *pErrorFilename = "IufNonParametricPulse.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IufNonParametricPulse);

TEST_SETUP(IufNonParametricPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_TRUE);
    iufErrorAutoReportSet(IUF_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iufErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IufNonParametricPulse)
{
    iufErrorLogClear();
    iufErrorLog(IUF_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iufErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IufNonParametricPulse, testIufCreateNonParametricPulse)
{
    int status = 0;
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iunpp_t nonParametricPulse;

    numPulseValues = 20;
    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iufNonParametricPulseCreate(-1));

    TEST_ASSERT_EQUAL(1,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}

TEST(IufNonParametricPulse, testIufDeleteNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;

    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse =
    iufNonParametricPulseCreate(numPulseValues);

    // Parametric transmit pulse
    parametricPulse = iufParametricPulseCreate(pulseFrequency,
                                               pulseAmplitude,
                                               numPulses);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    status = iufNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);
    status = iufNonParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}



TEST(IufNonParametricPulse, testIufCompareNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse,notherNonParametricPulse;
    IUF_BOOL isEqual;

    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    notherNonParametricPulse = iufNonParametricPulseCreate(numPulseValues);

    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    isEqual = iufNonParametricPulseCompare(nonParametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);
    isEqual = iufNonParametricPulseCompare(nonParametricPulse, notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);

    // Invalid arguments
    isEqual = iufNonParametricPulseCompare((iunpp_t) parametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);
    isEqual = iufNonParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUF_TRUE,isEqual);
    isEqual = iufNonParametricPulseCompare(NULL, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);
    isEqual = iufNonParametricPulseCompare(nonParametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUF_FALSE,isEqual);


    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufNonParametricPulseDelete(notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
}


TEST(IufNonParametricPulse, testIufSetGetNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseTime=0.00001f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=40.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse;


    // Non Parametric transmit pulse
    // alloc mem
    numPulseValues = 20;
    nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);
    TEST_ASSERT_EQUAL(numPulseValues,iufNonParametricPulseGetNumValues(nonParametricPulse));

    status = iufNonParametricPulseSetValue(nonParametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude, iufNonParametricPulseGetValueAmplitude(nonParametricPulse, 0));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iufNonParametricPulseGetValueTime(nonParametricPulse, 0));

    pulseTime *= numPulseValues;
    pulseAmplitude *= numPulseValues;
    status = iufNonParametricPulseSetValue(nonParametricPulse, numPulseValues - 1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude,
                            iufNonParametricPulseGetValueAmplitude(nonParametricPulse, numPulseValues - 1));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iufNonParametricPulseGetValueTime(nonParametricPulse, numPulseValues - 1));

    // Invalid params
    // Parametric transmit pulse
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iufErrorGetCount());

    parametricPulse = iufParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    status = iufNonParametricPulseSetValue((iunpp_t) parametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufNonParametricPulseSetValue(NULL, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufNonParametricPulseSetValue(nonParametricPulse, 0, -1.0f, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufNonParametricPulseSetValue(nonParametricPulse, -1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);
    status = iufNonParametricPulseSetValue(nonParametricPulse, numPulseValues, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUF_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(5,iufErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iufNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
    status = iufParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUF_E_OK);
}


TEST(IufNonParametricPulse, testIufSerialization)
{
    char *filename = "testIufNonParametricPulseSerialization.hdf5";
    int numPulseValues = 20;

    // create
    iunpp_t nonParametricPulse = iufNonParametricPulseCreate(numPulseValues);
    iunpp_t notherNonParametricPulse = iufNonParametricPulseCreate(numPulseValues+10);

    // fill
    int status = iufNonParametricPulseSetValue(nonParametricPulse,0,0.1f,10.0f);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);
    status = iufNonParametricPulseSetValue(nonParametricPulse,1,0.2f,10.0f);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

	// A specific pulse can only be loaded if you know the label in advance! this test needs to be rewritten
    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iufNonParametricPulseSave(nonParametricPulse, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUF_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iunpp_t savedObj = iufNonParametricPulseLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUF_TRUE, iufNonParametricPulseCompare(nonParametricPulse, savedObj));
    TEST_ASSERT_EQUAL(IUF_FALSE, iufNonParametricPulseCompare(notherNonParametricPulse, savedObj));
	iufNonParametricPulseDelete(savedObj);
 
    iufNonParametricPulseDelete(nonParametricPulse);
    iufNonParametricPulseDelete(notherNonParametricPulse);
    
}

TEST_GROUP_RUNNER(IufNonParametricPulse)
{
    RUN_TEST_CASE(IufNonParametricPulse, testIufCreateNonParametricPulse);
    RUN_TEST_CASE(IufNonParametricPulse, testIufDeleteNonParametricPulse);
    RUN_TEST_CASE(IufNonParametricPulse, testIufCompareNonParametricPulse);
    RUN_TEST_CASE(IufNonParametricPulse, testIufSetGetNonParametricPulse);
    RUN_TEST_CASE(IufNonParametricPulse, testIufSerialization);
}
