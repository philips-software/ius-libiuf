//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWC4DWDTest.c
//                  tests the CWC algorithm
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusRFFile.h"
#include "iusInputFile.h"
#include "iusMatrix.h"
#include "iusBasicFunctions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#ifdef _WIN32
#define LD_INPUTFILE   "\\\\code1\\storage\\2016_ius_ux\\processing\\regression\\Vera2Input\\example.mat"
#define LD_OUTPUTFILE  "\\\\code1\\storage\\2016_ius_ux\\processing\\regression\\Vera2Input\\example_result"
#define LD_REFOUTFILE  "\\\\code1\\storage\\2016_ius_ux\\processing\\regression\\Vera2Input\\example_ref.input"

//#define LD_INPUTFILE   "D:\\testData\\CWC4DWD\\synth_mpointsMovingDriving1.input"
//#define LD_OUTPUTFILE  "D:\\testData\\CWC4DWD\\synth_mpointsMovingDriving1_1"
#else
// #define LD_INPUTFILE   "/home/2016_ius/INPUT/regression/Vera2Input/example.mat"
// #define LD_LOGFILE   "/home/2016_ius/INPUT/regression/Vera2Input/example.log"
// #define LD_OUTPUTFILE  "/home/2016_ius/INPUT/regression/Vera2Input/example_result"
// #define LD_REFOUTFILE  "/home/2016_ius/INPUT/regression/Vera2Input/example_ref.input"
#define LD_INPUTFILE  "/Users/nlv09165/proj/PPF/ius/INPUT/regression/Vera2Input/example.mat"
#define LD_LOGFILE    "/Users/nlv09165/proj/PPF/ius/INPUT/regression/Vera2Input/example.log"
#define LD_OUTPUTFILE "/Users/nlv09165/proj/PPF/ius/INPUT/regression/Vera2Input/example_result"
#define LD_REFOUTFILE "/Users/nlv09165/proj/PPF/ius/INPUT/regression/Vera2Input/example_ref.input" 
#endif

#define VPRINT if( verbose) fprintf

/* Extracts the date, description of the experiment and the transducer that was used from the log file */
static int LF_parseLogFile(char *pLogFileName, char **pDateString, char **pDescription, char **pTransducerName)
{
  int success;
  FILE *logFile;
  int nameOffset= 23; /* description and transducer name start at character 23 of a line*/
  ssize_t read;
  int len=0;
  char *line=NULL;
  const size_t line_size = 300;
  const char delimiters[] = " .,;:!";
  char *date;
  char *token;
  char *slashPos;
    
  *pDateString = (char *)calloc(9,sizeof(char));
  date = *pDateString;

  logFile = fopen(pLogFileName, "r");
  if (logFile == NULL)
  {
    fprintf(stderr, "Error: could not open Log file %s\n", pLogFileName);
    return 1;
  }

  line = malloc(line_size);

  fgets(line, line_size, logFile); //get first line
  fgets(line, line_size, logFile); //get second line
  token = strtok(line+nameOffset, delimiters); /* second line contains the transducer */
  *pTransducerName = (char *)calloc(strlen(token)+1, sizeof(char));
  strcpy(*pTransducerName, token);  
  fprintf(stdout, "LF_parseLogFile TransducerName: %s\n", *pTransducerName);
  fgets(line, line_size, logFile); //get third line

  fgets(line, line_size, logFile); //get fourth line
  *pDescription = (char *)calloc(strlen(line+nameOffset)+1,sizeof(char));
  strcpy(*pDescription, line+nameOffset);                  /* fourth line contains the description */
  fprintf(stdout, "LF_parseLogFile description: %s\n", *pDescription);
  fgets(line, line_size, logFile); //get fifth line

  fgets(line, line_size, logFile); //get sixth line
  slashPos = strtok(line, "\\");
  while (slashPos != NULL) 
  {
    strncpy(date, slashPos, 8);                   /* date starts after last \ in sixth line */
    slashPos = strtok(NULL, "\\");
  }
  fprintf(stdout, "LF_parseLogFile date: %s\n", date);

  
  free(line);
  return 0;
}

