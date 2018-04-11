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
#include <include/iusHLNode.h>
#include <include/iusInput.h>
#include <include/ius.h>
#include <include/iusHLExperiment.h>
#include <include/iusHLInput.h>
#include <assert.h>

#if USED
//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int LF_GetNumberOfElements(IusInputInstance* pInst)
{
//    if ( pInst->pDrivingScheme->transmitChannelCoding.numChannels == 0 )
//    {
//        //we use all transducer elements
//        return pInst->pTransducer->numElements;
//    }
//    else
//    {
//        // we use only th number of channels
//        return pInst->pDrivingScheme->transmitChannelCoding.numChannels;
//    }
    return -1;
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
//    // transducerName is a descriptive name of the ultrasound probe
//    pDst->pTransducerName =
//        (char *)calloc( strlen(pSrc->pTransducerName) + 1, sizeof(char) );
//
//    if (pDst->pTransducerName == NULL)
//    {
//        return 1 << 2;
//    }
//    strcpy( pDst->pTransducerName, pSrc->pTransducerName );
//
//    // centerFrequency: operating frequency of the transducer in Hz
//    // numElements    : number of transducer Elements in the probe
//    pDst->centerFrequency = pSrc->centerFrequency;
//    pDst->numElements     = pSrc->numElements;
//
//    pDst->pElements = (IusTransducerElement *)calloc( pDst->numElements,
//        sizeof( IusTransducerElement ) );
//
//    if ( pDst->pElements == NULL )
//    {
//        return 2 << 2;
//    }
//
//    // an array of numElements transducer element (position, angle, size)
//    memcpy( pDst->pElements, pSrc->pElements,
//        pDst->numElements * sizeof( IusTransducerElement ) );

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

    pDst->pStartDelay      = (float *)calloc( numPulsesPerFrame, sizeof(float) );
    if ( pDst->pStartDelay == NULL )
    {
        return 2 << 4;
    }
    memcpy( pDst->pStartDelay, pSrc->pStartDelay,
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
//    int i;
//    IUS_ASSERT( numPulsesPerFrame > 0 );
//
//    pDst->drivingSchemeType  = pSrc->drivingSchemeType;   // driving scheme: e.g. diveringwaves, planeswaves, ...
//    pDst->numSamplesPerLine  = pSrc->numSamplesPerLine;   // length of an acquisition line
//    pDst->numTransmitSources = pSrc->numTransmitSources;  // number of US sources (tyically these are virtual)
//    pDst->numTransmitPulses  = pSrc->numTransmitPulses;   // number of pulses in a frame
//
//    if ( pDst->numTransmitSources != 0 && pSrc->pSourceLocations != NULL )
//    {
//	pDst->pSourceLocations = (IusPosition *)calloc(pDst->numTransmitSources, sizeof(IusPosition));
//	for (i = 0; i < pDst->numTransmitSources; i++)
//	{
//	    pDst->pSourceLocations[i].x = pSrc->pSourceLocations[i].x;
//	    pDst->pSourceLocations[i].y = pSrc->pSourceLocations[i].y;
//	    pDst->pSourceLocations[i].z = pSrc->pSourceLocations[i].z;
//	}
//    }
//
//    pDst->sourceFNumber        = pSrc->sourceFNumber;        // distance in [m] of sources to transducer for POLAR
//    pDst->sourceDeltaTheta   = pSrc->sourceDeltaTheta;   // angle in [rad] between sources
//    pDst->sourceStartTheta     = pSrc->sourceStartTheta;     // starting angle in [rad] for the sources
//    pDst->transmitPatternDelay = pSrc->transmitPatternDelay; // extra delay at the end of a transmit pattern
//
//    pDst->pTransmitPattern = (IusTransmitPattern *)calloc( numPulsesPerFrame, sizeof(IusTransmitPattern) );
//    if ( pDst->pTransmitPattern == 0 )
//    {
//        return 1 << 7;
//    }
//    memcpy( pDst->pTransmitPattern, pSrc->pTransmitPattern,
//            numPulsesPerFrame * sizeof( IusTransmitPattern ) );  // array (time, index) of length numTransmitPulses
//
//    pDst->pTransmitPulse.numPulseValues = pSrc->pTransmitPulse.numPulseValues;  // waveform of the transmit pulse
//    if ( pDst->pTransmitPulse.numPulseValues != 0 )
//    {
//        pDst->pTransmitPulse.pRawPulseAmplitudes = (float *)calloc( pDst->pTransmitPulse.numPulseValues, sizeof(float));       /* shape of waveform [in Volts] */
//        pDst->pTransmitPulse.pRawPulseTimes      = (float *)calloc( pDst->pTransmitPulse.numPulseValues, sizeof(float));            /* corresponding timestamps of amplitudes [in seconds] */
//        if ( pDst->pTransmitPulse.pRawPulseAmplitudes == NULL || pDst->pTransmitPulse.pRawPulseTimes == 0)
//        {
//            return 2 << 7;
//        }
//        memcpy( pDst->pTransmitPulse.pRawPulseAmplitudes, pSrc->pTransmitPulse.pRawPulseAmplitudes, pDst->pTransmitPulse.numPulseValues*sizeof(float));
//        memcpy( pDst->pTransmitPulse.pRawPulseTimes,      pSrc->pTransmitPulse.pRawPulseTimes,      pDst->pTransmitPulse.numPulseValues*sizeof(float));
//    }
//    pDst->pTransmitPulse.pulseFrequency = pSrc->pTransmitPulse.pulseFrequency;
//    pDst->pTransmitPulse.pulseAmplitude = pSrc->pTransmitPulse.pulseAmplitude;
//    pDst->pTransmitPulse.pulseCount     = pSrc->pTransmitPulse.pulseCount;
//
//    pDst->pTransmitApodization = (float *)calloc( numPulsesPerFrame * numElements, sizeof(float) );   // 2D array: per transmitted pulse we have numElement gains
//                                                                                                      // (which are numTransducerElements or numChannelElements)
//    if ( pDst->pTransmitApodization == NULL )
//    {
//        return 3<<7;
//    }
//    memcpy( pDst->pTransmitApodization, pSrc->pTransmitApodization,
//            numPulsesPerFrame * numElements * sizeof(float) );
//
//    if ( pSrc->transmitChannelCoding.numChannels != 0 )
//    {
//        pDst->transmitChannelCoding.numChannels = pSrc->transmitChannelCoding.numChannels;
//        pDst->transmitChannelCoding.pChannelMap = (int *)calloc(pDst->transmitChannelCoding.numChannels*numPulsesPerFrame, sizeof(int));
//        if ( pDst->transmitChannelCoding.pChannelMap == NULL )
//        {
//            return 4<<7;
//        }
//        memcpy( pDst->transmitChannelCoding.pChannelMap,
//                pSrc->transmitChannelCoding.pChannelMap,
//                pDst->transmitChannelCoding.numChannels * numPulsesPerFrame * sizeof(int) );
//    }
//    else
//    {
//        pDst->transmitChannelCoding.numChannels = 0;
//        pDst->transmitChannelCoding.pChannelMap = NULL;
//    }
//
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
//  IusPosition * pPosData;
//  IusAngle *    pAngleData;
//  IusSize *     pSizeData;
//  hid_t         position_tid; //position type id
//  hid_t         angle_tid;    //angle type id
//  hid_t         size_tid;     //size id
//
  herr_t status = 0;
//  IUS_UNUSED(verbose); // avoid compiler warning
//
//  dataset = H5Dopen(handle, "/Transducer/Elements/positions", H5P_DEFAULT);
//  space = H5Dget_space(dataset);
//  ndims = H5Sget_simple_extent_dims(space, dims, NULL);
//  if ( ndims != 1 )
//  {
//    fprintf( stderr, "readTransElements: Only 1D array of transducer elements supported \n" );
//    return -1;
//  }
//
//  pPosData   = (IusPosition *) malloc( (size_t)(dims[0] * sizeof (IusPosition)) );
//  pAngleData = (IusAngle *)    malloc( (size_t)(dims[0] * sizeof (IusAngle)) );
//  pSizeData  = (IusSize *)     malloc( (size_t)(dims[0] * sizeof (IusSize)) );
//  pInst->pTransducer->pElements = (IusTransducerElement *)malloc((size_t)(dims[0]*sizeof(IusTransducerElement)));
//  if (pInst->pTransducer->pElements == NULL)
//  {
//      fprintf(stderr, "Allocation failed: pInst->pTransducer->pElements\n");
//    return -1;
//  }
//
//  position_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusPosition));
//  H5Tinsert(position_tid, "x", HOFFSET(IusPosition, x), H5T_NATIVE_FLOAT);
//  H5Tinsert(position_tid, "y", HOFFSET(IusPosition, y), H5T_NATIVE_FLOAT);
//  H5Tinsert(position_tid, "z", HOFFSET(IusPosition, z), H5T_NATIVE_FLOAT);
//  status = H5Dread(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPosData);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
//    return status;
//  }
//  status = H5Dclose(dataset);
//  status = H5Sclose(space);
//  status = H5Tclose(position_tid);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error closing position dataset: %d\n", status);
//    return status;
//  }
//
//  dataset = H5Dopen(handle, "/Transducer/Elements/angles", H5P_DEFAULT);
//  space = H5Dget_space(dataset);
//  //read the angles array
//  angle_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusAngle));
//  H5Tinsert(angle_tid, "theta", HOFFSET(IusAngle, theta), H5T_NATIVE_FLOAT);
//  H5Tinsert(angle_tid, "phi",   HOFFSET(IusAngle, phi),   H5T_NATIVE_FLOAT);
//  status = H5Dread(dataset, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleData);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
//    return status;
//  }
//  status = H5Dclose(dataset);
//  status = H5Sclose(space);
//  status = H5Tclose(angle_tid);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error closing angle dataset: %d\n", status);
//    return status;
//  }
//
//  dataset = H5Dopen(handle, "/Transducer/Elements/sizes", H5P_DEFAULT);
//  space = H5Dget_space(dataset);
//  //read the angles array
//  size_tid = H5Tcreate (H5T_COMPOUND, sizeof(IusSize));
//  H5Tinsert(size_tid, "x", HOFFSET(IusSize, x), H5T_NATIVE_FLOAT);
//  H5Tinsert(size_tid, "y", HOFFSET(IusSize, y), H5T_NATIVE_FLOAT);
//  H5Tinsert(size_tid, "z", HOFFSET(IusSize, z), H5T_NATIVE_FLOAT);
//  status = H5Dread(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeData);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error reading data: H5Dread returned: %d\n", status);
//    return status;
//  }
//  status = H5Dclose(dataset);
//  status = H5Sclose(space);
//  status = H5Tclose(size_tid);
//  if (status < 0)
//  {
//    fprintf(stderr, "readTransElements: Error closing size dataset: %d\n", status);
//    return status;
//  }
//
//  for ( i = 0; i < dims[0]; i++ )
//  {
//    pInst->pTransducer->pElements[i].position.x = pPosData[i].x;
//    pInst->pTransducer->pElements[i].position.y = pPosData[i].y;
//    pInst->pTransducer->pElements[i].position.z = pPosData[i].z;
//    pInst->pTransducer->pElements[i].angle.theta= pAngleData[i].theta;
//    pInst->pTransducer->pElements[i].angle.phi  = pAngleData[i].phi;
//    pInst->pTransducer->pElements[i].size.x     = pSizeData[i].x;
//    pInst->pTransducer->pElements[i].size.y     = pSizeData[i].y;
//    pInst->pTransducer->pElements[i].size.z     = pSizeData[i].z;
//  }
//  free(pPosData);
//  free(pAngleData);
//  free(pSizeData);

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
//
//    int           i, ndims;
//    hid_t         dataset, space;
//    hsize_t       dims[4];  // NOTE: pElements assumed 1D array, extra length to prevent
//    Ius3DPosition * pPosData;
//    hid_t         position_tid; //position type id
//
//    int numLocations = pInst->pDrivingScheme->numTransmitSources;
//
//    IUS_UNUSED(verbose); // avoid compiler warnings
//
//    dataset = H5Dopen( handle, "/DrivingScheme/sourceLocations", H5P_DEFAULT );
//    space   = H5Dget_space( dataset );
//    ndims   = H5Sget_simple_extent_dims( space, dims, NULL );
//    if ( ndims != 1 )
//    {
//        fprintf( stderr, "Rank should be 1, but found %d\n", ndims );
//        return 1;
//    }
//    pPosData   = (Ius3DPosition *) malloc( (size_t)(dims[0] * sizeof(Ius3DPosition) ) );
//    pInst->pDrivingScheme->pSourceLocations = (Ius3DPosition *)malloc( (size_t)(dims[0] * sizeof(Ius3DPosition)) );
//    if ( pInst->pDrivingScheme->pSourceLocations == NULL )
//    {
//        fprintf( stderr, "LF_readSourceLocationsCartesian: Error allocating data\n" );
//        free( pPosData );
//        return -1;
//    }
//
//    //read the positions array
//    position_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius3DPosition));
//    H5Tinsert( position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT );
//    H5Tinsert( position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT );
//    H5Tinsert( position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT );
//    status = H5Dread( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPosData );
//    if ( status < 0 )
//    {
//        fprintf( stderr, "LF_readSourceLocationsCartesian: Error reading dataset: H5Dread returned: %d\n", status );
//        free( pPosData );
//        return status;
//    }
//
//    for ( i = 0; i < numLocations; i++ )
//    {
//        pInst->pDrivingScheme->pSourceLocations[i].x = pPosData[i].x;
//        pInst->pDrivingScheme->pSourceLocations[i].y = pPosData[i].y;
//        pInst->pDrivingScheme->pSourceLocations[i].z = pPosData[i].z;
//    }
//    free( pPosData );
//
//    status = H5Dclose( dataset );
//    status = H5Sclose( space );
//    status = H5Tclose( position_tid );
//
    return -1;
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

    pInst->pDrivingScheme->pTransmitPulse.pRawPulseAmplitudes = (float *) malloc(numAmplitudes * sizeof(float));
    pInst->pDrivingScheme->pTransmitPulse.pRawPulseTimes = (float *) malloc(numAmplitudes * sizeof(float));
    if ( pInst->pDrivingScheme->pTransmitPulse.pRawPulseAmplitudes == NULL ||
         pInst->pDrivingScheme->pTransmitPulse.pRawPulseTimes == NULL )
    {
        fprintf( stderr, "LF_readPulseRaw: Error allocating data\n" );
        return -1;
    }

    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/rawPulseAmplitudes", pInst->pDrivingScheme->pTransmitPulse.pRawPulseAmplitudes );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/rawPulseTimes", pInst->pDrivingScheme->pTransmitPulse.pRawPulseTimes );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/pulseFrequency", &(pInst->pDrivingScheme->pTransmitPulse.pulseFrequency) );
    status = H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPulse/pulseAmplitude", &(pInst->pDrivingScheme->pTransmitPulse.pulseAmplitude) );
    status = H5LTread_dataset_int( handle, "/DrivingScheme/TransmitPulse/pulseCount", &(pInst->pDrivingScheme->pTransmitPulse.pulseCount) );
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



IusInputInstance * iusInputCreateOld
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
   
    numElements = LF_GetNumberOfElements(pInst);

    copyStatus |= LF_copyDrivingSchemeData(pInst->pDrivingScheme, pDrivingScheme, pDrivingScheme->numTransmitPulses, numElements);

    if ( copyStatus != 0 )
    {
        fprintf(stderr, "iusInputCreate: deep copy of data instance failed: status is %d\n", copyStatus);
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
    pInst->numFrames = numFrames ;
    return pInst;
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
    free( ((Ius3DTransducer *)pInst->pTransducer)->pElements );
    free( pInst->pTransducer );

    if ( pInst->pReceiveSettings->receiveChannelCoding.numChannels != 0 )
    {
        free( pInst->pReceiveSettings->receiveChannelCoding.pChannelMap );
    }
    free( pInst->pReceiveSettings->pStartDelay );
    free( pInst->pReceiveSettings->pEndDepth );
    free( pInst->pReceiveSettings->pTimeGainControl );
    free( pInst->pReceiveSettings );

    free( pInst->pDrivingScheme->pTransmitPattern );
    free( pInst->pDrivingScheme->pTransmitPulse.pRawPulseAmplitudes );
    free( pInst->pDrivingScheme->pTransmitPulse.pRawPulseTimes );
    free( pInst->pDrivingScheme->pTransmitApodization );
    free( pInst->pDrivingScheme->pSourceLocations );
    if ( pInst->pDrivingScheme->transmitChannelCoding.numChannels != 0 )
    {
        free( pInst->pDrivingScheme->transmitChannelCoding.pChannelMap );
    }
    free( pInst->pDrivingScheme );

    free( pInst );
}

#endif


int iusWriteExperiment(IusExperiment *pExperiment, hid_t handle, int verbose) {
    herr_t status = 0;
    /* write the /Experiment data */
    status |= iusHdf5WriteFloat( handle , "/Experiment/speedOfSound",    &(pExperiment->speedOfSound), 1,   verbose );
    status |= iusHdf5WriteInt( handle , "/Experiment/date", &(pExperiment->date), 1,   verbose );
    status |= iusHdf5WriteString( handle , "/Experiment/description", pExperiment->pDescription, 1,   verbose );
    return status;
}

int iusWrite3DTransducerElementPositions(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t position_tid; // File datatype identifier for IusPosition
    hid_t dataset, space;
    hsize_t dims[1] = {1};
    Ius3DPosition * pPositionArray;
    int numElements = pTransducer->baseTransducer.numElements;
    int i; //iterator

    /* write the /Transducer/Elements/ positions, angles and sizes are compound types */
    dims[0] = numElements;

    //Positions
    space = H5Screate_simple( 1, dims, NULL );
    // step a:  create H5 dataset
    position_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius3DPosition) );
    status |= H5Tinsert( position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT );
    dataset = H5Dcreate( subgroup_id,    "/Transducer/Elements/positions", position_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

    // step b:  create array of positions
    pPositionArray = (Ius3DPosition *)calloc( numElements, sizeof(Ius3DPosition) ); //three dimensions for position
    for ( i = 0; i < numElements; i++ )
    {
        pPositionArray[i] = pTransducer->pElements[i].position;
    }

    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPositionArray );

    free( pPositionArray );

    // step d: release resources
    status |= H5Tclose( position_tid );
    status |= H5Sclose( space );
    status |= H5Dclose( dataset );
    return status;
}

