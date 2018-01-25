//==============================================================================
//     COPYRIGHT 2018 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWC.c
//                  instance describing ultrasound Coherent Wave Compounded data
//  iusVersion    : 255.255.255.255
//  author        : Frank van Heesch, Ben Bierens, Harm Belt
//
//==============================================================================

#include "iusCWC.h"
#include "iusBasicFunctions.h"
#include "iusVector.h"
#include "iusError.h"
#include "iusHDF5.h"
#include <stdlib.h>
#include <string.h>

/*
long long iusCWCStaticMemoryUsage
(
    int numTransducerElements,
    int numTransmitPulses,
    int numLinesPerPulse,
    int numSamplesPerLine,
    int numThetaGridPoints,
    int numRadiusGridPoints,
    int upFactor
)
{
    long long numBytes = 0;

    IUS_ASSERT( numTransducerElements > 0 );
    IUS_ASSERT( numTransmitPulses > 0 );
    IUS_ASSERT( numLinesPerPulse > 0 );
    IUS_ASSERT( numSamplesPerLine > 0 );
    IUS_ASSERT( numThetaGridPoints > 0 );
    IUS_ASSERT( numRadiusGridPoints > 0 );
    IUS_ASSERT( upFactor > 0.0f );

    // instance structure
    numBytes += (long long)sizeof( IUSCwcInstance );

    // pRadiusGridPoints
    numBytes += (long long)numRadiusGridPoints * sizeof(float);

    // pppUp
    numBytes += (long long)numTransmitPulses * numTransducerElements *
        upFactor * numSamplesPerLine * sizeof(float);

    // ppElementPositions
    numBytes += (long long)3 * numTransducerElements * sizeof(float);

    // pDelayOffsetSec
    numBytes += (long long)numTransmitPulses * sizeof(float);

    // pDelayIndices
    numBytes += ((long long)numThetaGridPoints * numRadiusGridPoints *
            numTransmitPulses * numTransducerElements + 1) *
        sizeof( int );

    // pKStart + pKEnd
    numBytes += (long long)2 * numRadiusGridPoints * numThetaGridPoints *
        sizeof(int);

    return numBytes;
}
*/

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
IusCwcInstance * iusCwcCreate
(
    int                   numFrames,
    int                   numComponents,
    const IusGrid * const pSampleGrid,
    const float * const   pTimeStamps,
    float                 centerFrequency,
    float                 speedOfSound
 // float                 focusRadius,
 // float * const         pFocusAngle

 //float * const          pApodR,
 //int                    numThetaGridPoints,
 //int                    numRadiusGridPoints,
 //int                    upFactor,
 //int                    applyWeightedCompounding
)
{
    //--------------------------------------------------------------------------
    // Create instance of object describing ultrasound Cwc data
    //--------------------------------------------------------------------------
    IusCwcInstance * pInst =
        (IusCwcInstance *)malloc( sizeof( IusCwcInstance ) );
    IUS_ASSERT_MEMORY( pInst != NULL );

    //--------------------------------------------------------------------------
    // Assertions on input arguments
    //--------------------------------------------------------------------------
    IUS_ASSERT_VALUE( numFrames > 0 );
    IUS_ASSERT_VALUE( numComponents > 0 );
    IUS_ASSERT_VALUE( centerFrequency > 0.0f );
    IUS_ASSERT_VALUE( speedOfSound > 0.0f );

    //--------------------------------------------------------------------------
    // Copy input arguments into cwc instance
    //--------------------------------------------------------------------------
    pInst->numFrames       = numFrames;
    pInst->numComponents   = numComponents;
    pInst->centerFrequency = centerFrequency;
    pInst->speedOfSound    = speedOfSound;

    pInst->pSampleGrid =
        iusCreateGrid( 0, pSampleGrid->numPoints1, pSampleGrid->numPoints2 );
    iusCopyGrid( pInst->pSampleGrid, pSampleGrid );

    pInst->pTimeStamps = iusAllocFloatVector( numFrames * numComponents );
    iusCopyFloatVector( pInst->pTimeStamps, pTimeStamps,
        numComponents * numFrames );

    //--------------------------------------------------------------------------
    // Node administration
    //--------------------------------------------------------------------------
    setIusUuidCreate( pInst->iusNode.pId );
    strcpy( pInst->iusNode.pType, IUS_CWC_TYPE );

    return pInst;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
IusCwcInstance * iusCwcRead
(
    hid_t handle,
    int   verbose
)
{
    int i, num, dims[3];
    int numFrames, numComponents;
    float dx, x1;
    IusCwcInstance * pInst;
    herr_t status;

    //--------------------------------------------------------------------------
    // Allocate Cwc object describing ultrasound Coherent Wave Compounded data
    //--------------------------------------------------------------------------
    pInst = (IusCwcInstance *)malloc( sizeof( IusCwcInstance ) );
    IUS_ASSERT_MEMORY( pInst != NULL );

    //--------------------------------------------------------------------------
    // Copy handle values into CWC instance
    //--------------------------------------------------------------------------

    // First some scalars
    status  =   iusHdf5ReadInt( handle, "numFrames"      ,
                                &(pInst->numFrames)      , verbose );
    status |=   iusHdf5ReadInt( handle, "numComponents"  ,
                                &(pInst->numComponents)  , verbose );
    status |= iusHdf5ReadFloat( handle, "centerFrequency",
                                &(pInst->centerFrequency), verbose );
    status |= iusHdf5ReadFloat( handle, "speedOfSound"   ,
                                &(pInst->speedOfSound)   , verbose );
    if ( status < 0 )
    {
        fprintf( stderr,
	    "iusCwcRead: Error reading some scalar values: error %d\n", status);
        return NULL;
    }

    // Spatial data grid
    dims[0]  = 0;  // elevation
    status   = iusHdf5ReadInt( handle, "PolarGrid/numPointsTheta",
                               &dims[1], verbose );  // lateral
    status  |= iusHdf5ReadInt( handle, "PolarGrid/numPointsRadial",
                               &dims[2], verbose );  // axial
    if ( status < 0 || dims[1] < 1 || dims[2] < 1 )
    {
        fprintf( stderr,
	    "iusCwcRead: Error reading grid dimensions: error %d\n", status );
        return NULL;
    }
    pInst->pSampleGrid = iusCreateGrid( dims[0], dims[1], dims[2] );
    status = iusHdf5ReadGrid( handle, "PolarGrid", pInst->pSampleGrid, verbose );
    if ( status < 0 )
    {
        fprintf( stderr,
	    "iusCwcRead: Error reading grid data: error %d\n", status );
        return NULL;
    }
  
    // Temporal data grid
    status  = iusHdf5ReadInt( handle, "numFrames"    , &numFrames    , verbose );
    status |= iusHdf5ReadInt( handle, "numComponents", &numComponents, verbose );
    if ( status < 0 || numFrames < 1 || numComponents < 1 )
    {
        fprintf( stderr,
	    "iusCwcRead: Error reading numFrames and numComponents: error %d\n",
	    status );
        return NULL;
    }
    pInst->pTimeStamps = iusAllocFloatVector( numFrames * numComponents );
    status =
        iusHdf5ReadFloat( handle, "timeStamps", pInst->pTimeStamps, verbose );
    if ( status < 0 )
    {
        fprintf( stderr,
	    "iusCwcRead: Error reading time stamps: error %d\n", status );
        return NULL;
    }

    //--------------------------------------------------------------------------
    // Node administration
    //--------------------------------------------------------------------------
    strcpy( pInst->iusNode.pType, IUS_CWC_TYPE );
    iusNodeLoadParents( (IusNode *)pInst, handle );

    return pInst;
}


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
int iusCwcWrite
(
    hid_t                        handle,
    const IusCwcInstance * const pInst,
    int                          verbose
)
{
    hsize_t dims[1] = {1};

    //--------------------------------------------------------------------------
    // TODO: comment
    //--------------------------------------------------------------------------
    H5LTmake_dataset_string( handle, "ID", pInst->iusNode.pId);
    H5LTmake_dataset_string( handle, "type", pInst->iusNode.pType);
    H5LTmake_dataset_int( handle,   "numFrames", 1, dims,
                          &(pInst->numFrames) );
    H5LTmake_dataset_int( handle,   "numComponents", 1, dims,
                          &(pInst->numComponents) );
    H5LTmake_dataset_float( handle, "centerFrequency", 1, dims,
                            &(pInst->centerFrequency));
    H5LTmake_dataset_float( handle, "speedOfSound", 1, dims,
                            &(pInst->speedOfSound));

    dims[0] = pInst->numFrames * pInst->numComponents;
    H5LTmake_dataset_float( handle,  "timeStamps",  1, dims,
                            &(pInst->pTimeStamps) );

    iusHdf5WriteGrid( handle, "PolarGrid", pInst->pSampleGrid, verbose );
    //H5LTmake_dataset_int( handle,    "/numTransducerElements",  1, dims,
    //                      &(pInst->numTransducerElements));
    //H5LTmake_dataset_float( handle,  "/elementWidth",  1, dims,
    //                        &(pInst->elementWidth));
    //H5LTmake_dataset_float( handle,  "/kerf",  1, dims, &(pInst->kerf));
    //H5LTmake_dataset_float( handle,  "/focusRadius",  1, dims,
    //                        &(pInst->focusRadius));
    //dims[0]=pInst->numTransmitPulses;
    //H5LTmake_dataset_float( handle,  "/focusAngle",  1, dims,
    //                        &(pInst->pFocusAngle));
    //dims[0]=1;
    //H5LTmake_dataset_int( handle,    "/numLinesPerPulse",  1, dims,
    //                      &(pInst->numLinesPerPulse));
    //H5LTmake_dataset_int( handle,    "/numSamplesPerLine",  1, dims,
    //                      &(pInst->numSamplesPerLine));
    //H5LTmake_dataset_float( handle,  "/sampleFrequency",  1, dims,
    //                        &(pInst->sampleFrequency));
    //dims[0] = pInst->numTransducerElements;
    //H5LTmake_dataset_float( handle,  "/receiveApodization",  1, dims,
    //                        &(pInst->pApodR));

    //"startFrame"               // is an algorithm parameter
    //"numThetaGridPoints"       // is an algorithm parameter
    //"numRadiusGridPoints"      // is an algorithm parameter
    //"applyWeightedCompounding" // is an algorithm parameter
    //"upFactor"                 // is an algorithm parameter

    //--------------------------------------------------------------------------
    // Node administration
    //--------------------------------------------------------------------------
    iusNodeSaveParents( (IusNode *)pInst, handle );

    //--------------------------------------------------------------------------
    // TODO: any error handling left to do?
    //--------------------------------------------------------------------------
  
    return 0;
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
void iusCwcDestroy
(
    IusCwcInstance * pInst
)
{
    IUS_ASSERT_MEMORY( pInst );

    iusFreeGrid( pInst->pSampleGrid );
    iusFreeFloatVector( pInst->pTimeStamps );
    free( pInst );
}


#ifdef __cplusplus
}
#endif