static void LF_copyNextEvent(IUSRFFileInstance *pRfFile, IusInputFileInstance *pInst)
{
	int status;
	float **pPage;
	float *pPageVector;
	int row,col, i=0;
	int numSamples  = pInst->pIusInput->pDrivingScheme->numSamplesPerLine;
	int numElements = pInst->pIusInput->pTransducer->numElements;

	// Allocate some memory
	pPageVector = (float *)calloc(numSamples*numElements, sizeof(float));
	pPage = (float **)iusAllocFloatMatrix(numElements, numSamples);
	assert(pPageVector != NULL);
	assert(pPage != NULL);
	assert(pPage[0] != pPageVector);

	status = iusRFFileRead(pRfFile, pPage);
        /* This copy might no longer be needed */
	for (col=0; col<numElements ; col++)
	{
          for (row=0; row<numSamples; row++)
          {
   	    // pPageVector[i++] = pPage[][row];
   	    pPageVector[i++] = pPage[col][row];
          }
	}
	iusInputFileWriteNextPulse(pInst, pPageVector);

	iusFreeFloatMatrix(pPage, pInst->pIusInput->pTransducer->numElements);
	free(pPageVector);
}

static int LF_readRFFile(IUSRFFileInstance **pRfFile,  char *filename, int verbose)
{
  /*------------------------------------------------------------------------*/
  /* open input file (creates the memory for the IUSRFFileInstance)         */
  /*------------------------------------------------------------------------*/
  *pRfFile = iusRFFileOpen( filename, verbose );
  if( *pRfFile == NULL )
  {
    fprintf( stderr, "%s: iusRFFileOpen failed\n", filename);
    return 1;
  }
  return 0;
}
    
/* creates the experiment, transducer, receive settings and driving scheme configuration from the data in the RF file instance 
 * and extra (missing data) parameters
 * */