int iusWrite3DTransducerElementSizes(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t size_tid; // File datatype identifier for IusPosition
    hid_t dataset, space;
    hsize_t dims[1] = {1};
    Ius3DSize * pSizeArray;
    int numElements = pTransducer->baseTransducer.numElements;
    int i; //iterator

    //
    space = H5Screate_simple(1, dims, NULL);

    // step a:  create H5 dataset
    size_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius3DSize));
    status |= H5Tinsert( size_tid, "sx", HOFFSET(Ius3DSize, sx), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( size_tid, "sy", HOFFSET(Ius3DSize, sy), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( size_tid, "sz", HOFFSET(Ius3DSize, sz), H5T_NATIVE_FLOAT );
    dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/sizes",
                         size_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // step b:  create array of sizes
    pSizeArray = (Ius3DSize *)calloc( numElements, sizeof(Ius3DSize) ); //three dimensions for size
    for ( i = 0; i < numElements; i++ )
    {
        pSizeArray[i] = pTransducer->pElements[i].size;
    }
    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeArray );

    // step d: release resources
    free( pSizeArray );
    status |= H5Tclose( size_tid );
    status |= H5Sclose( space );
    status |= H5Dclose( dataset );
    return status;
}

int iusWrite3DTransducerElementAngles(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t angle_tid; // File datatype identifier for IusAngle
    hid_t dataset, space;
    hsize_t dims[1] = {1};
    Ius3DAngle * pAngleArray;
    int numElements = pTransducer->baseTransducer.numElements;
    int i; //iterator

    // Angles
    space = H5Screate_simple( 1, dims, NULL );
    // step a:  create H5 dataset
    angle_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius3DAngle) );
    status |= H5Tinsert( angle_tid, "theta", HOFFSET(Ius3DAngle, theta), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( angle_tid,  "phi", HOFFSET(Ius3DAngle, phi), H5T_NATIVE_FLOAT );
    dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/angles", angle_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

    // step b:  create array of angles
    pAngleArray = (Ius3DAngle *)calloc( numElements, sizeof(Ius3DAngle) ); //two dimensions for angle
    for (i = 0; i < numElements; i++)
    {
        pAngleArray[i] = pTransducer->pElements[i].angle;
    }

    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleArray );

    free(pAngleArray);
    // step d: release resources
    status |= H5Tclose(angle_tid);
    status |= H5Sclose(space);
    status |= H5Dclose(dataset);
    return status;
}

