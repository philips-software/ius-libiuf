//==============================================================================
//    COPYRIGHT 2015 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusInput.c
//                  Input data structure.
//  iusVersion    : 255.255.255.255
//
//==============================================================================

#include "iusInput.h"
#include "iusError.h"
#include "iusHDF5.h"
#include "iusUuid.h"
#include <stdlib.h>
#include <string.h>
#include <H5LTpublic.h>


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_copyExperimentData
(
    IusExperiment * pDst,
    IusExperiment * pSrc
)
{
    // speed of sound in m/s
    // int concatenation of <year><month><day> e.g. 20160123 for 23th Jan 2016
    pDst->speedOfSound = pSrc->speedOfSound;
    pDst->date         = pSrc->date;

    pDst->pDescription  =
        (char *)calloc( strlen( pSrc->pDescription ) + 1, sizeof( char ) );
    if ( pDst->pDescription == NULL )
    {
        return 1;
    }
    strcpy( pDst->pDescription, pSrc->pDescription );

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_copyTransducerData
(
    IusTransducer * pDst,
    IusTransducer * pSrc
)
{
    // transducerName is a descriptive name of the ultrasound probe
    pDst->pTransducerName =
        (char *)calloc( strlen(pSrc->pTransducerName) + 1, sizeof(char) );

    if (pDst->pTransducerName == NULL)
    {
        return 1 << 2;
    }
    strcpy( pDst->pTransducerName, pSrc->pTransducerName );

    // centerFrequency: operating frequency of the transducer in Hz
    // numElements    : number of transducer Elements in the probe
    pDst->centerFrequency = pSrc->centerFrequency;
    pDst->numElements     = pSrc->numElements;

    pDst->pElements = (IusTransducerElement *)calloc( pDst->numElements,
        sizeof( IusTransducerElement ) );

    if ( pDst->pElements == NULL )
    {
        return 2 << 2;
    }

    // an array of numElements transducer element (position, angle, size)
    memcpy( pDst->pElements, pSrc->pElements,
        pDst->numElements * sizeof( IusTransducerElement ) );

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_copyReceiveSettingsData
(
    IusReceiveSettings * pDst,
    IusReceiveSettings * pSrc,
    int numPulsesPerFrame
)
{
    IUS_ASSERT( numPulsesPerFrame > 0 );

    pDst->receiveChannelCoding.numChannels =
        pSrc->receiveChannelCoding.numChannels;
    if ( pDst->receiveChannelCoding.numChannels != 0 ) // we use channel coding
    {
        int numChannelMapIndices =
            pDst->receiveChannelCoding.numChannels * numPulsesPerFrame;

        pDst->receiveChannelCoding.pChannelMap =
            (int *)calloc( numChannelMapIndices, sizeof(int) );

        if ( pDst->receiveChannelCoding.pChannelMap == NULL )
        {
            return 1;
        }

        memcpy( pDst->receiveChannelCoding.pChannelMap,
                pSrc->receiveChannelCoding.pChannelMap,
                numChannelMapIndices * sizeof(int) );
    }
    else
    {
        // explicitly set the channelMap to NULL
        pDst->receiveChannelCoding.pChannelMap = NULL;
    }

    pDst->sampleFrequency = pSrc->sampleFrequency;                                   /* The sampling frequency that was used */

    pDst->pStartDepth      = (float *)calloc( numPulsesPerFrame, sizeof(float) );
    if ( pDst->pStartDepth == NULL )
    {
        return 2 << 4;
    }
    memcpy( pDst->pStartDepth, pSrc->pStartDepth,
        numPulsesPerFrame * sizeof(float) );            /* The start depths of RFlines, array length is the number of pulses per frame,
                                                                                             values are in seconds */
    pDst->pEndDepth = (float *)calloc( numPulsesPerFrame, sizeof(float) );
    if ( pDst->pEndDepth == NULL )
    {
        return 3<<4;
    }
    memcpy( pDst->pEndDepth, pSrc->pEndDepth,numPulsesPerFrame*sizeof(float) );                /* The end depths of RFLines, this data can be considered redundant as it is
                                                                                             startDepth+(sampleFrequency*numSamplesPerLine)  */
    pDst->numTimeGainControlValues = pSrc->numTimeGainControlValues;                                             /* number of control points that describe the TGC */

    pDst->pTimeGainControl =
        (IusTimeGainControl *)calloc( pDst->numTimeGainControlValues,
                                      sizeof(IusTimeGainControl) );
    if ( pDst->pTimeGainControl == NULL )
    {
        return 4<<4;
    }

    memcpy( pDst->pTimeGainControl, pSrc->pTimeGainControl,
        pDst->numTimeGainControlValues*sizeof(IusTimeGainControl) );    /*< TimeGainControl points (time,gain) */

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_copyDrivingSchemeData
(
    IusDrivingScheme * pDst,
    IusDrivingScheme * pSrc,
    int                numPulsesPerFrame,
    int                numElements
)
{
    int i;
    IUS_ASSERT( numPulsesPerFrame > 0 );

    pDst->drivingSchemeType  = pSrc->drivingSchemeType;   // driving scheme: e.g. diveringwaves, planeswaves, ...
    pDst->numSamplesPerLine  = pSrc->numSamplesPerLine;   // length of an acquisition line
    pDst->numTransmitSources = pSrc->numTransmitSources;  // number of US sources (tyically these are virtual)
    pDst->numTransmitPulses  = pSrc->numTransmitPulses;   // number of pulses in a frame

    if ( pDst->numTransmitSources != 0 )
    {
	pDst->pSourceLocations = (IusPosition *)calloc(pDst->numTransmitSources, sizeof(IusPosition));
	for (i = 0; i < pDst->numTransmitSources; i++)
	{
	    pDst->pSourceLocations[i].x = pSrc->pSourceLocations[i].x;
	    pDst->pSourceLocations[i].y = pSrc->pSourceLocations[i].y;
	    pDst->pSourceLocations[i].z = pSrc->pSourceLocations[i].z;
	}
    }

    pDst->sourceFNumber        = pSrc->sourceFNumber;        // distance in [m] of sources to transducer for POLAR
    pDst->sourceAngularDelta   = pSrc->sourceAngularDelta;   // angle in [rad] between sources
    pDst->sourceStartAngle     = pSrc->sourceStartAngle;     // starting angle in [rad] for the sources
    pDst->transmitPatternDelay = pSrc->transmitPatternDelay; // extra delay at the end of a transmit pattern

    pDst->pTransmitPattern = (IusTransmitPattern *)calloc( numPulsesPerFrame, sizeof(IusTransmitPattern) );
    if ( pDst->pTransmitPattern == 0 )
    {
        return 1 << 7;
    }
    memcpy( pDst->pTransmitPattern, pSrc->pTransmitPattern,
            numPulsesPerFrame * sizeof( IusTransmitPattern ) );  // array (time, index) of length numTransmitPulses

    pDst->transmitPulse.numPulseValues = pSrc->transmitPulse.numPulseValues;  // waveform of the transmit pulse
    if ( pDst->transmitPulse.numPulseValues != 0 )
    {
        pDst->transmitPulse.pRawPulseAmplitudes = (float *)calloc( pDst->transmitPulse.numPulseValues, sizeof(float));       /* shape of waveform [in Volts] */
        pDst->transmitPulse.pRawPulseTimes      = (float *)calloc( pDst->transmitPulse.numPulseValues, sizeof(float));            /* corresponding timestamps of amplitudes [in seconds] */
        if ( pDst->transmitPulse.pRawPulseAmplitudes == NULL || pDst->transmitPulse.pRawPulseTimes == 0)
        {
            return 2 << 7;
        }
        memcpy( pDst->transmitPulse.pRawPulseAmplitudes, pSrc->transmitPulse.pRawPulseAmplitudes, pDst->transmitPulse.numPulseValues*sizeof(float));
        memcpy( pDst->transmitPulse.pRawPulseTimes,      pSrc->transmitPulse.pRawPulseTimes,      pDst->transmitPulse.numPulseValues*sizeof(float));
    }
    pDst->transmitPulse.pulseFrequency = pSrc->transmitPulse.pulseFrequency;
    pDst->transmitPulse.pulseAmplitude = pSrc->transmitPulse.pulseAmplitude;
    pDst->transmitPulse.pulseCount     = pSrc->transmitPulse.pulseCount;

    pDst->pTransmitApodization = (float *)calloc( numPulsesPerFrame * numElements, sizeof(float) );   // 2D array: per transmitted pulse we have numElement gains
                                                                                                      // (which are numTransducerElements or numChannelElements)
    if ( pDst->pTransmitApodization == NULL )
    {
        return 3<<7;
    }
    memcpy( pDst->pTransmitApodization, pSrc->pTransmitApodization,
            numPulsesPerFrame * numElements * sizeof(float) );

    if ( pSrc->transmitChannelCoding.numChannels != 0 )
    {
        pDst->transmitChannelCoding.numChannels = pSrc->transmitChannelCoding.numChannels;
        pDst->transmitChannelCoding.pChannelMap = (int *)calloc(pDst->transmitChannelCoding.numChannels*numPulsesPerFrame, sizeof(int));
        if ( pDst->transmitChannelCoding.pChannelMap == NULL )
        {
            return 4<<7;
        }
        memcpy( pDst->transmitChannelCoding.pChannelMap,
                pSrc->transmitChannelCoding.pChannelMap,
                pDst->transmitChannelCoding.numChannels * numPulsesPerFrame * sizeof(int) );
    }
    else
    {
        pDst->transmitChannelCoding.numChannels = 0;
        pDst->transmitChannelCoding.pChannelMap = NULL;
    }

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static herr_t LF_readTransElements(IusInputInstance *pInst, hid_t handle, int verbose)
{
  int           ndims;
  hsize_t       i;
  hid_t         dataset, space;
  hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent 
  IusPosition * pPosData;
  IusAngle *    pAngleData;
  IusSize *     pSizeData;
  hid_t         position_tid; //position type id
  hid_t         angle_tid;    //angle type id
  hid_t         size_tid;     //size id

  herr_t status = 0;
  IUS_UNUSED(verbose); // avoid compiler warning

  dataset = H5Dopen(handle, "/Transducer/Elements/positions", H5P_DEFAULT);
  space = H5Dget_space(dataset);
  ndims = H5Sget_simple_extent_dims(space, dims, NULL); 
  if ( ndims != 1 )
  {
    fprintf( stderr, "readTransElements: Only 1D array of transducer elements supported \n" );
    return -1; 
  }

  pPosData   = (IusPosition *) malloc( (size_t)(dims[0] * sizeof (IusPosition)) );
  pAngleData = (IusAngle *)    malloc( (size_t)(dims[0] * sizeof (IusAngle)) );
  pSizeData  = (IusSize *)     malloc( (size_t)(dims[0] * sizeof (IusSize)) );
  pInst->pTransducer->pElements = (IusTransducerElement *)malloc((size_t)(dims[0]*sizeof(IusTransducerElement)));
  if (pInst->pTransducer->pElements == NULL)
  {
      fprintf(stderr, "Allocation failed: pInst->pTransducer->pElements\n");
    return -1;
  }

  position_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusPosition));
  H5Tinsert(position_tid, "x", HOFFSET(IusPosition, x), H5T_NATIVE_FLOAT);
  H5Tinsert(position_tid, "y", HOFFSET(IusPosition, y), H5T_NATIVE_FLOAT);
  H5Tinsert(position_tid, "z", HOFFSET(IusPosition, z), H5T_NATIVE_FLOAT);
  status = H5Dread(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPosData);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
    return status;
  }
  status = H5Dclose(dataset);
  status = H5Sclose(space);
  status = H5Tclose(position_tid);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error closing position dataset: %d\n", status);
    return status;
  }

  dataset = H5Dopen(handle, "/Transducer/Elements/angles", H5P_DEFAULT);
  space = H5Dget_space(dataset);
  //read the angles array
  angle_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusAngle));
  H5Tinsert(angle_tid, "theta", HOFFSET(IusAngle, theta), H5T_NATIVE_FLOAT);
  H5Tinsert(angle_tid, "phi",   HOFFSET(IusAngle, phi),   H5T_NATIVE_FLOAT);
  status = H5Dread(dataset, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleData);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
    return status;
  }
  status = H5Dclose(dataset);
  status = H5Sclose(space);
  status = H5Tclose(angle_tid);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error closing angle dataset: %d\n", status);
    return status;
  }

  dataset = H5Dopen(handle, "/Transducer/Elements/sizes", H5P_DEFAULT);
  space = H5Dget_space(dataset);
  //read the angles array
  size_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusSize));
  H5Tinsert(size_tid, "x", HOFFSET(IusSize, x), H5T_NATIVE_FLOAT);
  H5Tinsert(size_tid, "y", HOFFSET(IusSize, y), H5T_NATIVE_FLOAT);
  H5Tinsert(size_tid, "z", HOFFSET(IusSize, z), H5T_NATIVE_FLOAT);
  status = H5Dread(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeData);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
    return status;
  }
  status = H5Dclose(dataset);
  status = H5Sclose(space);
  status = H5Tclose(size_tid);
  if (status < 0)
  {
    fprintf(stderr, "readTransElements: Error closing size dataset: %d\n", status);
    return status;
  }

  for ( i = 0; i < dims[0]; i++ )
  {
    pInst->pTransducer->pElements[i].position.x = pPosData[i].x;
    pInst->pTransducer->pElements[i].position.y = pPosData[i].y;
    pInst->pTransducer->pElements[i].position.z = pPosData[i].z;
    pInst->pTransducer->pElements[i].angle.theta= pAngleData[i].theta;
    pInst->pTransducer->pElements[i].angle.phi  = pAngleData[i].phi;
    pInst->pTransducer->pElements[i].size.x     = pSizeData[i].x;
    pInst->pTransducer->pElements[i].size.y     = pSizeData[i].y;
    pInst->pTransducer->pElements[i].size.z     = pSizeData[i].z;
  }
  free(pPosData);
  free(pAngleData);
  free(pSizeData);

  return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static herr_t LF_readSourceLocationsCartesian
