#include "unity/unity.h"
#include "iusInput.h"
#include "iusInputFile.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



#define Print(text) \
{ \
	printf("\t"); \
	printf(text); \
	printf("\n"); \
}

char* allocString(char* str)
{
	char* result;

	result = (char*)calloc(strlen(str) + 1, sizeof(char));
	strcpy(result, str);
	
	return result;
}

void CreateHeaderStructs(IusExperiment* experiment, IusTransducer* transducer, IusReceiveSettings* receiveSettings, IusDrivingScheme* drivingScheme)
{
	experiment->speedOfSound = 1.2;
	experiment->date = 3;
	experiment->pDescription = allocString("experimentDescription");
	transducer->pTransducerName = allocString("transducerName");
	transducer->centerFrequency = 1.2;
	transducer->numElements = 1;
	transducer->pElements = (IusTransducerElement*)calloc(1, sizeof(IusTransducerElement));
	transducer->pElements[0].position.x = 1.1;
	transducer->pElements[0].position.y = 2.2;
	transducer->pElements[0].position.z = 3.3;
	transducer->pElements[0].angle.theta = 4.4;
	transducer->pElements[0].angle.phi = 5.5;
	transducer->pElements[0].size.x = 6.6;
	transducer->pElements[0].size.y = 7.7;
	transducer->pElements[0].size.z = 8.8;

	receiveSettings->receiveChannelCoding.numChannels = 9;
	receiveSettings->receiveChannelCoding.pChannelMap = (int*)calloc(1, sizeof(int));
	receiveSettings->receiveChannelCoding.pChannelMap[0] = 8;
	receiveSettings->sampleFrequency = 1.1;
	receiveSettings->pStartDepth = (float*)calloc(1, sizeof(float));
	receiveSettings->pStartDepth[0] = 2.2;
	receiveSettings->pEndDepth = (float*)calloc(1, sizeof(float));
	receiveSettings->pEndDepth[0] = 3.3;
	receiveSettings->numTimeGainControlValues = 1;
	receiveSettings->pTimeGainControl = (IusTimeGainControl*)calloc(1, sizeof(IusTimeGainControl));
	receiveSettings->pTimeGainControl[0].time = 4.4;
	receiveSettings->pTimeGainControl[0].gain = 5.5;

	drivingScheme->drivingSchemeType = IUS_FOCUSED_WAVES;
	drivingScheme->numSamplesPerLine = 9;
	drivingScheme->numTransmitSources = 8;
	drivingScheme->numTransmitPulses = 7;
	drivingScheme->pSourceLocations = (IusPosition*)calloc(1, sizeof(IusPosition));
	drivingScheme->pSourceLocations[0].x = 1.1;
	drivingScheme->pSourceLocations[0].y = 2.2;
	drivingScheme->pSourceLocations[0].z = 3.3;
	drivingScheme->sourceFNumber = 0.1;
	drivingScheme->sourceAngularDelta = 0.2;
	drivingScheme->sourceStartAngle = 0.3;
	drivingScheme->transmitPatternDelay = 0.4;
	drivingScheme->pTransmitPattern = (IusTransmitPattern*)calloc(1, sizeof(IusTransmitPattern));
	drivingScheme->pTransmitPattern[0].index = 6;
	drivingScheme->pTransmitPattern[0].time = 0.5;
	drivingScheme->transmitPulse.numPulseValues = 1;
	drivingScheme->transmitPulse.pRawPulseAmplitudes = (float*)calloc(1, sizeof(float));
	drivingScheme->transmitPulse.pRawPulseAmplitudes[0] = 9.9;
	drivingScheme->transmitPulse.pRawPulseTimes = (float*)calloc(1, sizeof(float));
	drivingScheme->transmitPulse.pRawPulseTimes[0] = 8.8;
	drivingScheme->transmitPulse.pulseFrequency = 4.4;
	drivingScheme->transmitPulse.pulseAmplitude = 5.5;
	drivingScheme->transmitPulse.pulseCount = 3;
	drivingScheme->pTransmitApodization = (float*)calloc(1, sizeof(float));
	drivingScheme->pTransmitApodization[0] = 0.6;
	drivingScheme->transmitChannelCoding.numChannels = 1;
	drivingScheme->transmitChannelCoding.pChannelMap = (int*)calloc(1, sizeof(int));
	drivingScheme->transmitChannelCoding.pChannelMap[0] = 2;
}

void DestroyHeaderStructs(IusExperiment* experiment, IusTransducer* transducer, IusReceiveSettings* receiveSettings, IusDrivingScheme* drivingScheme)
{
	free(experiment->pDescription);
	free(transducer->pTransducerName);
	free(transducer->pElements);
	free(receiveSettings->receiveChannelCoding.pChannelMap);
	free(receiveSettings->pStartDepth);
	free(receiveSettings->pEndDepth);
	free(receiveSettings->pTimeGainControl);
	free(drivingScheme->pSourceLocations);
	free(drivingScheme->pTransmitPattern);
	free(drivingScheme->transmitPulse.pRawPulseAmplitudes);
	free(drivingScheme->transmitPulse.pRawPulseTimes);
	free(drivingScheme->pTransmitApodization);
	free(drivingScheme->transmitChannelCoding.pChannelMap);
}

int InputFormatTest()
{
	IusExperiment experiment;
	IusTransducer transducer;
	IusReceiveSettings receiveSettings;
	IusDrivingScheme drivingScheme;

	IusInputInstance* pInst;
	IusInputFileInstance* pFile;
	int result;

	Print("Decorating header structs...");
	CreateHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);	
	Print("header structs completed.");

	Print("creating iusInput...");
	pInst = iusInputCreate(&experiment, &transducer, &receiveSettings, &drivingScheme, 3, 1);
	Print("iusInput created.");

	Print("destroying header structs...");
	DestroyHeaderStructs(&experiment, &transducer, &receiveSettings, &drivingScheme);
	Print("destroying header completed.");

	Print("creating iusInputFile...");
//	pFile = iusInputFileCreate("TestInput.input", pInst, 3);
	Print("iusInputFile created.");

	// Todo: write frames?

	Print("closing iusInputFile...");
//	result = iusInputFileClose(pFile);
//	printf("close return value: %d\n", result);
	Print("iusInputFile closed.");




	Print("destroying iusInput...");
	iusInputDestroy(pInst);
	Print("iusInput destroyed.");

	Print("deleting test file...");
//	unlink("TestInput.input");
	Print("test file deleted.");

	return 1;	
}


// InputTest
//------------------------------------------------------------------------------
void InputTest(void)
{
    int retVal = InputFormatTest();
    TEST_ASSERT( 0 );
}