int iusWrite3DTransducer(Ius3DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    status |= iusWrite3DTransducerElementPositions(pTransducer,subgroup_id,verbose);
    status |= iusWrite3DTransducerElementSizes(pTransducer,subgroup_id,verbose);
    status |= iusWrite3DTransducerElementAngles(pTransducer,subgroup_id,verbose);
    return status;
}



int iusWrite2DTransducerElementPositions(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t position_tid; // File datatype identifier for IusPosition
    hid_t dataset, space;
    hsize_t dims[1] = {1};
    Ius2DPosition * pPositionArray;
    int numElements = pTransducer->baseTransducer.numElements;
    int i; //iterator

    /* write the /Transducer/Elements/ positions, angles and sizes are compound types */
    dims[0] = numElements;

    //Positions
    space = H5Screate_simple( 1, dims, NULL );
    // step a:  create H5 dataset
    position_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius2DPosition) );
    status |= H5Tinsert( position_tid, "x", HOFFSET(Ius2DPosition, x), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( position_tid, "z", HOFFSET(Ius2DPosition, z), H5T_NATIVE_FLOAT );
    dataset = H5Dcreate( subgroup_id,    "/Transducer/Elements/positions", position_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

    // step b:  create array of positions
    pPositionArray = (Ius2DPosition *)calloc( numElements, sizeof(Ius2DPosition) ); //three dimensions for position
    for ( i = 0; i < numElements; i++ )
    {
        pPositionArray[i] = pTransducer->pElements[i].position;
    }

    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pPositionArray );

    free( pPositionArray );

    // step d: release resources
    status |= H5Tclose( position_tid );
    status |= H5Sclose( space );
    status |= H5Dclose( dataset );
    return status;
}

int iusWrite2DTransducerElementSizes(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t size_tid; // File datatype identifier for IusPosition
    hid_t dataset, space;
    hsize_t dims[1] = {1};
    Ius2DSize * pSizeArray;
    int numElements = pTransducer->baseTransducer.numElements;
    int i; //iterator

    //
    space = H5Screate_simple(1, dims, NULL);

    // step a:  create H5 dataset
    size_tid = H5Tcreate(H5T_COMPOUND, sizeof(Ius2DSize));
    status |= H5Tinsert( size_tid, "sx", HOFFSET(Ius2DSize, sx), H5T_NATIVE_FLOAT );
    status |= H5Tinsert( size_tid, "sz", HOFFSET(Ius2DSize, sz), H5T_NATIVE_FLOAT );
    dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/sizes",
                         size_tid, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // step b:  create array of sizes
    pSizeArray = (Ius2DSize *)calloc( numElements, sizeof(Ius2DSize) ); //three dimensions for size
    for ( i = 0; i < numElements; i++ )
    {
        pSizeArray[i] = pTransducer->pElements[i].size;
    }
    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT, pSizeArray );

    // step d: release resources
    free( pSizeArray );
    status |= H5Tclose( size_tid );
    status |= H5Sclose( space );
    status |= H5Dclose( dataset );
    return status;
}

