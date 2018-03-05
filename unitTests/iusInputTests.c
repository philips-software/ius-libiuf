#include "unity/unity.h"
#include "iusInput.h"
#include "iusInputFile.h"
#include "iusVector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define Print(text) \
{ \
    printf("\t"); \
    printf(text); \
    printf("\n"); \
}


char* AllocString
(
    char * pStr
)
{
    char * pResult;

    pResult = (char *)calloc( strlen(pStr) + 1, sizeof(char) );
    strcpy( pResult, pStr );
    return pResult;
}

void CreateHeaderStructs
(
    IusExperiment *      pExperiment,
    IusTransducer *      pTransducer,
    IusReceiveSettings * pReceiveSettings,
    IusDrivingScheme *   pDrivingScheme
)
{
    pExperiment->speedOfSound = 1.2f;
    pExperiment->date = 3;
    pExperiment->pDescription = AllocString( "experimentDescription" );

    pTransducer->pTransducerName = AllocString( "transducerName" );
    pTransducer->centerFrequency = 1.2f;
    pTransducer->numElements = 1;
    pTransducer->pElements = (IusTransducerElement *)calloc(1, sizeof(IusTransducerElement));
    pTransducer->pElements[0].position.x = 1.1f;
    pTransducer->pElements[0].position.y = 2.2f;
    pTransducer->pElements[0].position.z = 3.3f;
    pTransducer->pElements[0].angle.theta = 4.4f;
    pTransducer->pElements[0].angle.phi = 5.5f;
    pTransducer->pElements[0].size.x = 6.6f;
    pTransducer->pElements[0].size.y = 7.7f;
    pTransducer->pElements[0].size.z = 8.8f;

    pReceiveSettings->receiveChannelCoding.numChannels = 1; // this * numTransmitPulses = pChannelMap length
    pReceiveSettings->receiveChannelCoding.pChannelMap = (int *)calloc(1, sizeof(int));
    pReceiveSettings->receiveChannelCoding.pChannelMap[0] = 8;
    pReceiveSettings->sampleFrequency = 1.1f;
    pReceiveSettings->pStartDepth = (float *)calloc(1, sizeof(float));
    pReceiveSettings->pStartDepth[0] = 2.2f;
    pReceiveSettings->pEndDepth = (float *)calloc(1, sizeof(float));
    pReceiveSettings->pEndDepth[0] = 3.3f;
    pReceiveSettings->numTimeGainControlValues = 1;
    pReceiveSettings->pTimeGainControl = (IusTimeGainControl *)calloc(1, sizeof(IusTimeGainControl));
    pReceiveSettings->pTimeGainControl[0].time = 4.4f;
    pReceiveSettings->pTimeGainControl[0].gain = 5.5f;

    pDrivingScheme->drivingSchemeType = IUS_FOCUSED_WAVES;
    pDrivingScheme->numSamplesPerLine = 9;
    pDrivingScheme->numTransmitSources = 1; // Determines length of pSourceLocations
    pDrivingScheme->numTransmitPulses = 1; // this * receiveChannelCoding.numChannels = pChannelMap length
    pDrivingScheme->pSourceLocations = (IusPosition *)calloc(1, sizeof(IusPosition));
    pDrivingScheme->pSourceLocations[0].x = 1.1f;
    pDrivingScheme->pSourceLocations[0].y = 2.2f;
    pDrivingScheme->pSourceLocations[0].z = 3.3f;
    pDrivingScheme->sourceFNumber = 0.1f;
    pDrivingScheme->sourceAngularDelta = 0.2f;
    pDrivingScheme->sourceStartAngle = 0.3f;
    pDrivingScheme->transmitPatternDelay = 0.4f;
    pDrivingScheme->pTransmitPattern = (IusTransmitPattern *)calloc(1, sizeof(IusTransmitPattern));
    pDrivingScheme->pTransmitPattern[0].index = 6;
    pDrivingScheme->pTransmitPattern[0].time = 0.5;
    pDrivingScheme->transmitPulse.numPulseValues = 1;
    pDrivingScheme->transmitPulse.pRawPulseAmplitudes = (float *)calloc(1, sizeof(float));
    pDrivingScheme->transmitPulse.pRawPulseAmplitudes[0] = 9.9f;
    pDrivingScheme->transmitPulse.pRawPulseTimes = (float *)calloc(1, sizeof(float));
    pDrivingScheme->transmitPulse.pRawPulseTimes[0] = 8.8f;
    pDrivingScheme->transmitPulse.pulseFrequency = 4.4f;
    pDrivingScheme->transmitPulse.pulseAmplitude = 5.5f;
    pDrivingScheme->transmitPulse.pulseCount = 3;
    pDrivingScheme->pTransmitApodization = (float *)calloc(1, sizeof(float));
    pDrivingScheme->pTransmitApodization[0] = 0.6f;
    pDrivingScheme->transmitChannelCoding.numChannels = 1;
    pDrivingScheme->transmitChannelCoding.pChannelMap = (int *)calloc(1, sizeof(int));
    pDrivingScheme->transmitChannelCoding.pChannelMap[0] = 2;
}


