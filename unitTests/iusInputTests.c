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

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static void genRFLine
(
    IusInputFileInstance * pInputFile0
)
{
    float * pPageVector = NULL;
    int     numSamples  = pInputFile0->pIusInput->pDrivingScheme->numSamplesPerLine;
    int     numElements = pInputFile0->pIusInput->pTransducer->numElements;
    int     i = 0;

    // Allocate some memory
    pPageVector = (float *)calloc(numSamples * numElements, sizeof(float));
    
    for ( i = 0; i < numSamples * numElements; i++ )
    {
        pPageVector[i] = 0.0f + (float)i;
    }

    iusInputFileWriteNextPulse( pInputFile0, pPageVector );

    free(pPageVector);
}

void CreateInputFile(
    char* filename,
    IusExperiment* experiment,
    IusTransducer* transducer,
    IusReceiveSettings* receiveSettings,
    IusDrivingScheme* drivingScheme)
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

//    Print("adding RF line...");
//    genRFLine(pFile);
//    Print("RF line added.");

    Print( "closing iusInputFile..." );
    Print( "iusInputFile closed." );

    Print( "destroying iusInput..." );
    iusInputDestroy( pInst );
    Print( "iusInput destroyed." );

    Print( "destroying iusInputFile..." );
    iusInputFileClose( pFile );
    Print( "iusInputFile destroyed." );
}