int iusWrite2DTransducerElementAngles(Ius2DTransducer *pTransducer, hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    hid_t dataset, space;
    float * pAngleArray;
    int numElements = pTransducer->baseTransducer.numElements;
    hsize_t dims[1] = {numElements};
    int i; //iterator

    // Angles
    space = H5Screate_simple( 1, dims, NULL );
    // step a:  create H5 dataset
    dataset = H5Dcreate( subgroup_id, "/Transducer/Elements/theta", H5T_NATIVE_FLOAT, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );

    // step b:  create array of angles
    pAngleArray = (float *)calloc( numElements, sizeof(float) ); //two dimensions for angle
    for (i = 0; i < numElements; i++)
    {
        pAngleArray[i] = pTransducer->pElements[i].theta;
    }

    // step c: write the array to the dataset
    status |= H5Dwrite( dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, pAngleArray );

    free(pAngleArray);
    // step d: release resources
    status |= H5Sclose(space);
    status |= H5Dclose(dataset);
    return status;
}

int iusWrite2DTransducer(Ius2DTransducer *pTransducer,  hid_t subgroup_id, int verbose)
{
    herr_t        status=0;
    status |= iusWrite2DTransducerElementPositions(pTransducer,subgroup_id,verbose);
    status |= iusWrite2DTransducerElementSizes(pTransducer,subgroup_id,verbose);
    status |= iusWrite2DTransducerElementAngles(pTransducer,subgroup_id,verbose);
    return status;
}

herr_t iusWriteShape(hid_t group_id, char *pVariableString, IusTransducerShape shape, int verbose)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_shapeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_LINE,     (enumValue=IUS_LINE,     &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE,   (enumValue=IUS_CIRCLE,   &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_PLANE,    (enumValue=IUS_PLANE,    &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue=IUS_CYLINDER, &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_SPHERE,   (enumValue=IUS_SPHERE,   &enumValue) );
    enumValue = shape;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_shapeType, &enumValue );
    return status;
}

int iusWriteBaseTransducer(IusTransducer *pTransducer, hid_t groupd_id, int verbose)
{
    herr_t status=0;
    status |= iusWriteShape(groupd_id,"/Transducer/shape", pTransducer->shape,verbose);
    status |= iusHdf5WriteString( groupd_id,"/Transducer/transducerName", pTransducer->pTransducerName,1,verbose);
    status |= iusHdf5WriteFloat( groupd_id, "/Transducer/centerFrequency", &(pTransducer->centerFrequency),1,verbose);
    status |= iusHdf5WriteInt( groupd_id, "/Transducer/numElements", &(pTransducer->numElements),1,verbose);
    return status;
}