void DestroyHeaderStructs
(
    IusExperiment *      pExperiment,
    IusTransducer *      pTransducer,
    IusReceiveSettings * pReceiveSettings,
    IusDrivingScheme *   pDrivingScheme
)
{
    free( pExperiment->pDescription );
    free( pTransducer->pTransducerName );
    free( pTransducer->pElements );
    free( pReceiveSettings->receiveChannelCoding.pChannelMap );
    free( pReceiveSettings->pStartDepth );
    free( pReceiveSettings->pEndDepth );
    free( pReceiveSettings->pTimeGainControl );
    free( pDrivingScheme->pSourceLocations );
    free( pDrivingScheme->pTransmitPattern );
    free( pDrivingScheme->transmitPulse.pRawPulseAmplitudes );
    free( pDrivingScheme->transmitPulse.pRawPulseTimes );
    free( pDrivingScheme->pTransmitApodization );
    free( pDrivingScheme->transmitChannelCoding.pChannelMap );
}

static float *  allocRFLine
(
    int numSamplesPerLine,
    int numElements
)
{
    return (float *)calloc(numSamplesPerLine * numElements, sizeof(float));
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static float * genRFLine
(
    IusDrivingScheme* drivingScheme,
    IusTransducer* transducer
    
)
{
    float * pPageVector = NULL;
    int     numSamples  = drivingScheme->numSamplesPerLine;
    int     numElements = transducer->numElements;
    int     i = 0;

    // Allocate some memory
    pPageVector = allocRFLine(numSamples,numElements);
    for ( i = 0; i < numSamples * numElements; i++ )
    {
        pPageVector[i] = 0.0f + (float)i;
    }
    return pPageVector;
}

void CreateInputFile
(
    char* filename,
    IusExperiment* experiment,
    IusTransducer* transducer,
    IusReceiveSettings* receiveSettings,
    IusDrivingScheme* drivingScheme,
    float * pPageVector
)
{
    IusInputInstance * pInst;
    IusInputFileInstance * pFile;

    Print("creating iusInput...");
    pInst = iusInputCreate(experiment, transducer, receiveSettings, drivingScheme, 3, 1);
    TEST_ASSERT(pInst != NULL);
    Print("iusInput created.");

    pInst->numFrames = 1;

    Print("creating iusInputFile...");
    pFile = iusInputFileCreate(filename, pInst, 3);
    TEST_ASSERT(pFile != NULL);
    Print("iusInputFile created.");

    Print("adding RF line...");
    iusInputFileWriteNextPulse( pFile, pPageVector );
    Print("RF line added.");

    Print( "closing iusInputFile..." );
    Print( "iusInputFile closed." );

    Print( "destroying iusInput..." );
    iusInputDestroy( pInst );
    Print( "iusInput destroyed." );

    Print( "destroying iusInputFile..." );
    iusInputFileClose( pFile );
    Print( "iusInputFile destroyed." );
}


void AssertExperimentFields(IusExperiment* truth, IusExperiment* validate)
{
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->speedOfSound, validate->speedOfSound, "speedOfSound incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->date, validate->date, "date incorrect.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(truth->pDescription, validate->pDescription, "description incorrect.");
}

void AssertTransducerFields(IusTransducer* truth, IusTransducer* validate)
{
    TEST_ASSERT_EQUAL_STRING_MESSAGE(truth->pTransducerName, validate->pTransducerName, "transducer name incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->centerFrequency, validate->centerFrequency, "centerFrequency incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->numElements, validate->numElements, "numElements incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].position.x , validate->pElements[0].position.x, "position X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].position.y , validate->pElements[0].position.y, "position Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].position.z , validate->pElements[0].position.z, "position Z incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].angle.theta, validate->pElements[0].angle.theta, "angle theta incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].angle.phi, validate->pElements[0].angle.phi, "angle phi incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].size.x, validate->pElements[0].size.x, "size X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].size.y, validate->pElements[0].size.y, "size Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pElements[0].size.z, validate->pElements[0].size.z, "size Z incorrect.");
}