void AssertAllFields(IusInputInstance* pInst,
    IusExperiment* experiment,
    IusTransducer* transducer,
    IusReceiveSettings* receiveSettings,
    IusDrivingScheme* drivingScheme)
{
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(experiment->speedOfSound, pInst->pExperiment->speedOfSound, "speedOfSound incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(experiment->date, pInst->pExperiment->date, "date incorrect.");
    TEST_ASSERT_EQUAL_STRING_MESSAGE(experiment->pDescription, pInst->pExperiment->pDescription, "description incorrect.");

    TEST_ASSERT_EQUAL_STRING_MESSAGE(transducer->pTransducerName, pInst->pTransducer->pTransducerName, "transducer name incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->centerFrequency, pInst->pTransducer->centerFrequency, "centerFrequency incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(transducer->numElements, pInst->pTransducer->numElements, "numElements incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].position.x , pInst->pTransducer->pElements[0].position.x, "position X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].position.y , pInst->pTransducer->pElements[0].position.y, "position Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].position.z , pInst->pTransducer->pElements[0].position.z, "position Z incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].angle.theta, pInst->pTransducer->pElements[0].angle.theta, "angle theta incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].angle.phi, pInst->pTransducer->pElements[0].angle.phi, "angle phi incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].size.x, pInst->pTransducer->pElements[0].size.x, "size X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].size.y, pInst->pTransducer->pElements[0].size.y, "size Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(transducer->pElements[0].size.z, pInst->pTransducer->pElements[0].size.z, "size Z incorrect.");

    TEST_ASSERT_EQUAL_INT_MESSAGE(receiveSettings->receiveChannelCoding.numChannels, pInst->pReceiveSettings->receiveChannelCoding.numChannels, "receiveChannelCoding numChannels incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(receiveSettings->receiveChannelCoding.pChannelMap[0], pInst->pReceiveSettings->receiveChannelCoding.pChannelMap[0], "channel map incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(receiveSettings->sampleFrequency, pInst->pReceiveSettings->sampleFrequency, "sampleFrequency incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(receiveSettings->pStartDepth[0], pInst->pReceiveSettings->pStartDepth[0], "startDepth incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(receiveSettings->pEndDepth[0], pInst->pReceiveSettings->pEndDepth[0], "endDepth incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(receiveSettings->numTimeGainControlValues, pInst->pReceiveSettings->numTimeGainControlValues, "numTimeGainControlValues incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(receiveSettings->pTimeGainControl[0].time, pInst->pReceiveSettings->pTimeGainControl[0].time, "timeGain time incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(receiveSettings->pTimeGainControl[0].gain, pInst->pReceiveSettings->pTimeGainControl[0].gain, "timeGain gain incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->drivingSchemeType, pInst->pDrivingScheme->drivingSchemeType, "drivingSchemeType incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->numSamplesPerLine, pInst->pDrivingScheme->numSamplesPerLine, "numSamplesPerLine incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->numTransmitSources, pInst->pDrivingScheme->numTransmitSources, "numTransmitSources incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->numTransmitPulses, pInst->pDrivingScheme->numTransmitPulses, "numTransmitPulses incorrect.");

    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->pSourceLocations[0].x, pInst->pDrivingScheme->pSourceLocations[0].x, "sourceLocations X incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->pSourceLocations[0].y, pInst->pDrivingScheme->pSourceLocations[0].y, "sourceLocations Y incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->pSourceLocations[0].z, pInst->pDrivingScheme->pSourceLocations[0].z, "sourceLocations Z incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->sourceFNumber, pInst->pDrivingScheme->sourceFNumber, "sourceFNumber incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->sourceAngularDelta, pInst->pDrivingScheme->sourceAngularDelta, "sourceAngularDelta incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->sourceStartAngle, pInst->pDrivingScheme->sourceStartAngle, "sourceStartAngle incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->transmitPatternDelay, pInst->pDrivingScheme->transmitPatternDelay, "transmitPatternDelay incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->pTransmitPattern[0].index, pInst->pDrivingScheme->pTransmitPattern[0].index, "transmitPattern index incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->pTransmitPattern[0].time, pInst->pDrivingScheme->pTransmitPattern[0].time, "transmitPattern time incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->transmitPulse.numPulseValues, pInst->pDrivingScheme->transmitPulse.numPulseValues, "numPulseValues incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->transmitPulse.pRawPulseAmplitudes[0], pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes[0], "rawPulseAmplitudes incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->transmitPulse.pRawPulseTimes[0], pInst->pDrivingScheme->transmitPulse.pRawPulseTimes[0], "rawPulseTimes incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->transmitPulse.pulseFrequency, pInst->pDrivingScheme->transmitPulse.pulseFrequency, "pulseFrequency incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->transmitPulse.pulseAmplitude, pInst->pDrivingScheme->transmitPulse.pulseAmplitude, "pulseAmplitude incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->transmitPulse.pulseCount, pInst->pDrivingScheme->transmitPulse.pulseCount, "pulseCount incorrect.");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(drivingScheme->pTransmitApodization[0], pInst->pDrivingScheme->pTransmitApodization[0], "transmitApodization incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->transmitChannelCoding.numChannels, pInst->pDrivingScheme->transmitChannelCoding.numChannels, "transmitChannelCoding numChannels incorrect.");
    TEST_ASSERT_EQUAL_INT_MESSAGE(drivingScheme->transmitChannelCoding.pChannelMap[0], pInst->pDrivingScheme->transmitChannelCoding.pChannelMap[0], "channelMap incorrect.");
}

void ReadInputFile(
    char* filename,
    IusExperiment* experiment,
    IusTransducer* transducer,
    IusReceiveSettings* receiveSettings,
    IusDrivingScheme* drivingScheme)
{
    IusInputFileInstance* pInst;

    Print("opening...");
    pInst = iusInputFileOpen(filename, 0);
    Print("opened.");

    // Check data.
    AssertAllFields(pInst->pIusInput, experiment, transducer, receiveSettings, drivingScheme);

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

    Print("Decorating header structs...");
    CreateHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);
    Print("header structs completed.");

    // Write this data to an input file.
    CreateInputFile(filename, &experiment, &transducer, &receiveSettings, &drivingScheme);

    // Read the data from the input file and check it against these structs.
    ReadInputFile(filename, &experiment, &transducer, &receiveSettings, &drivingScheme);

    Print("deleting test file...");
    unlink(filename);
    Print("test file deleted.");

    Print("destroying header structs...");
    DestroyHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);
    Print("destroying header completed.");
}

void InputFormatTest_OneParentNode()
{
}

// InputFormatTest
//------------------------------------------------------------------------------

void InputFormatTest()
{
    InputFormatTest_Default();
    InputFormatTest_OneParentNode();
}