int iusWriteTransducer(IusTransducer *pTransducer, hid_t group_id, int verbose) {
    /* write the /Transducer data */
    herr_t        status;
    hsize_t dims[1] = {1};
    hid_t subgroup_id;


    status = iusWriteBaseTransducer(pTransducer,group_id,verbose);
    if( status < 0 )
    {
        return status;
    }

    subgroup_id = H5Gcreate(group_id, "/Transducer/Elements", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    if( pTransducer->type == IUS_3D_SHAPE )
    {
        Ius3DTransducer *p3DTransducer = (Ius3DTransducer *) pTransducer;
        status |= iusWrite3DTransducer(p3DTransducer,subgroup_id,verbose);
    }

    if( pTransducer->type == IUS_2D_SHAPE )
    {
        Ius2DTransducer *p2DTransducer = (Ius2DTransducer *) pTransducer;
        status |= iusWrite2DTransducer(p2DTransducer,subgroup_id,verbose);
    }
    status |= H5Gclose( subgroup_id );

    return status;

}


herr_t iusWriteDrivingSchemeType(hid_t group_id, char *pVariableString, IusDrivingSchemeType type, int verbose)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_drivingSchemeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_drivingSchemeType,  DRIVINGSCHEME_DIVERGING_WAVES_PARAMETRIZED,
                              (enumValue=IUS_DIVERGING_WAVES_PARAMETRIZED, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_DIVERGING_WAVES,
                              (enumValue=IUS_DIVERGING_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_FOCUSED_WAVES_PARAMETRIZED,
                              (enumValue=IUS_FOCUSED_WAVES_PARAMETRIZED, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_FOCUSED_WAVES,
                              (enumValue=IUS_FOCUSED_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_PLANE_WAVES,
                              (enumValue=IUS_PLANE_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_SINGLE_ELEMENT,
                              (enumValue=IUS_SINGLE_ELEMENT, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_CUSTOM_WAVES,
                              (enumValue=IUS_CUSTOM_WAVES, &enumValue) );

    enumValue = type;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_drivingSchemeType, &enumValue );
    return status;
}

herr_t iusWriteTransmitPattern(IusDrivingScheme *pDrivingScheme, hid_t subgroup_id, int verbose)
{
    int i;
    int status = 0;
    hsize_t dims[1] = {pDrivingScheme->numTransmitPulses};

    status |= iusHdf5WriteFloat(subgroup_id, "/DrivingScheme/TransmitPattern/transmitPatternDelay",  &pDrivingScheme->transmitPatternDelay, 1, verbose);

    float *pFloatArray = (float *)calloc( (size_t)dims[0], sizeof(float) );
    if( pFloatArray == NULL )
        return -1;

    int *pIntArray = (int *)calloc( (size_t)dims[0], sizeof(int) );
    if( pIntArray == NULL )
    {
        free(pFloatArray);
        return -1;
    }

    for ( i = 0; i < dims[0]; i++ )
    {
        pFloatArray[i] = pDrivingScheme->pTransmitPatterns->pTransmitPattern[i].time;
        pIntArray[i] = pDrivingScheme->pTransmitPatterns->pTransmitPattern[i].index;
    }

    status |= H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPattern/transmitPatternTime",  1, dims, pFloatArray );
    status |= H5LTmake_dataset_int( subgroup_id,   "/DrivingScheme/TransmitPattern/transmitPatternIndex",  1, dims, pIntArray );

    free( pFloatArray );
    free( pIntArray );
    return status;
}


herr_t iusWriteDrivingSchemeTransmitApodization(hid_t group_id, char *pVariableString, IusDrivingScheme *pDrivingScheme, int verbose)
{
    hsize_t apodDims[2] = {pDrivingScheme->numTransmitPulses,  pDrivingScheme->numElements};
    herr_t status = H5LTmake_dataset_float( group_id,"/DrivingScheme/transmitApodization",  2, apodDims, pDrivingScheme->pTransmitApodization );
    return status;
}

herr_t iusWriteBaseDrivingScheme(IusDrivingScheme *pDrivingScheme, hid_t group_id, int verbose)
{
    herr_t  status;
    status |= iusWriteDrivingSchemeType(group_id, "/DrivingScheme/drivingSchemeType", pDrivingScheme->type,verbose);
    status |= iusWriteDrivingSchemeTransmitApodization(group_id, "/DrivingScheme/transmitApodization", pDrivingScheme,verbose);
    status |= iusHdf5WriteInt(group_id, "/DrivingScheme/numTransmitSources",  &pDrivingScheme->numTransmitSources, 1, verbose);
    status |= iusHdf5WriteInt(group_id, "/DrivingScheme/numTransmitPulses",  &pDrivingScheme->numTransmitPulses, 1, verbose);
    status |= iusHdf5WriteInt(group_id, "/DrivingScheme/numElements",  &pDrivingScheme->numElements, 1, verbose);

    hid_t subgroup_id = H5Gcreate(group_id, "/DrivingScheme/TransmitPattern/", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT );
    status |= iusWriteTransmitPattern(pDrivingScheme, subgroup_id, verbose);
    H5Gclose( subgroup_id );
    return status;
}

int iusWriteDrivingScheme(IusDrivingScheme *pDrivingScheme, hid_t group_id, int verbose)
{
    /* write the /Transducer data */
    herr_t        status=0;
    hsize_t dims[1] = {1};


    status = iusWriteBaseDrivingScheme(pDrivingScheme,group_id,verbose);
    if( status < 0 )
    {
        return status;
    }
    return status;
}

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
int iusInputWrite
(
    hid_t handle,
    IusInputInstance *pInst,
    int verbose
)
{
#if old
    Ius3DPosition * pPositionArray;
    IusAngle *    pAngleArray;
    IusSize *     pSizeArray;
    float *       pFloatArray;
    int *         pIntArray;
    const int     libVersion = version;

    hid_t position_tid; // File datatype identifier for Ius3DPosition
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
        position_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius3DPosition) );
        H5Tinsert( position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT );
        dataset = H5Dcreate( subgroup_id,    "/Transducer/Elements/positions", position_tid, space, H5P_DEFAULT, propList, H5P_DEFAULT );

        // step b:  create array of positions
        pPositionArray = (Ius3DPosition *)calloc( pInst->pTransducer->numElements, sizeof(Ius3DPosition) ); //three dimensions for position
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
        H5Tinsert( angle_tid,  "theta", HOFFSET(IusAngle, theta), H5T_NATIVE_FLOAT );
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
      //  pReceiveSettings->pStartDelay + (pDrivingScheme->numSamplesPerLine * pReceiveSettings->sampleFrequency);
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
      H5LTmake_dataset_float( group_id,    "/ReceiveSettings/startDepth", 1, dims, pInst->pReceiveSettings->pStartDelay );
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

    dims[0] = 1;
    H5LTmake_dataset_int(group_id, "/DrivingScheme/numChannels", 1, dims, &(pInst->pDrivingScheme->transmitChannelCoding.numChannels));
    if(pInst->pDrivingScheme->transmitChannelCoding.numChannels > 0)
    {
        dims[0] = pInst->pDrivingScheme->transmitChannelCoding.numChannels;
	H5LTmake_dataset_int(group_id, "/DrivingScheme/channelMap", 1, dims, pInst->pDrivingScheme->transmitChannelCoding.pChannelMap);
    }

    // Is there any reason to make these datafields conditional, ON THE FILE-IO level???
    // Spoiler: I don't think so.
    if (enumValue == IUS_DIVERGING_WAVES) //use radial info
    {
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceFNumber",      1, dims, &(pInst->pDrivingScheme->sourceFNumber) );
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceDeltaTheta", 1, dims, &(pInst->pDrivingScheme->sourceDeltaTheta) );
        H5LTmake_dataset_float( group_id, "/DrivingScheme/sourceStartTheta",   1, dims, &(pInst->pDrivingScheme->sourceStartTheta) );
    }
    else
    {
        dims[0] = pInst->pDrivingScheme->numTransmitSources;
        space   = H5Screate_simple( 1, dims, NULL );
        // step a:  create H5 dataset
        position_tid = H5Tcreate( H5T_COMPOUND, sizeof(Ius3DPosition) );
        H5Tinsert( position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT );
        H5Tinsert( position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT );
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
    H5LTmake_dataset_int( subgroup_id,   "/DrivingScheme/TransmitPulse/numPulseValues",  1, dims, &(pInst->pDrivingScheme->pTransmitPulse.numPulseValues) );
    if ( pInst->pDrivingScheme->pTransmitPulse.numPulseValues > 0 )
    {
        dims[0] = pInst->pDrivingScheme->pTransmitPulse.numPulseValues;
        H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPulse/rawPulseAmplitudes", 1, dims, pInst->pDrivingScheme->pTransmitPulse.pRawPulseAmplitudes );
        H5LTmake_dataset_float( subgroup_id, "/DrivingScheme/TransmitPulse/rawPulseTimes", 1, dims, pInst->pDrivingScheme->pTransmitPulse.pRawPulseTimes );
    }

    dims[0] = 1;
    H5LTmake_dataset_float( subgroup_id,"/DrivingScheme/TransmitPulse/pulseFrequency",  1, dims, &(pInst->pDrivingScheme->pTransmitPulse.pulseFrequency) );
    H5LTmake_dataset_float( subgroup_id,"/DrivingScheme/TransmitPulse/pulseAmplitude",  1, dims, &(pInst->pDrivingScheme->pTransmitPulse.pulseAmplitude) );
    H5LTmake_dataset_int( subgroup_id,  "/DrivingScheme/TransmitPulse/pulseCount",  1, dims, &(pInst->pDrivingScheme->pTransmitPulse.pulseCount) );
    H5Gclose( subgroup_id );
    apodDims[0] = pInst->pDrivingScheme->numTransmitPulses;
    apodDims[1] = pInst->pTransducer->numElements;
    H5LTmake_dataset_float( group_id,"/DrivingScheme/transmitApodization",  2, apodDims, pInst->pDrivingScheme->pTransmitApodization );

    H5Gclose( group_id );
#endif // old
    herr_t        status=0;
    hsize_t dims[1] = {1};
    if ( handle == 0 || pInst == NULL )
    {
        fprintf( stderr, "iusInputWrite: Input arguments can not be NULL \n");
        return -1;
    }

    // Make dataset for Node
    status |=iusWriteNode(&pInst->iusNode, handle, verbose);

    // Make dataset for input type
    status |=H5LTmake_dataset_int( handle,    "/IusVersion", 1, dims, &(pInst->IusVersion));
    status |=H5LTmake_dataset_int( handle,    "/numFrames",  1, dims, &(pInst->numFrames) );

    // Make dataset for Experiment
    hid_t group_id = H5Gcreate(handle, "/Experiment", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusWriteExperiment(pInst->pExperiment, handle, verbose);
    status |= H5Gclose(group_id );

    // Make dataset for Transducer
    group_id = H5Gcreate(handle, "/Transducer", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iusWriteTransducer(pInst->pTransducer, group_id, verbose);
    status |= H5Gclose(group_id );


    // Make dataset for DrivingScheme
    group_id = H5Gcreate(handle, "/DrivingScheme", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iusWriteDrivingScheme(pInst->pDrivingScheme, group_id, verbose);
    status |= H5Gclose(group_id );

    return status;
}



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
IusInputInstance * iusInputCreate( IusNode *node, int  numFrames )
{
    //--------------------------------------------------------------------------
    // alloc instance ; using calloc to clear all state.
    //--------------------------------------------------------------------------
    IusInputInstance * pInst;
    pInst = (IusInputInstance *)calloc( 1, sizeof( IusInputInstance ) );
    if ( pInst == NULL )
    {
        fprintf( stderr, "iusInputCreate: calloc of data instance failed\n" );
        return NULL;
    }

    memcpy( &pInst->iusNode, node, sizeof(IusNode));
    pInst->numFrames = numFrames;
    pInst->IusVersion = iusGetVersionMajor();
    return pInst;
};



IusExperiment *iusReadExperiment(hid_t handle, int verbose) {
    int status = 0;
    float speedOfSound;
    int date;
    char *pDescription;
    iue_t experiment;
    status |= iusHdf5ReadFloat( handle , "/Experiment/speedOfSound",    &(speedOfSound),    verbose );
    status |= iusHdf5ReadInt( handle,    "/Experiment/date",            &(date),            verbose );
    status |= iusHdf5ReadString( handle, "/Experiment/description",     &(pDescription),    verbose );

    if( status < 0 )
        return NULL;
    experiment = iusHLCreateExperiment(speedOfSound,date,pDescription);
    return experiment;
}



herr_t iusReadShape(hid_t handle, char *pVariableString, IusTransducerShape *pShape, int verbose)
{
    herr_t status = 0;
    hid_t hdf_shapeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_LINE,     (enumValue=IUS_LINE,     &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_CIRCLE,   (enumValue=IUS_CIRCLE,   &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_PLANE,    (enumValue=IUS_PLANE,    &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_CYLINDER, (enumValue=IUS_CYLINDER, &enumValue) );
    status |= H5Tenum_insert( hdf_shapeType, TRANSDUCER_SHAPE_SPHERE,   (enumValue=IUS_SPHERE,   &enumValue) );
    status |= H5LTread_dataset( handle, pVariableString , hdf_shapeType, pShape );

    return status;
}

IusTransducer *iusReadBaseTransducer(hid_t handle, int verbose)
{
    int status = 0;
    float centerFrequency;
    int numElements;
    char *pTransducerName;
    IusTransducerShape shape;

    IusTransducer * transducer;
    status |= iusReadShape( handle, "/Transducer/shape",  &(shape), verbose );
    status |= iusHdf5ReadString( handle, "/Transducer/transducerName",  &(pTransducerName), verbose );
    status |= iusHdf5ReadFloat( handle,  "/Transducer/centerFrequency", &(centerFrequency), verbose );
    status |= iusHdf5ReadInt( handle,    "/Transducer/numElements",     &(numElements),     verbose );

    if( status < 0 )
        return NULL;
    transducer = iusHLCreateTransducer(pTransducerName, shape, centerFrequency, numElements);
    return transducer;
}

int iusRead3DTransducerElementPositions(Ius3DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/positions", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);
    hid_t position_tid;
    Ius3DPosition * pPositionArray;
    int numElements = pTransducer->baseTransducer.numElements;

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead3DTransducerElementPositions: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pPositionArray = (Ius3DPosition *) calloc(numElements,sizeof(Ius3DPosition));
    position_tid = H5Tcreate (H5T_COMPOUND, sizeof(Ius3DPosition));
    H5Tinsert(position_tid, "x", HOFFSET(Ius3DPosition, x), H5T_NATIVE_FLOAT);
    H5Tinsert(position_tid, "y", HOFFSET(Ius3DPosition, y), H5T_NATIVE_FLOAT);
    H5Tinsert(position_tid, "z", HOFFSET(Ius3DPosition, z), H5T_NATIVE_FLOAT);

    // read positions
    status = H5Dread(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pPositionArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementPositions: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].position = pPositionArray[i];
    }

    free(pPositionArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    status |= H5Tclose(position_tid);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementPositions: Error closing position dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead3DTransducerElementSizes(Ius3DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/sizes", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);
    hid_t size_tid;
    Ius3DSize * pSizeArray;
    int numElements = pTransducer->baseTransducer.numElements;

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead3DTransducerElementSizes: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pSizeArray = (Ius3DSize *) calloc(numElements,sizeof(Ius3DSize));
    size_tid = H5Tcreate (H5T_COMPOUND, sizeof(Ius3DSize));
    H5Tinsert(size_tid, "sx", HOFFSET(Ius3DSize, sx), H5T_NATIVE_FLOAT);
    H5Tinsert(size_tid, "sy", HOFFSET(Ius3DSize, sy), H5T_NATIVE_FLOAT);
    H5Tinsert(size_tid, "sz", HOFFSET(Ius3DSize, sz), H5T_NATIVE_FLOAT);
    status = H5Dread(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pSizeArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementSizes: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].size = pSizeArray[i];
    }

    free(pSizeArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    status |= H5Tclose(size_tid);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementSizes: Error closing size dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead3DTransducerElementAngles(Ius3DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/angles", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);
    hid_t angle_tid;
    Ius3DAngle * pAngleArray;
    int numElements = pTransducer->baseTransducer.numElements;

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead3DTransducerElementAngles: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pAngleArray = (Ius3DAngle *) calloc(numElements,sizeof(Ius3DAngle));
    angle_tid = H5Tcreate (H5T_COMPOUND, sizeof(Ius3DAngle));
    H5Tinsert(angle_tid, "theta", HOFFSET(Ius3DAngle, theta), H5T_NATIVE_FLOAT);
    H5Tinsert(angle_tid, "phi", HOFFSET(Ius3DAngle, phi), H5T_NATIVE_FLOAT);
    status = H5Dread(dataset, angle_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pAngleArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementAngles: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].angle = pAngleArray[i];
    }

    free(pAngleArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    status |= H5Tclose(angle_tid);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementAngles: Error closing angle dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead3DTransducer(Ius3DTransducer * pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    status |= iusRead3DTransducerElementPositions(pTransducer, handle, verbose);
    status |= iusRead3DTransducerElementSizes(pTransducer, handle, verbose);
    status |= iusRead3DTransducerElementAngles(pTransducer, handle, verbose);
    return status;
}


int iusRead2DTransducerElementPositions(Ius2DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/positions", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);
    hid_t position_tid;
    Ius2DPosition * pPositionArray;
    int numElements = pTransducer->baseTransducer.numElements;

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead2DTransducerElementPositions: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pPositionArray = (Ius2DPosition *) calloc(numElements,sizeof(Ius2DPosition));
    position_tid = H5Tcreate (H5T_COMPOUND, sizeof(Ius2DPosition));
    H5Tinsert(position_tid, "x", HOFFSET(Ius2DPosition, x), H5T_NATIVE_FLOAT);
    H5Tinsert(position_tid, "z", HOFFSET(Ius2DPosition, z), H5T_NATIVE_FLOAT);

    // read positions
    status = H5Dread(dataset, position_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pPositionArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementPositions: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].position = pPositionArray[i];
    }

    free(pPositionArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    status |= H5Tclose(position_tid);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementPositions: Error closing position dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead2DTransducerElementSizes(Ius2DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/sizes", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);
    hid_t size_tid;
    Ius2DSize * pSizeArray;
    int numElements = pTransducer->baseTransducer.numElements;

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead2DTransducerElementSizes: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pSizeArray = (Ius2DSize *) calloc(numElements,sizeof(Ius2DSize));
    size_tid = H5Tcreate (H5T_COMPOUND, sizeof(Ius2DSize));
    H5Tinsert(size_tid, "sx", HOFFSET(Ius2DSize, sx), H5T_NATIVE_FLOAT);
    H5Tinsert(size_tid, "sz", HOFFSET(Ius2DSize, sz), H5T_NATIVE_FLOAT);
    status = H5Dread(dataset, size_tid, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pSizeArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementSizes: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].size = pSizeArray[i];
    }

    free(pSizeArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    status |= H5Tclose(size_tid);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementSizes: Error closing size dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead2DTransducerElementAngles(Ius2DTransducer *pTransducer, hid_t handle, int verbose)
{
    int status = 0;

    hid_t dataset;
    dataset = H5Dopen(handle, "/Transducer/Elements/theta", H5P_DEFAULT);
    hid_t space = H5Dget_space(dataset);

    float * pAngleArray;
    int numElements = pTransducer->baseTransducer.numElements;
    hsize_t       dims[1];              //NOTE: pElements assumed 1D array, extra length to prevent

    int ndims = H5Sget_simple_extent_dims(space, dims, NULL);
    if ( ndims != 1 )
    {
        fprintf( stderr, "iusRead3DTransducerElementAngles: Only 1D array of transducer elements supported \n" );
        return -1;
    }

    pAngleArray = (float *) calloc(numElements,sizeof(float));
    status = H5Dread(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,  pAngleArray);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementAngles: Error reading data: H5Dread returned: %d\n", status);
        return status;
    }

    // copy positions
    for (int i = 0; i < dims[0]; i++ )
    {
        pTransducer->pElements[i].theta = pAngleArray[i];
    }

    free(pAngleArray);
    status |= H5Dclose(dataset);
    status |= H5Sclose(space);
    if (status < 0)
    {
        fprintf(stderr, "iusRead3DTransducerElementAngles: Error closing angle dataset: %d\n", status);
        return status;
    }

    return status;
}

int iusRead2DTransducer(Ius2DTransducer * pTransducer, hid_t handle, int verbose)
{
    int status = 0;
    status |= iusRead2DTransducerElementPositions(pTransducer, handle, verbose);
    status |= iusRead2DTransducerElementSizes(pTransducer, handle, verbose);
    status |= iusRead2DTransducerElementAngles(pTransducer, handle, verbose);
    return status;
}

IusTransducer *iusReadTransducer(hid_t handle, int verbose) {
    int status = 0;
    IusTransducer *pTransducer = iusReadBaseTransducer(handle,verbose);
    if( pTransducer->type == IUS_3D_SHAPE )
    {
        Ius3DTransducer *p3DTransducer = (Ius3DTransducer *) pTransducer;
        p3DTransducer->pElements = (Ius3DTransducerElement *) calloc( pTransducer->numElements , sizeof(Ius3DTransducer));
        if( p3DTransducer->pElements != NULL )
            status = iusRead3DTransducer(p3DTransducer, handle, verbose);
        else
            status = IUS_ERR_VALUE;
    }

    if( pTransducer->type == IUS_2D_SHAPE )
    {
        Ius2DTransducer *p2DTransducer = (Ius2DTransducer *) pTransducer;
        p2DTransducer->pElements = (Ius2DTransducerElement *) calloc( pTransducer->numElements , sizeof(Ius2DTransducer));
        if( p2DTransducer->pElements != NULL )
            status = iusRead2DTransducer(p2DTransducer, handle, verbose);
        else
            status = IUS_ERR_VALUE;
    }
    if( status != 0 )
        return NULL;
    return pTransducer;
}


int iusReadDrivingSchemeType(hid_t handle, char *pVariableString, IusDrivingSchemeType *pType, int verbose)
{
    herr_t status = 0;
    hid_t hdf_drivingSchemeType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_drivingSchemeType,  DRIVINGSCHEME_DIVERGING_WAVES_PARAMETRIZED,
                                                      (enumValue=IUS_DIVERGING_WAVES_PARAMETRIZED, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_DIVERGING_WAVES,
                                                      (enumValue=IUS_DIVERGING_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_FOCUSED_WAVES_PARAMETRIZED,
                                                      (enumValue=IUS_FOCUSED_WAVES_PARAMETRIZED, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_FOCUSED_WAVES,
                                                      (enumValue=IUS_FOCUSED_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_PLANE_WAVES,
                                                      (enumValue=IUS_PLANE_WAVES, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_SINGLE_ELEMENT,
                                                      (enumValue=IUS_SINGLE_ELEMENT, &enumValue) );
    status |= H5Tenum_insert( hdf_drivingSchemeType, DRIVINGSCHEME_CUSTOM_WAVES,
                                                      (enumValue=IUS_CUSTOM_WAVES, &enumValue) );
    *pType = IUS_INVALID_DRIVING_SCHEME;
    status |= H5LTread_dataset( handle, pVariableString , hdf_drivingSchemeType, pType );
    return status;
}

IusDrivingScheme *iusReadBaseDrivingScheme(hid_t handle, int verbose)
{
    IusDrivingScheme * parametrizedDrivingScheme;
    int numElements = 32;
    int numTransmitPulses = 13;
    int numTransmitSources = 13;
    int status = 0;

    IusShape shape = IUS_2D_SHAPE;

//    status |= iusHdf5ReadString( handle, "/Transducer/transducerName",  &(pTransducerName), verbose );
//    status |= iusHdf5ReadFloat( handle,  "/Transducer/centerFrequency", &(centerFrequency), verbose );
//    status |= iusHdf5ReadInt( handle,    "/Transducer/numElements",     &(numElements),     verbose );

    IusDrivingSchemeType type = IUS_DIVERGING_WAVES_PARAMETRIZED;
    parametrizedDrivingScheme = iusHLCreateDrivingScheme(type,
                                                         shape,
                                                         numTransmitPulses,
                                                         numTransmitSources,
                                                         numElements );
    // read type

    // read shape
    // read num transmit sources
    // read num transmit pulses
    // read numelements
    // transmit pattern delay
    // transmit pattern
    // transmit pulse
    // transmit pulse apodization
    return parametrizedDrivingScheme;
}

int iusRead3DDrivingScheme(Ius3DDrivingScheme *scheme, hid_t handle, int verbose)
{
    return IUS_E_OK;
}

int iusRead2DDrivingScheme(Ius2DDrivingScheme *scheme, hid_t handle, int verbose)
{
    return IUS_E_OK; //
}

int iusReadTransmitApodization
(
    hid_t handle,
    IusDrivingScheme *pDrivingScheme,
    int verbose
)
{
    int status = 0;
    return status;
}

int iusReadTransmitPattern
(
    hid_t handle,
    IusDrivingScheme *pDrivingScheme,
    int verbose
)
{

    int status = 0;
    status |= iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPattern/transmitPatternDelay", &(pDrivingScheme->transmitPatternDelay), verbose );

    int i;
    int *   pIndexData;
    float * pTimeData;

    int numPulses = pDrivingScheme->numTransmitPulses;

    IUS_UNUSED(verbose); // avoid compiler warnings

    pIndexData   =   (int *) malloc(numPulses * sizeof(int));
    pTimeData   = (float *) malloc(numPulses * sizeof(float));
    pDrivingScheme->pTransmitPatterns = iusHLCreateTransmitPatternList(numPulses);
    if ( pDrivingScheme->pTransmitPatterns == NULL || pIndexData == NULL || pTimeData == NULL)
    {
        fprintf( stderr, "iusReadTransmitPattern: Error allocating data\n" );
        free( pIndexData );
        free( pTimeData );
        return -1;
    }

    status |= H5LTread_dataset_int( handle,   "/DrivingScheme/TransmitPattern/transmitPatternIndex",  pIndexData );
    status |= H5LTread_dataset_float( handle, "/DrivingScheme/TransmitPattern/transmitPatternTime",   pTimeData );
    if ( status < 0 )
    {
        fprintf( stderr, "iusReadTransmitPattern: Error reading dataset: H5Dread returned: %d\n", status );
        free( pIndexData );
        free( pTimeData );
        return status;
    }

    for ( i = 0; i < numPulses; i++ )
    {
        pDrivingScheme->pTransmitPatterns->pTransmitPattern[i].index = pIndexData[i];
        pDrivingScheme->pTransmitPatterns->pTransmitPattern[i].time  = pTimeData[i];
    }

    free( pIndexData );
    free( pTimeData );
    return status;
}

IusDrivingScheme *iusReadDrivingScheme(hid_t handle, IusShape shape,  int verbose) {
    int status = 0;
    int numTransmitSources;
    int numTransmitPulses;
    int numElements;
    IusDrivingSchemeType type;
    IusDrivingScheme *pDrivingScheme;

    // Read info needed for constructor
    status |= iusReadDrivingSchemeType( handle, "/DrivingScheme/drivingSchemeType",  &(type), verbose );
    status |= iusHdf5ReadInt( handle,    "/DrivingScheme/numTransmitSources",     &(numTransmitSources),     verbose );
    status |= iusHdf5ReadInt( handle,    "/DrivingScheme/numTransmitPulses",     &(numTransmitPulses),     verbose );
    status |= iusHdf5ReadInt( handle,    "/DrivingScheme/numElements",     &(numElements),     verbose );

    if( shape == IUS_3D_SHAPE )
    {
        Ius3DDrivingScheme *_3DDrivingScheme = (Ius3DDrivingScheme *) iusCreate3DDrivingScheme(numTransmitSources);
        if( _3DDrivingScheme == NULL ) return NULL;
        status = iusRead3DDrivingScheme(_3DDrivingScheme, handle, verbose);
        pDrivingScheme = (IusDrivingScheme *) _3DDrivingScheme;
    }

    if( shape == IUS_2D_SHAPE )
    {
        Ius2DDrivingScheme *_2DDrivingScheme = (Ius2DDrivingScheme *) iusCreate2DDrivingScheme(numTransmitSources);
        if( _2DDrivingScheme == NULL ) return NULL;
        status = iusRead2DDrivingScheme(_2DDrivingScheme, handle, verbose);
        pDrivingScheme = (IusDrivingScheme *) _2DDrivingScheme;
    }

    if( status != 0 )
        return NULL;

    // Read base drivingscheme
    pDrivingScheme->pTransmitApodization = iusCreateTransmitApodization(numTransmitPulses , numElements);
    if ( pDrivingScheme->pTransmitApodization == NULL )
    {
        iusHLDeleteDrivingScheme(pDrivingScheme);
        return NULL;
    }

    // TransmitPattern
    pDrivingScheme->numElements = numElements;
    pDrivingScheme->numTransmitSources = numTransmitSources;
    pDrivingScheme->numTransmitPulses = numTransmitPulses;
    pDrivingScheme->shape = shape;
    pDrivingScheme->type = type;

    status |= H5LTread_dataset_float(handle, "/DrivingScheme/transmitApodization", pDrivingScheme->pTransmitApodization);
    status |= iusReadTransmitPattern(handle,pDrivingScheme,verbose);
    if( status != 0 )
        return NULL;

    return pDrivingScheme;
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
    IusExperiment *pExperiment;
    IusTransducer *pTransducer;
    IusDrivingScheme *pDrivingScheme;
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
#ifdef old
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
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numSamplesPerLine",   &(pInst->pDrivingScheme->numSamplesPerLine), verbose );
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numTransmitPulses",   &(pInst->pDrivingScheme->numTransmitPulses), verbose );
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/numTransmitSources",  &(pInst->pDrivingScheme->numTransmitSources), verbose );
    status = iusHdf5ReadInt( handle,   "/numFrames",                         &(pInst->numFrames), verbose );

    if ( pInst->pDrivingScheme->drivingSchemeType == IUS_DIVERGING_WAVES )
    {
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceDeltaTheta", &(pInst->pDrivingScheme->sourceDeltaTheta), verbose );
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceFNumber",      &(pInst->pDrivingScheme->sourceFNumber), verbose );
        status = iusHdf5ReadFloat(handle, "/DrivingScheme/sourceStartTheta",   &(pInst->pDrivingScheme->sourceStartTheta), verbose );
    }
    else
    {
        status = LF_readSourceLocationsCartesian(pInst, handle, verbose);
    }
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPattern/transmitPatternDelay", &(pInst->pDrivingScheme->transmitPatternDelay), verbose );
    status = LF_readTransmitPattern( pInst, handle, verbose );
    status = iusHdf5ReadInt( handle, "/DrivingScheme/TransmitPulse/numPulseValues", &(pInst->pDrivingScheme->pTransmitPulse.numPulseValues), verbose );

    if (pInst->pDrivingScheme->pTransmitPulse.numPulseValues != 0)
    {
        // read rawTransmitPulse
        status = LF_readPulseRaw(pInst, handle, pInst->pDrivingScheme->pTransmitPulse.numPulseValues, verbose);
    }
    else
    {
        //alternative waveform description
        // TODO: define these
    }
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPulse/pulseFrequency", &(pInst->pDrivingScheme->pTransmitPulse.pulseFrequency), verbose );
    status = iusHdf5ReadFloat( handle, "/DrivingScheme/TransmitPulse/pulseAmplitude", &(pInst->pDrivingScheme->pTransmitPulse.pulseAmplitude), verbose );
    status = iusHdf5ReadInt( handle,   "/DrivingScheme/TransmitPulse/pulseCount", &(pInst->pDrivingScheme->pTransmitPulse.pulseCount), verbose );

    status = iusHdf5ReadInt(handle, "/ReceiveSettings/numChannels", &(pInst->pReceiveSettings->receiveChannelCoding.numChannels), verbose);
    pInst->pReceiveSettings->receiveChannelCoding.pChannelMap = (int*)calloc(pInst->pReceiveSettings->receiveChannelCoding.numChannels, sizeof(int));
    status = iusHdf5ReadInt(handle, "/ReceiveSettings/channelMap", pInst->pReceiveSettings->receiveChannelCoding.pChannelMap, verbose);

    status = iusHdf5ReadFloat( handle, "/ReceiveSettings/sampleFrequency", &(pInst->pReceiveSettings->sampleFrequency),verbose );
    status = iusHdf5ReadInt( handle,   "/ReceiveSettings/TimeGainControl/numValues", &(pInst->pReceiveSettings->numTimeGainControlValues), verbose );
    status = LF_readTimeGainControls( pInst, handle, verbose );
    pInst->pReceiveSettings->pStartDelay = (float *)calloc(pInst->pDrivingScheme->numTransmitPulses, sizeof(float));
    pInst->pReceiveSettings->pEndDepth   = (float *)calloc(pInst->pDrivingScheme->numTransmitPulses, sizeof(float));
    status = H5LTread_dataset_float(handle, "/ReceiveSettings/startDepth", pInst->pReceiveSettings->pStartDelay);
    status = H5LTread_dataset_float(handle, "/ReceiveSettings/endDepth", pInst->pReceiveSettings->pEndDepth);

    status = iusHdf5ReadInt(handle,  "/DrivingScheme/numChannels", &(pInst->pDrivingScheme->transmitChannelCoding.numChannels), verbose);
    pInst->pDrivingScheme->transmitChannelCoding.pChannelMap = (int*)calloc(pInst->pDrivingScheme->transmitChannelCoding.numChannels, sizeof(int));
    status = iusHdf5ReadInt(handle, "/DrivingScheme/channelMap", pInst->pDrivingScheme->transmitChannelCoding.pChannelMap, verbose);

    numElements = LF_GetNumberOfElements(pInst);
    pInst->pDrivingScheme->pTransmitApodization = (float*)calloc(pInst->pDrivingScheme->numTransmitPulses * numElements, sizeof(float));
    status = H5LTread_dataset_float(handle, "/DrivingScheme/transmitApodization", pInst->pDrivingScheme->pTransmitApodization);

#endif
    // Read NodeData
    status = 0;
    status |= iusReadNode(&pInst->iusNode, handle, verbose);

    // Read input type fields
    status |= iusHdf5ReadInt( handle, "/IusVersion", &(pInst->IusVersion), verbose );
    status |= iusHdf5ReadInt( handle, "/numFrames", &(pInst->numFrames), verbose );

    // Read Experiment fields
    pExperiment = iusReadExperiment(handle, verbose);
    status |= iusHLHeaderSetExperiment(pInst,pExperiment);


    // Read Transducer fields
    pTransducer = iusReadTransducer(handle, verbose);
    status |= iusHLHeaderSetTransducer(pInst,pTransducer);


    IusShape shape = iusHLTransducerGetShapeType(pTransducer);

    // Read DrivingScheme fields
    pDrivingScheme = iusReadDrivingScheme(handle, shape, verbose);
    status |= iusHLHeaderSetDrivingScheme(pInst, pDrivingScheme);

    if( status != 0 )
        return NULL;
    else
        return pInst;
}


