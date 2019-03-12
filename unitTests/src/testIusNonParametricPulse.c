//
// Created by nlv09165 on 01/05/2018.
//
#include <unity.h>
#include <unity_internals.h>
#include <unity_fixture.h>

#include <ius.h>
#include <iusNonParametricPulsePrivate.h>
#include <dg/dataGenerators.h>

static char *pErrorFilename = "IusNonParametricPulse.errlog";
static FILE *fpErrorLogging = NULL;

TEST_GROUP(IusNonParametricPulse);

TEST_SETUP(IusNonParametricPulse)
{
    iusErrorLogClear();
    iusErrorLog(IUS_TRUE);
    iusErrorAutoReportSet(IUS_TRUE);
    fpErrorLogging = fopen(pErrorFilename, "w+");
    iusErrorSetStream(fpErrorLogging);
}

TEST_TEAR_DOWN(IusNonParametricPulse)
{
    iusErrorLogClear();
    iusErrorLog(IUS_FALSE);
    if (fpErrorLogging != NULL)
        fclose(fpErrorLogging);
    fpErrorLogging=stderr;
    iusErrorSetStream(fpErrorLogging);
    remove(pErrorFilename);
}


TEST(IusNonParametricPulse, testIusCreateNonParametricPulse)
{
    int status = 0;
    int     numPulseValues;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iunpp_t nonParametricPulse;

    numPulseValues = 20;
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    TEST_ASSERT_EQUAL(NULL, iusNonParametricPulseCreate(-1));

    TEST_ASSERT_EQUAL(1,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}

TEST(IusNonParametricPulse, testIusDeleteNonParametricPulse)
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
    iusNonParametricPulseCreate(numPulseValues);

    // Parametric transmit pulse
    parametricPulse = iusParametricPulseCreate(pulseFrequency,
                                               pulseAmplitude,
                                               numPulses);

    // Invalid operation on nonparametric dta type
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    status = iusNonParametricPulseDelete((iunpp_t) parametricPulse);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);
    status = iusNonParametricPulseDelete(NULL);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE, status);

    TEST_ASSERT_EQUAL(2,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));

    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}



TEST(IusNonParametricPulse, testIusCompareNonParametricPulse)
{
    int status = 0;
    // Transmit parameters
    float   pulseFrequency=8000000.0f;   /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude=800.0f;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses=10;               /**< number of cycles that the pulse represents */
    int     numPulseValues=20;              /**< number of points to describe waveform, 0 implies a parametric description only */
    iupp_t  parametricPulse;
    iunpp_t nonParametricPulse,notherNonParametricPulse;
    IUS_BOOL isEqual;

    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
    notherNonParametricPulse = iusNonParametricPulseCreate(numPulseValues);

    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);

    isEqual = iusNonParametricPulseCompare(nonParametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusNonParametricPulseCompare(nonParametricPulse, notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);

    // Invalid arguments
    isEqual = iusNonParametricPulseCompare((iunpp_t) parametricPulse, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusNonParametricPulseCompare(NULL, NULL);
    TEST_ASSERT_EQUAL(IUS_TRUE,isEqual);
    isEqual = iusNonParametricPulseCompare(NULL, nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);
    isEqual = iusNonParametricPulseCompare(nonParametricPulse, NULL);
    TEST_ASSERT_EQUAL(IUS_FALSE,isEqual);


    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusNonParametricPulseDelete(notherNonParametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
}


TEST(IusNonParametricPulse, testIusSetGetNonParametricPulse)
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
    nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
    TEST_ASSERT(nonParametricPulse != IUNPP_INVALID);
    TEST_ASSERT_EQUAL(numPulseValues,iusNonParametricPulseGetNumValues(nonParametricPulse));

    status = iusNonParametricPulseSetValue(nonParametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude, iusNonParametricPulseGetValueAmplitude(nonParametricPulse, 0));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iusNonParametricPulseGetValueTime(nonParametricPulse, 0));

    pulseTime *= numPulseValues;
    pulseAmplitude *= numPulseValues;
    status = iusNonParametricPulseSetValue(nonParametricPulse, numPulseValues - 1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    TEST_ASSERT_EQUAL_FLOAT(pulseAmplitude,
                            iusNonParametricPulseGetValueAmplitude(nonParametricPulse, numPulseValues - 1));
    TEST_ASSERT_EQUAL_FLOAT(pulseTime, iusNonParametricPulseGetValueTime(nonParametricPulse, numPulseValues - 1));

    // Invalid params
    // Parametric transmit pulse
    long filePos = ftell(fpErrorLogging);
    TEST_ASSERT_EQUAL(0,iusErrorGetCount());

    parametricPulse = iusParametricPulseCreate(pulseFrequency, pulseAmplitude, numPulses);
    status = iusNonParametricPulseSetValue((iunpp_t) parametricPulse, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusNonParametricPulseSetValue(NULL, 0, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusNonParametricPulseSetValue(nonParametricPulse, 0, -1.0f, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusNonParametricPulseSetValue(nonParametricPulse, -1, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);
    status = iusNonParametricPulseSetValue(nonParametricPulse, numPulseValues, pulseTime, pulseAmplitude);
    TEST_ASSERT_EQUAL(IUS_ERR_VALUE,status);

    TEST_ASSERT_EQUAL(5,iusErrorGetCount());
    TEST_ASSERT_NOT_EQUAL(filePos,ftell(fpErrorLogging));


    status = iusNonParametricPulseDelete(nonParametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
    status = iusParametricPulseDelete(parametricPulse);
    TEST_ASSERT(status == IUS_E_OK);
}


TEST(IusNonParametricPulse, testIusSerialization)
{
    char *filename = "testIusNonParametricPulseSerialization.hdf5";
    int numPulseValues = 20;

    // create
    iunpp_t nonParametricPulse = iusNonParametricPulseCreate(numPulseValues);
    iunpp_t notherNonParametricPulse = iusNonParametricPulseCreate(numPulseValues+10);

    // fill
    int status = iusNonParametricPulseSetValue(nonParametricPulse,0,0.1f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);
    status = iusNonParametricPulseSetValue(nonParametricPulse,1,0.2f,10.0f);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

	// A specific pulse can only be loaded if you know the label in advance! this test needs to be rewritten
    // save
    hid_t handle = H5Fcreate( filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT );
    TEST_ASSERT(handle > 0);
    status = iusNonParametricPulseSave(nonParametricPulse, handle);
    H5Fclose(handle);
    TEST_ASSERT_EQUAL(IUS_E_OK,status);

    // read back
    handle = H5Fopen(filename, H5F_ACC_RDONLY, H5P_DEFAULT );
    iunpp_t savedObj = iusNonParametricPulseLoad(handle);
    TEST_ASSERT(savedObj != NULL);
    H5Fclose(handle);

    TEST_ASSERT_EQUAL(IUS_TRUE, iusNonParametricPulseCompare(nonParametricPulse, savedObj));
    TEST_ASSERT_EQUAL(IUS_FALSE, iusNonParametricPulseCompare(notherNonParametricPulse, savedObj));
	iusNonParametricPulseDelete(savedObj);
 
    iusNonParametricPulseDelete(nonParametricPulse);
    iusNonParametricPulseDelete(notherNonParametricPulse);
    
}

TEST_GROUP_RUNNER(IusNonParametricPulse)
{
    RUN_TEST_CASE(IusNonParametricPulse, testIusCreateNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusDeleteNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusCompareNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusSetGetNonParametricPulse);
    RUN_TEST_CASE(IusNonParametricPulse, testIusSerialization);
}