static int LF_convertRFF2Input( IusInputFileInstance **ppInputFile, 
		    	       char *pFileName,
                               IUSRFFileInstance *pRfFile, 
                               int date,                                      /* missing in RF file */
                               char *pDescription,                            /* missing in RF file */
                               char *pTransducerName,                         /* missing in RF file */
                               enum IusDrivingSchemeType drivingSchemeType,   /* missing in RF File */
                               int verbose)
{
  int i;
  char statusDescription[200]; //long enough for a short message
  IusExperiment        *pExperiment;
  IusTransducer        *pTransducer;
  IusReceiveSettings   *pReceiveSettings;
  IusDrivingScheme     *pDrivingScheme;
  IusInputInstance     * pInst;

  pExperiment = (IusExperiment *)calloc(1,sizeof(IusExperiment));
  pExperiment->speedOfSound = pRfFile->par.speedOfSound;
  pExperiment->date = date;
  pExperiment->pDescription = pDescription;

  pTransducer = (IusTransducer *)calloc(1,sizeof(IusTransducer));
  pTransducer->pTransducerName = pTransducerName;
  pTransducer->centerFrequency = pRfFile->Fc;
  pTransducer->numElements = pRfFile->nrRxElements;
  pTransducer->pElements = (IusTransducerElement *)calloc(pTransducer->numElements, sizeof(IusTransducerElement));
  for (i=0; i<pTransducer->numElements; i++)
  {
    pTransducer->pElements[i].position.x = pRfFile->Trans.ElementPos[0][i]/1000.0f; // mm to meters
    pTransducer->pElements[i].position.y = pRfFile->Trans.ElementPos[1][i]/1000.0f; // mm to meters
    pTransducer->pElements[i].position.z = pRfFile->Trans.ElementPos[2][i]/1000.0f; // mm to meters
    pTransducer->pElements[i].angle.theta = 0.0;
    pTransducer->pElements[i].angle.phi = 0.0;
    pTransducer->pElements[i].size.x = fabs(pRfFile->Trans.ElementPos[0][1]/1000.0f - pRfFile->Trans.ElementPos[0][0]/1000.0f);
    pTransducer->pElements[i].size.y = 0.0;
    pTransducer->pElements[i].size.z = 0.0;
  }

  pReceiveSettings = (IusReceiveSettings *)calloc(1,sizeof(IusReceiveSettings));
  pReceiveSettings->sampleFrequency = pRfFile->par.Fs;
  pReceiveSettings->numTimeGainControlValues = 1;              /* receiveTGC currently not yet implemented */
  pReceiveSettings->pTimeGainControl = (IusTimeGainControl *)calloc(pReceiveSettings->numTimeGainControlValues, sizeof(IusTimeGainControl));
  for (i=0; i<pReceiveSettings->numTimeGainControlValues; i++) /* not yet implemented*/
  {
    pReceiveSettings->pTimeGainControl[i].time = 0.0;
    pReceiveSettings->pTimeGainControl[i].gain = 1.0;      
  }
  pReceiveSettings->pStartDepth = (float *)calloc(pRfFile->nrTxEvents, sizeof(float));
  pReceiveSettings->pEndDepth   = (float *)calloc(pRfFile->nrTxEvents, sizeof(float));
  for (i=0; i<pRfFile->nrTxEvents; i++)
  {
    pReceiveSettings->pStartDepth[i]=0.0; //TODO: extract this data from the RFFile
    pReceiveSettings->pEndDepth[i]= pReceiveSettings->pStartDepth[i] + (pRfFile->lineLength/pRfFile->par.Fs);
  }

  pDrivingScheme = (IusDrivingScheme *)malloc(sizeof(IusDrivingScheme));
  pDrivingScheme->numSamplesPerLine = pRfFile->lineLength; 
  assert(pRfFile->par.ensembleSize>0);
  pDrivingScheme->numTransmitSources = pRfFile->nrTxEvents/pRfFile->par.ensembleSize;
  pDrivingScheme->numTransmitPulses = pRfFile->nrTxEvents;
  pDrivingScheme->drivingSchemeType = drivingSchemeType;
  pDrivingScheme->pTransmitPattern = (IusTransmitPattern *)calloc(pDrivingScheme->numTransmitPulses, sizeof(IusTransmitPattern));
  pDrivingScheme->transmitPatternDelay = 1.0f / (float)pRfFile->par.waveRate; 

  if (drivingSchemeType ==  IUS_DIVERGING_WAVES)
  {
    float D; 
    float deltaSign;
    float minTheta=(float)(2.0*IUS_PI), maxTheta=0.0f; // theta should always have a range between pi/2 and 3*pi/2 rad
    float time=0.0f;
    pDrivingScheme->pSourceLocations      = NULL ;
    D = pTransducer->pElements[pTransducer->numElements-1].position.x - pTransducer->pElements[0].position.x;
    pDrivingScheme->sourceFNumber = pRfFile->par.txFNumber; 
    if (pDrivingScheme->numTransmitSources > 1)
    {
      for (i = 0; i < pDrivingScheme->numTransmitPulses; i++)
      {
        minTheta = (float)fmin(pRfFile->par.theta[i] + IUS_PI, minTheta);
        maxTheta = (float)fmax(pRfFile->par.theta[i] + IUS_PI, maxTheta);
        pDrivingScheme->pTransmitPattern[i].index = i / pRfFile->par.ensembleSize;
        pDrivingScheme->pTransmitPattern[i].time = time;
        time += 1.0f / (float)pRfFile->par.waveRate;
      }

      if (pRfFile->par.theta[0] < pRfFile->par.theta[pDrivingScheme->numTransmitPulses-1]) 
      {
        deltaSign = 1.0;
      }
      else 
      {
        deltaSign = -1.0;
      }
      pDrivingScheme->sourceAngularDelta = deltaSign * (maxTheta - minTheta) / (pDrivingScheme->numTransmitSources - 1);
      pDrivingScheme->sourceStartAngle = pDrivingScheme->sourceAngularDelta>0? minTheta: maxTheta;
    }
    else
    {
      pDrivingScheme->sourceAngularDelta = 0.0f; 
      pDrivingScheme->sourceStartAngle = pRfFile->par.theta[0]+IUS_PI;
    }
  }
  else if (drivingSchemeType == IUS_FOCUSED_WAVES)
  {
    pDrivingScheme->pSourceLocations       = NULL; /* to be defined */
    pDrivingScheme->sourceFNumber  = 0.0;  /* not used for this scheme */
    pDrivingScheme->sourceAngularDelta    = 0.0;
    pDrivingScheme->sourceStartAngle      = pRfFile->par.theta[0] + IUS_PI;
  }
  else if (drivingSchemeType ==  IUS_PLANE_WAVES)
  {
  }
  else
  {
  }
  /* sane data to be added */
  pDrivingScheme->transmitPulse.numPulseValues = 0;
  pDrivingScheme->transmitPulse.pulseAmplitude = pRfFile->par.tPCHighVoltage;
  pDrivingScheme->transmitPulse.pulseCount = pRfFile->par.nrPulseCycles;
  pDrivingScheme->transmitPulse.pulseFrequency = pRfFile->par.pulseFreqMHz * 1000000.0f;
  pDrivingScheme->transmitPulse.pRawPulseAmplitudes = NULL;
  pDrivingScheme->transmitPulse.pRawPulseTimes = NULL;
  
  pDrivingScheme->pTransmitApodization = (float *)calloc(pDrivingScheme->numTransmitPulses*pTransducer->numElements, sizeof(float));
  fprintf(stderr, "Apodization\n");
  /* transmit apodization to be filled in */
  for (i=0; i< pDrivingScheme->numTransmitPulses*pTransducer->numElements; i++)
  {
    pDrivingScheme->pTransmitApodization[i] = 1.0; 
  }

  pInst = iusInputCreate(pExperiment, pTransducer, pReceiveSettings, pDrivingScheme, 3, pRfFile->nrFrames);
  *ppInputFile = iusInputFileCreate(pFileName, pInst, 3);
  
  /* ************************** overrule nrFrames */
  pRfFile->nrFrames = 2;
  
  fprintf(stderr, "copying %d x %d pulses\n", pRfFile->nrFrames, pDrivingScheme->numTransmitPulses);
  for (i = 0; i < pRfFile->nrFrames*pDrivingScheme->numTransmitPulses; i++)
  {
    LF_copyNextEvent(pRfFile, *ppInputFile);
    if ((i%pDrivingScheme->numTransmitPulses) == 0)
    {
      sprintf(statusDescription, "Processing Frame %d/%d\n",i/pDrivingScheme->numTransmitPulses+1, pRfFile->nrFrames);
    }
    fprintf(stderr, "Processing %d/%d\n",i/pDrivingScheme->numTransmitPulses+1, pRfFile->nrFrames);
  }

  if (iusInputFileClose(*ppInputFile))
  {
    fprintf(stderr, "Error cleaning up inputFileInstance sub structures\n");
    exit(1);
  }

  //free(pExperiment->description);    done by iusInputFileClose
  //free(pTransducer->transducerName); done by iusInputFileClose

  free(pExperiment);
  free(pTransducer);
  free(pReceiveSettings);
  free(pDrivingScheme);

  return 0;
}