void AssertReceiveSettingsFields(IusReceiveSettings* truth, IusReceiveSettings* validate) 
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->receiveChannelCoding.numChannels, validate->receiveChannelCoding.numChannels, "receiveChannelCoding numChannels incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->receiveChannelCoding.pChannelMap[0], validate->receiveChannelCoding.pChannelMap[0], "channel map incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->sampleFrequency, validate->sampleFrequency, "sampleFrequency incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pStartDepth[0], validate->pStartDepth[0], "startDepth incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pEndDepth[0], validate->pEndDepth[0], "endDepth incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->numTimeGainControlValues, validate->numTimeGainControlValues, "numTimeGainControlValues incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pTimeGainControl[0].time, validate->pTimeGainControl[0].time, "timeGain time incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pTimeGainControl[0].gain, validate->pTimeGainControl[0].gain, "timeGain gain incorrect.");
}

void AssertDrivingSchemeFields(IusDrivingScheme* truth, IusDrivingScheme* validate)
{
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->drivingSchemeType, validate->drivingSchemeType, "drivingSchemeType incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->numSamplesPerLine, validate->numSamplesPerLine, "numSamplesPerLine incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->numTransmitSources, validate->numTransmitSources, "numTransmitSources incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->numTransmitPulses, validate->numTransmitPulses, "numTransmitPulses incorrect.");

    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pSourceLocations[0].x, validate->pSourceLocations[0].x, "sourceLocations X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pSourceLocations[0].y, validate->pSourceLocations[0].y, "sourceLocations Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pSourceLocations[0].z, validate->pSourceLocations[0].z, "sourceLocations Z incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->sourceFNumber, validate->sourceFNumber, "sourceFNumber incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->sourceAngularDelta, validate->sourceAngularDelta, "sourceAngularDelta incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->sourceStartAngle, validate->sourceStartAngle, "sourceStartAngle incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->transmitPatternDelay, validate->transmitPatternDelay, "transmitPatternDelay incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->pTransmitPattern[0].index, validate->pTransmitPattern[0].index, "transmitPattern index incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pTransmitPattern[0].time, validate->pTransmitPattern[0].time, "transmitPattern time incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->transmitPulse.numPulseValues, validate->transmitPulse.numPulseValues, "numPulseValues incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->transmitPulse.pRawPulseAmplitudes[0], validate->transmitPulse.pRawPulseAmplitudes[0], "rawPulseAmplitudes incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->transmitPulse.pRawPulseTimes[0], validate->transmitPulse.pRawPulseTimes[0], "rawPulseTimes incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->transmitPulse.pulseFrequency, validate->transmitPulse.pulseFrequency, "pulseFrequency incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->transmitPulse.pulseAmplitude, validate->transmitPulse.pulseAmplitude, "pulseAmplitude incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->transmitPulse.pulseCount, validate->transmitPulse.pulseCount, "pulseCount incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(truth->pTransmitApodization[0], validate->pTransmitApodization[0], "transmitApodization incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->transmitChannelCoding.numChannels, validate->transmitChannelCoding.numChannels, "transmitChannelCoding numChannels incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(truth->transmitChannelCoding.pChannelMap[0], validate->transmitChannelCoding.pChannelMap[0], "channelMap incorrect.");
}