(
    IusInputInstance * pInst,
    hid_t              handle,
    int                verbose
)
{
    herr_t status = 0;

    int           i, ndims;
    hid_t         dataset, space;
    hsize_t       dims[4];  // NOTE: pElements assumed 1D array, extra length to prevent 
    IusPosition * pPosData;
    hid_t         position_tid; //position type id

    int numLocations = pInst->pDrivingScheme->numTransmitSources;

    IUS_UNUSED(verbose); // avoid compiler warnings

    dataset = H5Dopen( handle, "/DrivingScheme/sourceLocations", H5P_DEFAULT );
    space   = H5Dget_space( dataset );
    ndims   = H5Sget_simple_extent_dims( space, dims, NULL ); 
    if ( ndims != 1 )
    {
        fprintf( stderr, "Rank should be 1, but found %d\n", ndims );
        return 1;
    }
    pPosData   = (IusPosition *) malloc( (size_t)(dims[0] * sizeof(IusPosition) ) );
    pInst->pDrivingScheme->pSourceLocations = (IusPosition *)malloc( (size_t)(dims[0] * sizeof(IusPosition)) );
    if ( pInst->pDrivingScheme->pSourceLocations == NULL )
    {
        fprintf( stderr, "LF_readSourceLocationsCartesian: Error allocating data\n" );
        free( pPosData );
        return -1;
    }

    //read the positions array
    position_tid = H5Tcreate( H5T_COMPOUND, sizeof(IusPosition));
    H5Tinsert( position_tid, "x", HOFFSET(IusPosition, x), H5T_NATIVE_FLOAT );
    H5Tinsert( position_tid, "y", HOFFSET(IusPosition, y), H5T_NATIVE_FLOAT );
    H5Tinsert( position_tid, "z", HOFFSET(IusPosition, z), H5T_NATIVE_FLOAT );
    status = H5Dread( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPosData );
    if ( status < 0 )
    {
        fprintf( stderr, "LF_readSourceLocationsCartesian: Error reading dataset: H5Dread returned: %d\n", status );
        free( pPosData );
        return status;
    }

    for ( i = 0; i < numLocations; i++ )
    {
        pInst->pDrivingScheme->pSourceLocations[i].x = pPosData[i].x; 
        pInst->pDrivingScheme->pSourceLocations[i].y = pPosData[i].y; 
        pInst->pDrivingScheme->pSourceLocations[i].z = pPosData[i].z; 
    }
    free( pPosData );

    status = H5Dclose( dataset );
    status = H5Sclose( space );
    status = H5Tclose( position_tid );

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static herr_t LF_readPulseRaw
(
    IusInputInstance * pInst,
    hid_t              handle,
    int                numAmplitudes,
    int                verbose
)
{
    herr_t status = 0;

    IUS_UNUSED(verbose); // avoid compiler warnings

    pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes = (float *) malloc(numAmplitudes * sizeof(float));
    pInst->pDrivingScheme->transmitPulse.pRawPulseTimes = (float *) malloc(numAmplitudes * sizeof(float));
    if ( pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes == NULL ||
         pInst->pDrivingScheme->transmitPulse.pRawPulseTimes == NULL )
    {
        fprintf( stderr, "LF_readPulseRaw: Error allocating data\n" );
        return -1;
    }

    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/rawPulseAmplitudes", pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/rawPulseTimes", pInst->pDrivingScheme->transmitPulse.pRawPulseTimes );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/pulseFrequency", &(pInst->pDrivingScheme->transmitPulse.pulseFrequency) );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/pulseAmplitude", &(pInst->pDrivingScheme->transmitPulse.pulseAmplitude) );
    status = H5LTread_dataset_int( handle, "/DrivingScheme/TransmitPulse/pulseCount", &(pInst->pDrivingScheme->transmitPulse.pulseCount) );
    if ( status < 0 )
    {
        fprintf( stderr, "LF_readPulseRaw: Error reading dataset: H5Dread returned: %d\n", status );
    }

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static herr_t LF_readTransmitPattern
(
    IusInputInstance * pInst,
    hid_t              handle,
    int verbose
)
{
    int i;
    herr_t status=0;
    int *   pIndexData;
    float * pTimeData;

    int numPulses = pInst->pDrivingScheme->numTransmitPulses;

    IUS_UNUSED(verbose); // avoid compiler warnings

    pIndexData   =   (int *) malloc(numPulses * sizeof(int));
    pTimeData   = (float *) malloc(numPulses * sizeof(float));
    pInst->pDrivingScheme->pTransmitPattern = (IusTransmitPattern *)malloc(numPulses*sizeof(IusTransmitPattern));
    if (pInst->pDrivingScheme->pTransmitPattern == NULL || pIndexData == NULL || pTimeData == NULL)
    {
        fprintf( stderr, "LF_readTransmitPattern: Error allocating data\n" );
        free( pIndexData );
        free( pTimeData );
        return -1;
    }

    status = H5LTread_dataset_int( handle,   "/DrivingScheme/TransmitPattern/transmitPatternIndex",  pIndexData );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPattern/transmitPatternTime",   pTimeData );
    if ( status < 0 )
    {
        fprintf( stderr, "LF_readTransmitPattern: Error reading dataset: H5Dread returned: %d\n", status );
        free( pIndexData );
        free( pTimeData );
        return status;
    }

    for ( i = 0; i < numPulses; i++ )
    {
        pInst->pDrivingScheme->pTransmitPattern[i].index = pIndexData[i]; 
        pInst->pDrivingScheme->pTransmitPattern[i].time  = pTimeData[i]; 
    }

    free( pIndexData );
    free( pTimeData );

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
static herr_t LF_readTimeGainControls
(
    IusInputInstance * pInst,
    hid_t              handle,
    int                verbose
)
{
    int i;
    herr_t status = 0;
    float * pGainData;
    float * pTimeData;

    int numElements = pInst->pReceiveSettings->numTimeGainControlValues;

    IUS_UNUSED(verbose); // avoid compiler warnings

    pGainData   = (float *) malloc(numElements * sizeof(float));
    pTimeData   = (float *) malloc(numElements * sizeof(float));
    pInst->pReceiveSettings->pTimeGainControl = (IusTimeGainControl *)malloc(numElements*sizeof(IusTimeGainControl));
    if (pGainData==NULL || pTimeData==NULL || pInst->pReceiveSettings->pTimeGainControl ==NULL)
    {
        fprintf( stderr, "LF_readTimeGainControls: Error allocating memory\n" );
        free( pGainData );
        free( pTimeData );
        return -1;
    }

    status = H5LTread_dataset_float( handle, "/ReceiveSettings/TimeGainControl/gains",   pGainData );
    status = H5LTread_dataset_float( handle, "/ReceiveSettings/TimeGainControl/timings", pTimeData );
    if ( status < 0 )
    {
        fprintf( stderr, "LF_readTimeGainControls: Error reading dataset: H5Dread returned: %d\n", status );
        free( pGainData );
        free( pTimeData );
        return status;
    }

    for ( i = 0; i < numElements; i++ )
    {
        pInst->pReceiveSettings->pTimeGainControl[i].gain = pGainData[i]; 
        pInst->pReceiveSettings->pTimeGainControl[i].time = pTimeData[i]; 
    }

    free( pGainData );
    free( pTimeData );

    return status;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusInputSetDepthRange
(
    const IusInputInstance * const pInst,
    int                            startIndex,
    int                            numSamples,
    IusRange * const               pDepthRange
)
{
    IUS_ASSERT_MEMORY( pInst != NULL );
    IUS_ASSERT_MEMORY( pDepthRange != NULL );
    IUS_ASSERT_VALUE( startIndex + numSamples <=
        pInst->pDrivingScheme->numSamplesPerLine );
    IUS_UNUSED(pInst); // avoid unreferenced parameter warning

    pDepthRange->startIndex = startIndex;
    pDepthRange->numSamples = numSamples;

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusInputInstance * iusInputCreate
(
    IusExperiment      * pExperiment,
    IusTransducer      * pTransducer,
    IusReceiveSettings * pReceiveSettings,
    IusDrivingScheme   * pDrivingScheme,
    int                  version,
    int                  numFrames
)
{
    int copyStatus  = -1;
    int numElements =  0;
    IusInputInstance * pInst;
    IUS_UNUSED(version);
    IUS_UNUSED(numFrames);

    if ( pExperiment == NULL ||
         pTransducer == NULL ||
         pReceiveSettings == NULL ||
         pDrivingScheme == NULL)
    {
        fprintf( stderr, "iusInputCreate: Input arguments can not be NULL \n" );
        return NULL;
    }
    //--------------------------------------------------------------------------
    // alloc instance ; using calloc to clear all state.
    //--------------------------------------------------------------------------
    pInst = (IusInputInstance *)calloc( 1, sizeof( IusInputInstance ) );
    if ( pInst == NULL )
    {
        fprintf( stderr, "iusInputCreate: calloc of data instance failed\n" );
        return NULL;
    }

    pInst->pExperiment = (IusExperiment *)calloc( 1, sizeof( IusExperiment ) );
    if ( pInst->pExperiment == NULL )
    {
        free( pInst );
        fprintf( stderr, "Allocation failed: pInst->pExperiment\n" );
        return NULL;
    }

    pInst->pTransducer = (IusTransducer *)calloc( 1, sizeof( IusTransducer ) );
    if ( pInst->pTransducer == NULL )
    {
        free( pInst->pExperiment );
        free( pInst );
        fprintf( stderr, "Allocation failed: pInst->pTransducer\n" );
        return NULL;
    }

    pInst->pReceiveSettings = (IusReceiveSettings *)calloc( 1, sizeof( IusReceiveSettings ) );
    if ( pInst->pReceiveSettings == NULL )
    {
        free( pInst->pExperiment );
        free( pInst->pTransducer );
        free( pInst );
        fprintf( stderr, "Allocation failed: pInst->pReceiveSettings\n" );
        return NULL;
    }

    pInst->pDrivingScheme = (IusDrivingScheme *)calloc( 1, sizeof( IusDrivingScheme ) );
    if ( pInst->pDrivingScheme == NULL ) 
    {
        free( pInst->pExperiment );
        free( pInst->pTransducer );
        free( pInst->pReceiveSettings );
        free( pInst );
        fprintf( stderr, "Allocation failed: pInst->pReceiveSettings" );
        return NULL;
    }

    copyStatus  = LF_copyExperimentData( pInst->pExperiment, pExperiment );
    copyStatus |= LF_copyTransducerData( pInst->pTransducer, pTransducer );
    copyStatus |= LF_copyReceiveSettingsData( pInst->pReceiveSettings,
                      pReceiveSettings, pDrivingScheme->numTransmitPulses );
    
    if ( pInst->pDrivingScheme->transmitChannelCoding.numChannels == 0 )
    {
        //we use all transducer elements
        numElements = pInst->pTransducer->numElements; 
    }
    else
    {
        // we use only th number of channels
        numElements = pInst->pDrivingScheme->transmitChannelCoding.numChannels;
    }

    copyStatus |=
        LF_copyDrivingSchemeData( pInst->pDrivingScheme, pDrivingScheme,
        pDrivingScheme->numTransmitPulses, numElements);
    if ( copyStatus != 0 )
    {
        fprintf( stderr, 
            "iusInputCreate: deep copy of data instance failed: status is %d\n",
            copyStatus );
        free( pInst->pExperiment );
        free( pInst->pTransducer );
        free( pInst->pReceiveSettings );
        free( pInst->pDrivingScheme );
        free( pInst);
        return NULL;
    }

    // A new instance of Input data is created. This is the only place where a
    // Uuid should be generated for it.
    setIusUuidCreate( pInst->iusNode.pId );
    strcpy( pInst->iusNode.pType, IUS_INPUT_TYPE ); 

    return pInst;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusInputInstance * iusInputRead
(
    hid_t handle,
    int   verbose
)
{
    IusInputInstance *pInst;
    herr_t  status;
  
    //--------------------------------------------------------------------------
    // alloc instance ; using calloc to clear all state.
    //--------------------------------------------------------------------------
    if ( handle <= 0 )
    {
        fprintf( stderr, "iuInputRead: Error hid_t handle is invalid\n" );
        return NULL;
    }

    pInst = (IusInputInstance *)calloc( 1, sizeof( IusInputInstance ) );
    if ( pInst == NULL )
    {
        fprintf( stderr, "iusInputRead: calloc failed\n" );
        return NULL;
    }

    strcpy( pInst->iusNode.pType, IUS_INPUT_TYPE );

    pInst->pExperiment = (IusExperiment *)calloc( 1, sizeof( IusExperiment ) );
    if ( pInst->pExperiment == NULL )
    {
        free(pInst);
        return NULL;
    }

    pInst->pTransducer = (IusTransducer *)calloc( 1, sizeof( IusTransducer ) );
    if ( pInst->pTransducer == NULL )
    {
        free(pInst->pExperiment);
        free(pInst);
        return NULL;
    }

    pInst->pReceiveSettings =
        (IusReceiveSettings *)calloc( 1, sizeof( IusReceiveSettings ) );
    if ( pInst->pReceiveSettings == NULL )
    {
        free( pInst->pTransducer );
        free( pInst->pExperiment );
        free( pInst );
        return NULL;
    } 

    pInst->pDrivingScheme =
        (IusDrivingScheme *)calloc( 1, sizeof( IusDrivingScheme ) );
    if (pInst->pDrivingScheme == NULL)
    {
        free( pInst->pReceiveSettings );
        free( pInst->pTransducer );
        free( pInst->pExperiment );
        free( pInst);
        return NULL;
    }
  
    //--------------------------------------------------------------------------
    // init instance variables
    //--------------------------------------------------------------------------

    status = iusHdf5ReadFloat( handle , "/Experiment/speedOfSound",    &(pInst->pExperiment->speedOfSound),              verbose );
    status = iusHdf5ReadInt( handle,    "/Experiment/date",            &(pInst->pExperiment->date),                      verbose );
    status = iusHdf5ReadString( handle, "/Experiment/description",     (char * *)&(pInst->pExperiment->pDescription),    verbose );
    status = iusHdf5ReadString( handle, "/Transducer/transducerName",  (char * *)&(pInst->pTransducer->pTransducerName), verbose );
    status = iusHdf5ReadFloat( handle,  "/Transducer/centerFrequency", &(pInst->pTransducer->centerFrequency),           verbose );
    status = iusHdf5ReadInt( handle,    "/Transducer/numElements",     &(pInst->pTransducer->numElements),               verbose );

    status = LF_readTransElements( pInst, handle, verbose );
    status = iusHdf5ReadInt( handle, "/DrivingScheme/drivingSchemeType", (int *)&(pInst->pDrivingScheme->drivingSchemeType), verbose );

    /* Based on a native signed short */
    hid_t hdf_drivingSchemeType = H5Tcreate(H5T_ENUM, sizeof(short));
    short enumValue = 0;
    H5Tenum_insert( hdf_drivingSchemeType, "DIVERGING_WAVES_RADIAL", (enumValue=IUS_DIVERGING_WAVES,&enumValue));
    H5Tenum_insert( hdf_drivingSchemeType, "FOCUSED_WAVES",          (enumValue=IUS_FOCUSED_WAVES,&enumValue));
    H5Tenum_insert( hdf_drivingSchemeType, "PLANE_WAVES",            (enumValue=IUS_PLANE_WAVES,  &enumValue));
    H5Tenum_insert( hdf_drivingSchemeType, "SINGLE_ELEMENT",         (enumValue=IUS_SINGLE_ELEMENT,  &enumValue));

    status = H5LTread_dataset( handle, "/DrivingScheme/drivingSchemeType",   hdf_drivingSchemeType, &enumValue ); 
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numSamplesPerLine",   &(pInst->pDrivingScheme->numSamplesPerLine), verbose );
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numTransmitPulses",   &(pInst->pDrivingScheme->numTransmitPulses), verbose );
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numTransmitSources",  &(pInst->pDrivingScheme->numTransmitSources), verbose );
    status = iusHdf5ReadInt( handle,   "/numFrames",                         &(pInst->numFrames), verbose );

    if ( pInst->pDrivingScheme->drivingSchemeType == IUS_DIVERGING_WAVES )
    {
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceAngularDelta", &(pInst->pDrivingScheme->sourceAngularDelta), verbose );
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceFNumber",      &(pInst->pDrivingScheme->sourceFNumber), verbose );
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceStartAngle",   &(pInst->pDrivingScheme->sourceStartAngle), verbose );
    }
    else
    {  
        status = LF_readSourceLocationsCartesian(pInst, handle, verbose);
    }
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPattern/transmitPatternDelay", &(pInst->pDrivingScheme->transmitPatternDelay), verbose );
    status = LF_readTransmitPattern( pInst, handle, verbose );
    status = iusHdf5ReadInt( handle, "/DrivingScheme/TransmitPulse/numPulseValues", &(pInst->pDrivingScheme->transmitPulse.numPulseValues), verbose );

    if (pInst->pDrivingScheme->transmitPulse.numPulseValues != 0)
    {
        // read rawTransmitPulse 
        status = LF_readPulseRaw(pInst, handle, pInst->pDrivingScheme->transmitPulse.numPulseValues, verbose);
    }
    else
    {
        //alternative waveform description
        // TODO: define these
    }
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPulse/pulseFrequency", &(pInst->pDrivingScheme->transmitPulse.pulseFrequency), verbose ); 
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPulse/pulseAmplitude", &(pInst->pDrivingScheme->transmitPulse.pulseAmplitude), verbose ); 
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/TransmitPulse/pulseCount", &(pInst->pDrivingScheme->transmitPulse.pulseCount), verbose ); 

//receiveSettings->receiveChannelCoding.numChannels
    status = iusHdf5ReadInt(handle, "/ReceiveSettings/numChannels", &(pInst->pReceiveSettings->receiveChannelCoding.numChannels), verbose);
    pInst->pReceiveSettings->receiveChannelCoding.pChannelMap = (int*)calloc(pInst->pReceiveSettings->receiveChannelCoding.numChannels, sizeof(int));
    status = iusHdf5ReadInt(handle, "/ReceiveSettings/channelMap", pInst->pReceiveSettings->receiveChannelCoding.pChannelMap, verbose);

    status = iusHdf5ReadFloat( handle, "/ReceiveSettings/sampleFrequency", &(pInst->pReceiveSettings->sampleFrequency),verbose );
    status = iusHdf5ReadInt( handle,   "/ReceiveSettings/TimeGainControl/numValues", &(pInst->pReceiveSettings->numTimeGainControlValues), verbose );
    status = LF_readTimeGainControls( pInst, handle, verbose );
    pInst->pReceiveSettings->pStartDepth = (float *)calloc(pInst->pDrivingScheme->numTransmitPulses, sizeof(float));
    pInst->pReceiveSettings->pEndDepth   = (float *)calloc(pInst->pDrivingScheme->numTransmitPulses, sizeof(float));
    status = H5LTread_dataset_float(handle, "/ReceiveSettings/startDepth", pInst->pReceiveSettings->pStartDepth);
    status = H5LTread_dataset_float(handle, "/ReceiveSettings/endDepth", pInst->pReceiveSettings->pEndDepth);

    status = iusHdf5ReadInt( handle, "/IusVersion", &(pInst->IusVersion), verbose ); 
    status = iusHdf5ReadString( handle, "/ID", (char **)&(pInst->iusNode.pId), verbose );

    iusNodeLoadParents((IusNode *)pInst, handle);

    return pInst;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusInputWrite
( 
    hid_t handle, 
    IusInputInstance *pInst,
    int version
)
{
    herr_t        status;
    IusPosition * pPositionArray;
    IusAngle *    pAngleArray;
    IusSize *     pSizeArray;
    float *       pFloatArray;
    int *         pIntArray;
    const int     libVersion = version;

    hid_t position_tid; // File datatype identifier for IusPosition
    hid_t angle_tid;    // File datatype identifier for IusAngle
    hid_t size_tid;     // File datatype identifier for IusSize
    hid_t propList;     // Property list
    hid_t group_id;     // group ID
    hid_t subgroup_id;  // subgroup ID
    hid_t dataset, space, dataChunkConfig; 

    hsize_t dims[1] = {1};
    hsize_t rfDataDims[4];
    hsize_t apodDims[2] = {0, 0};
    hsize_t chunkDims[4];

    int i; //iterator
    hsize_t j; //iterator

    if ( handle == 0 || pInst == NULL )
    {
        fprintf( stderr, "iusInputWrite: Input arguments can not be NULL \n");
        return -1;
    }

    propList = H5Pcreate(H5P_DATASET_CREATE);
    /* Create a new file using default properties. */
    group_id = H5Gcreate(handle, "/Experiment", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    /* write the /Experiment data */
    H5LTmake_dataset_float( group_id,  "/Experiment/speedOfSound", 1, dims, &(pInst->pExperiment->speedOfSound) );
    H5LTmake_dataset_int( group_id,    "/Experiment/date",         1, dims, &(pInst->pExperiment->date) );
    H5LTmake_dataset_string( group_id, "/Experiment/description", pInst->pExperiment->pDescription );
    /* Close the group. */
    status = H5Gclose( group_id );

    /* write the /Transducer data */ 
    group_id = H5Gcreate( handle,   "/Transducer", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    H5LTmake_dataset_string( handle,"/Transducer/transducerName",     pInst->pTransducer->pTransducerName );
    dims[0] = 1;
    H5LTmake_dataset_float( handle, "/Transducer/centerFrequency", 1, dims, &(pInst->pTransducer->centerFrequency) );
    H5LTmake_dataset_int( handle,   "/Transducer/numElements",     1, dims, &(pInst->pTransducer->numElements) );

    /* write the /Transducer/Elements/ positions, angles and sizes are compound types */ 
    dims[0] = pInst->pTransducer->numElements;
    subgroup_id = H5Gcreate( group_id, "/Transducer/Elements", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    {
        //Positions
        space = H5Screate_simple( 1, dims, NULL );
        // step a:  create H5 dataset
        position_tid = H5Tcreate( H5T_COMPOUND, sizeof(IusPosition) );
        H5Tinsert( position_tid, "x", HOFFSET(IusPosition, x), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "y", HOFFSET(IusPosition, y), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "z", HOFFSET(IusPosition, z), H5T_NATIVE_FLOAT );
        dataset = H5Dcreate( subgroup_id,    "/Transducer/Elements/positions", position_tid, space, H5P_DEFAULT, propList, H5P_DEFAULT );

        // step b:  create array of positions
        pPositionArray = (IusPosition *)calloc( pInst->pTransducer->numElements, sizeof(IusPosition) ); //three dimensions for position 
        for ( i = 0; i < pInst->pTransducer->numElements; i++ )
        {
            pPositionArray[i] = pInst->pTransducer->pElements[i].position;
        }

        // step c: write the array to the dataset
        status = H5Dwrite( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPositionArray );

        free( pPositionArray );
        // step d: release resources 
        H5Tclose( position_tid );
        H5Sclose( space );
        H5Dclose( dataset );
    }

    { //Angles
        space = H5Screate_simple( 1, dims, NULL );
        // step a:  create H5 dataset
        angle_tid = H5Tcreate( H5T_COMPOUND, sizeof(IusAngle) );
        H5Tinsert( angle_tid, "theta", HOFFSET(IusAngle, theta), H5T_NATIVE_FLOAT );
        H5Tinsert( angle_tid,  "phi", HOFFSET(IusAngle, phi), H5T_NATIVE_FLOAT );
        dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/angles", angle_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

        // step b:  create array of angles
        pAngleArray = (IusAngle *)calloc( pInst->pTransducer->numElements, sizeof(IusAngle) ); //two dimensions for angle 
        for (i = 0; i < pInst->pTransducer->numElements; i++)
        {
            pAngleArray[i] = pInst->pTransducer->pElements[i].angle;
        }

        // step c: write the array to the dataset
        status = H5Dwrite( dataset, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleArray );

        free(pAngleArray);
        // step d: release resources 
        H5Tclose(angle_tid);
        H5Sclose(space);
        H5Dclose(dataset);
    }

    { //Sizes
        space = H5Screate_simple(1, dims, NULL);
        // step a:  create H5 dataset
        size_tid = H5Tcreate(H5T_COMPOUND, sizeof(IusSize));
        H5Tinsert( size_tid, "x", HOFFSET(IusSize, x), H5T_NATIVE_FLOAT );
        H5Tinsert( size_tid, "y", HOFFSET(IusSize, y), H5T_NATIVE_FLOAT );
        H5Tinsert( size_tid, "z", HOFFSET(IusSize, z), H5T_NATIVE_FLOAT );
        dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/sizes",
            size_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

        // step b:  create array of sizes
        pSizeArray = (IusSize *)calloc( pInst->pTransducer->numElements*3, sizeof(IusSize) ); //three dimensions for size 
        for ( i = 0; i < pInst->pTransducer->numElements; i++ )
        {
            pSizeArray[i] = pInst->pTransducer->pElements[i].size;
        }
        // step c: write the array to the dataset
        status = H5Dwrite( dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeArray );
        /* Close the subgroup. */
        status = H5Gclose( subgroup_id );

        // step d: release resources 
        free( pSizeArray );
        H5Tclose( size_tid );
        H5Sclose( space );
        H5Dclose( dataset );
    }

      /* Close the group. */
      status = H5Gclose( group_id );

      dims[0] = 1;
      //float *endDepths = (float *)calloc()
      //  pReceiveSettings->pStartDepth + (pDrivingScheme->numSamplesPerLine * pReceiveSettings->sampleFrequency);
      group_id = H5Gcreate( handle, "/ReceiveSettings", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
      // write channel map
      H5LTmake_dataset_int(group_id, "/ReceiveSettings/numChannels", 1, dims, &(pInst->pReceiveSettings->receiveChannelCoding.numChannels));
      if(pInst->pReceiveSettings->receiveChannelCoding.numChannels > 0)
      {
	  dims[0] = pInst->pReceiveSettings->receiveChannelCoding.numChannels;
	  H5LTmake_dataset_int(group_id, "/ReceiveSettings/channelMap", 1, dims, pInst->pReceiveSettings->receiveChannelCoding.pChannelMap);
      }

      H5LTmake_dataset_float( group_id,    "/ReceiveSettings/sampleFrequency", 1, dims, &(pInst->pReceiveSettings->sampleFrequency) );
      dims[0] = pInst->pDrivingScheme->numTransmitPulses;
      H5LTmake_dataset_float( group_id,    "/ReceiveSettings/startDepth", 1, dims, pInst->pReceiveSettings->pStartDepth );
      H5LTmake_dataset_float( group_id,    "/ReceiveSettings/endDepth"  , 1, dims, pInst->pReceiveSettings->pEndDepth );
      subgroup_id = H5Gcreate( group_id,   "/ReceiveSettings/TimeGainControl", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
      dims[0] = 1;
      H5LTmake_dataset_int( subgroup_id,   "/ReceiveSettings/TimeGainControl/numValues", 1, dims, &(pInst->pReceiveSettings->numTimeGainControlValues) );

    {// TimeGainControl settings (gain, time)
        pFloatArray = (float *)calloc( pInst->pReceiveSettings->numTimeGainControlValues, sizeof(float) ); //three dimensions for size 
        dims[0] = pInst->pReceiveSettings->numTimeGainControlValues;
        for ( i = 0; i < pInst->pReceiveSettings->numTimeGainControlValues; i++ )
        {
            pFloatArray[i] = pInst->pReceiveSettings->pTimeGainControl[i].gain;
        }
        H5LTmake_dataset_float(subgroup_id,"/ReceiveSettings/TimeGainControl/gains", 1, dims, pFloatArray );
        for ( i = 0; i < pInst->pReceiveSettings->numTimeGainControlValues; i++ )
        {
            pFloatArray[i] = pInst->pReceiveSettings->pTimeGainControl[i].time;
        }
        H5LTmake_dataset_float( subgroup_id,"/ReceiveSettings/TimeGainControl/timings", 1, dims, pFloatArray );
        free(pFloatArray);
    }
    /* Close the subgroup. */
    status = H5Gclose( subgroup_id );
    /* Close the group. */
    status = H5Gclose( group_id );

    group_id = H5Gcreate( handle, "/DrivingScheme", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    dims[0]=1;
  
    /* Based on a native signed short */
    hid_t hdf_drivingSchemeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    H5Tenum_insert( hdf_drivingSchemeType, "DIVERGING_WAVES_RADIAL", (enumValue=IUS_DIVERGING_WAVES,&enumValue) );
    H5Tenum_insert( hdf_drivingSchemeType, "FOCUSED_WAVES",          (enumValue=IUS_FOCUSED_WAVES,&enumValue) );
    H5Tenum_insert( hdf_drivingSchemeType, "PLANE_WAVES",            (enumValue=IUS_PLANE_WAVES,  &enumValue) );
    H5Tenum_insert( hdf_drivingSchemeType, "SINGLE_ELEMENT",         (enumValue=IUS_SINGLE_ELEMENT,  &enumValue) );
    //H5Tlock(hdf_drivingSchemeType);
  
    enumValue = pInst->pDrivingScheme->drivingSchemeType;
    //H5LTmake_dataset_( group_id,     "/DrivingScheme/drivingSchemeType",    1, dims, (const int *)&enumValue );
    H5LTmake_dataset( group_id, "/DrivingScheme/drivingSchemeType", 1, dims, hdf_drivingSchemeType, &enumValue );

    H5LTmake_dataset_int( group_id, "/DrivingScheme/numSamplesPerLine",  1, dims, &(pInst->pDrivingScheme->numSamplesPerLine) );
    H5LTmake_dataset_int( group_id, "/DrivingScheme/numTransmitPulses",  1, dims, &(pInst->pDrivingScheme->numTransmitPulses) );
    H5LTmake_dataset_int( group_id, "/DrivingScheme/numTransmitSources", 1, dims, &(pInst->pDrivingScheme->numTransmitSources) );

    if (enumValue == IUS_DIVERGING_WAVES) //use radial info
    {
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceFNumber",      1, dims, &(pInst->pDrivingScheme->sourceFNumber) );
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceAngularDelta", 1, dims, &(pInst->pDrivingScheme->sourceAngularDelta) );
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceStartAngle",   1, dims, &(pInst->pDrivingScheme->sourceStartAngle) );
    }
    else
    {
        dims[0] = pInst->pDrivingScheme->numTransmitSources;
        space   = H5Screate_simple( 1, dims, NULL );
        // step a:  create H5 dataset
        position_tid = H5Tcreate( H5T_COMPOUND, sizeof(IusPosition) );
        H5Tinsert( position_tid, "x", HOFFSET(IusPosition, x), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "y", HOFFSET(IusPosition, y), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "z", HOFFSET(IusPosition, z), H5T_NATIVE_FLOAT );
        //H5Dcreate( hid_t loc_id, const char *name, hid_t type_id, hid_t space_id, hid_t create_plist_id) 
        dataset = H5Dcreate( group_id, "/DrivingScheme/sourceLocations", position_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

        // step b: write the array to the dataset
        status = H5Dwrite( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pInst->pDrivingScheme->pSourceLocations );
        // step c: release resources 
        H5Tclose( position_tid );
        H5Sclose( space );
        H5Dclose( dataset );
    }

    subgroup_id = H5Gcreate( group_id,   "/DrivingScheme/TransmitPattern/", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPattern/transmitPatternDelay",  1, dims, &(pInst->pDrivingScheme->transmitPatternDelay) );

    dims[0] = pInst->pDrivingScheme->numTransmitPulses;
    pFloatArray = (float *)calloc( (size_t)dims[0], sizeof(float) ); 
    for ( j = 0; j < dims[0]; j++ )
    {
        pFloatArray[j] = pInst->pDrivingScheme->pTransmitPattern[j].time;
    }
    H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPattern/transmitPatternTime",  1, dims, pFloatArray );
    free( pFloatArray );

    pIntArray = (int *)calloc( (size_t)dims[0], sizeof(int) ); 
    for ( j = 0; j < dims[0]; j++ )
    {
        pIntArray[j] = pInst->pDrivingScheme->pTransmitPattern[j].index;
    }
    H5LTmake_dataset_int( subgroup_id,   "/DrivingScheme/TransmitPattern/transmitPatternIndex",  1, dims, pIntArray );
    free( pIntArray );
    H5Gclose( subgroup_id );

    subgroup_id = H5Gcreate( group_id,   "/DrivingScheme/TransmitPulse", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    dims[0] = 1;
    H5LTmake_dataset_int( subgroup_id,   "/DrivingScheme/TransmitPulse/numPulseValues",  1, dims, &(pInst->pDrivingScheme->transmitPulse.numPulseValues) );
    if ( pInst->pDrivingScheme->transmitPulse.numPulseValues > 0 )
    {
        dims[0] = pInst->pDrivingScheme->transmitPulse.numPulseValues;
        H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPulse/rawPulseAmplitudes", 1, dims, pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes );
        H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPulse/rawPulseTimes", 1, dims, pInst->pDrivingScheme->transmitPulse.pRawPulseTimes );
    }

    dims[0] = 1;
    H5LTmake_dataset_float( subgroup_id,"/DrivingScheme/TransmitPulse/pulseFrequency",  1, dims, &(pInst->pDrivingScheme->transmitPulse.pulseFrequency) );
    H5LTmake_dataset_float( subgroup_id,"/DrivingScheme/TransmitPulse/pulseAmplitude",  1, dims, &(pInst->pDrivingScheme->transmitPulse.pulseAmplitude) );
    H5LTmake_dataset_int( subgroup_id,  "/DrivingScheme/TransmitPulse/pulseCount",  1, dims, &(pInst->pDrivingScheme->transmitPulse.pulseCount) );
    H5Gclose( subgroup_id );
    apodDims[0] = pInst->pDrivingScheme->numTransmitPulses;
    apodDims[1] = pInst->pTransducer->numElements;
    H5LTmake_dataset_float( group_id,"/DrivingScheme/transmitApodization",  2, apodDims, pInst->pDrivingScheme->pTransmitApodization );
  
    H5Gclose( group_id );
    H5LTmake_dataset_int( handle,    "/IusVersion", 1, dims, &libVersion );

    H5LTmake_dataset_string( handle, "/ID", pInst->iusNode.pId );
    H5LTmake_dataset_string( handle, "/type", pInst->iusNode.pType );
    H5LTmake_dataset_int( handle,    "/numFrames",  1, dims, &(pInst->numFrames) );

    iusNodeSaveParents( (IusNode*)pInst, handle );

    return 0;
}


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
void iusInputDestroy
(
   IusInputInstance * pInst
)
{
//    IUS_ASSERT_MEMORY( pInst );
//    IUS_ASSERT_MEMORY( pInst->pExperiment );
//    IUS_ASSERT_MEMORY( pInst->pTransducer );
//    IUS_ASSERT_MEMORY( pInst->pReceiveSettings );
//    IUS_ASSERT_MEMORY( pInst->pDrivingScheme );

    //free memory of sub structures

    free( pInst->pExperiment->pDescription );
    free( pInst->pExperiment );
    
    free( pInst->pTransducer->pTransducerName );
    free( pInst->pTransducer->pElements );
    free( pInst->pTransducer );

    if ( pInst->pReceiveSettings->receiveChannelCoding.numChannels != 0 )
    {
        free( pInst->pReceiveSettings->receiveChannelCoding.pChannelMap );
    }
    free( pInst->pReceiveSettings->pStartDepth );
    free( pInst->pReceiveSettings->pEndDepth );
    free( pInst->pReceiveSettings->pTimeGainControl );
    free( pInst->pReceiveSettings );

    free( pInst->pDrivingScheme->pTransmitPattern );
    free( pInst->pDrivingScheme->transmitPulse.pRawPulseAmplitudes );
    free( pInst->pDrivingScheme->transmitPulse.pRawPulseTimes );
    free( pInst->pDrivingScheme->pTransmitApodization );
    free( pInst->pDrivingScheme->pSourceLocations );
    if ( pInst->pDrivingScheme->transmitChannelCoding.numChannels != 0 )
    {
        free( pInst->pDrivingScheme->transmitChannelCoding.pChannelMap );
    }
    free( pInst->pDrivingScheme );

    free( pInst );
}