//-----------------------------------------------------------------------------
// main
//-----------------------------------------------------------------------------
//int ConvertVerasonicsToInput(SetValueCallback svc, char* pInputFn, char* pLogFileName, char* pOutputFolder, int verbose)
int main(int argc, char * argv[])
{
    int verbose =1;
    IUSRFFileInstance    *pRfFile;
    IusInputFileInstance *pInputFile;
    char                 *pOutputFileName = argv[1];
    char                 *pInputFn = argv[2];
    char                 *pLogFileName = argv[3];
    int                  result;
    char                 *pDateString;
    char                 *pDescription;
    char                 *pTransducerName;

    //pOutputFileName = (char *)calloc(strlen(pOutputFolder)+7,sizeof(char));
    //sprintf(pOutputFileName, "%s.input", pOutputFolder);

    fprintf(stderr, "pOutputFileName %s\n", pOutputFileName);
    fprintf(stderr, "pInputFileName %s\n",  pInputFn);
    result = LF_readRFFile(&pRfFile, pInputFn, verbose); 
    if (result != 0)
    {
      fprintf(stderr, "Error reading RFFile\n");
      return -1;
    }
    result = LF_parseLogFile(pLogFileName, &pDateString, &pDescription, &pTransducerName);
    if (result != 0)
    {
      fprintf(stderr, "Error parsing log file\n");
      return -1;
    }
    result = LF_convertRFF2Input(&pInputFile, 
                                 pOutputFileName, 
                                 pRfFile,
                                 atoi(pDateString), 
                                 pDescription,
                                 pTransducerName,
                                 IUS_DIVERGING_WAVES, 
                                 verbose);
    if (result != 0)
    {
      fprintf(stderr, "Error converting RFF to Input format\n");
      return -1;
    }
    
    /*------------------------------------------------------------------------*/
    /* done                                                                   */
    /*------------------------------------------------------------------------*/
    free(pDateString);
    iusRFFileClose(pRfFile);

    //free(pInputFile);
    //free(pOutputFileName);
    return result;
}