void AssertData
(
    IusInputFileInstance* pInst,
    float * truth
)
{
    int     numSamples  = pInst->pIusInput->pDrivingScheme->numSamplesPerLine;
    int     numElements = pInst->pIusInput->pTransducer->numElements;
    float   *pPageVector = allocRFLine(numSamples,numElements);
    int     i = 0;
    char    message[256];

    iusInputFileReadNextPulse(pInst, &pPageVector);
    TEST_ASSERT( pPageVector != NULL );

    // Enumerate data
    for ( i = 0; i < numSamples * numElements; i++ )
    {
        sprintf(message, "RF data[%d] not correct", i);
        TEST_ASSERT_EQUAL_INT_MESSAGE(truth[i], pPageVector[i], message);
    }
    return;
}

void ReadInputFile
(
    char* filename,
    IusExperiment* experiment,
    IusTransducer* transducer,
    IusReceiveSettings* receiveSettings,
    IusDrivingScheme* drivingScheme,
    float *pPageVector
)
{
    IusInputFileInstance* pInst;

    Print("opening...");
    pInst = iusInputFileOpen(filename, 0);
    Print("opened.");

    // Check Fields.
    AssertExperimentFields( experiment, pInst->pIusInput->pExperiment );
    AssertTransducerFields( transducer, pInst->pIusInput->pTransducer );
    AssertReceiveSettingsFields( receiveSettings, pInst->pIusInput->pReceiveSettings );
    AssertDrivingSchemeFields( drivingScheme, pInst->pIusInput->pDrivingScheme );

    // Check Data.
    AssertData( pInst, pPageVector );

    Print("deleting data object...");
    iusInputDestroy(pInst->pIusInput);
    Print("delete completed.");

    Print("closing...");
    iusInputFileClose(pInst);
    Print("closed.");
}

void InputFormatTest_Default()
{
    IusExperiment      experiment;
    IusTransducer      transducer;
    IusReceiveSettings receiveSettings;
    IusDrivingScheme   drivingScheme;
    char* filename = "TestInput.input";
    float *pPageVector = NULL;

    Print("Decorating header structs...");
    CreateHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);
    Print("header structs completed.");

    pPageVector = genRFLine(&drivingScheme,&transducer);
    TEST_ASSERT(pPageVector != NULL);

    // Write this data to an input file.
    CreateInputFile(filename, &experiment, &transducer, &receiveSettings, &drivingScheme, pPageVector);

    // Read the data from the input file and check it against these structs.
    ReadInputFile(filename, &experiment, &transducer, &receiveSettings, &drivingScheme, pPageVector);

    Print("deleting test file...");
    // unlink(filename);
    Print("test file deleted.");

    Print("destroying header structs...");
    DestroyHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);
    Print("destroying header completed.");

    free(pPageVector);
}

void InputFormatTest_OneParentNode()
{
}

void iusInputCreateTest() 
{
// Goal test iusInputCreate routine
// 
}

// InputFormatTest
//------------------------------------------------------------------------------

void InputFormatTest()
{
    InputFormatTest_Default();
    InputFormatTest_OneParentNode();
}

