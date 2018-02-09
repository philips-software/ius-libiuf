#include "unity/unity.h"
#include "iusInput.h"
#include "iusInputFile.h"
#include "iusVector.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


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

    pReceiveSettings->receiveChannelCoding.numChannels = 9;
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
    pDrivingScheme->numTransmitSources = 8;
    pDrivingScheme->numTransmitPulses = 7;
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
    int     i = 0 ;
    // Allocate some memory
    pPageVector = iusAllocFloatVector( numSamples );
    for ( i = 0; i < numSamples; i++ )
    {
        pPageVector[i] = 0.0f + (float)i;
    }

    iusInputFileWriteNextPulse( pInputFile0, pPageVector );
    iusFreeFloatVector( pPageVector );
}

int InputFormatTest()
{
    IusExperiment      experiment;
    IusTransducer      transducer;
    IusReceiveSettings receiveSettings;
    IusDrivingScheme   drivingScheme;

    IusInputInstance * pInst;
    IusInputFileInstance * pFile;

    Print( "Decorating header structs..." );
    CreateHeaderStructs( &experiment, &transducer, &receiveSettings, &drivingScheme );    
    Print( "header structs completed." );

    Print( "creating iusInput..." );
    pInst = iusInputCreate( &experiment, &transducer, &receiveSettings, &drivingScheme, 3, 1 );
    TEST_ASSERT(pInst != NULL);
    Print( "iusInput created." );

    //Print( "destroying header structs..." );
    //DestroyHeaderStructs( &experiment, &transducer, &receiveSettings, &drivingScheme );
    //Print( "destroying header completed." );

    Print( "creating iusInputFile..." );
    pFile = iusInputFileCreate( "TestInput.input", pInst, 3 );
    TEST_ASSERT(pFile != NULL);
    Print( "iusInputFile created." );

    // Todo: write frames?
    genRFLine(pFile);

    Print( "closing iusInputFile..." );
    Print( "iusInputFile closed." );

    Print( "destroying iusInput..." );
    iusInputDestroy( pInst );
    Print( "iusInput destroyed." );

    Print( "deleting test file..." );
//    unlink( "TestInput.input" );
    Print( "test file deleted." );

    return 0;
}


// InputTest
//------------------------------------------------------------------------------
int InputTest(void)
{
    int retVal = InputFormatTest();
    TEST_ASSERT( 0 == retVal );

    return retVal;
}
