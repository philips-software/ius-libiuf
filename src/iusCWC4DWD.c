//==============================================================================
//     COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusCWC4DWD.c
//                  Coherent Wave Compounding for Diverging Wave Driving
//  iusVersion    : 255.255.255.255
//  author        : Harm Belt and Kees Janse
//
//==============================================================================

#include "iusCWC.h"
#include "iusCWC4DWD.h"
#include "iusBasicFunctions.h"
#include "iusVector.h"
#include "iusMatrix.h"
#include "iusLUT.h"
#include "iusError.h"         // IUS_ASSERT
#include <stdio.h>
#include <stdlib.h>           // malloc, free
#include <math.h>             // sinf, cosf

//=============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//=============================================================================

//=============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//=============================================================================
/**
 * \brief Compute the minimum of all propagation delays between one virtual
 * source (focus point) behind the array and all the elements. Rationale: this
 * minimum of all propagation delays varies between transmit focus points and
 * was thrown out on the Verasonics platform by the Verasonics function that
 * computes the transmit delays.
 */
static float LF_ComputeFocus2ElementsMinPropDelay
(
    float ** ppElemPos,              // x,y,z coordinates of elements in [m]
    int      numTransducerElements,  // numer of transducer elements in array
    float    rF,                     // radius [m] of focus point behind array
    float    thF,                    // in radians
    float    speedOfSound            // speed of sound in the medium [m/s]
);

//=============================================================================
// E X P O R T E D   F U N C T I O N S
//=============================================================================

long long iusCwc4DwdStaticMemoryUsage
(
    int numTransducerElements,
    int numTransmitWaves,
    int numLinesPerWave,
    int numSamplesPerLine,
    int numThetaGridPoints,
    int numRadiusGridPoints,
    int upFactor
)
{
    long long numBytes = 0;

    IUS_ASSERT( numTransducerElements > 0 );
    IUS_ASSERT( numTransmitWaves > 0 );
    IUS_ASSERT( numLinesPerWave > 0 );
    IUS_ASSERT( numSamplesPerLine > 0 );
    IUS_ASSERT( numThetaGridPoints > 0 );
    IUS_ASSERT( numRadiusGridPoints > 0 );
    IUS_ASSERT( upFactor > 0.0f );
    
    // instance structure
    numBytes += (long long)sizeof( IusCwc4DwdInstance );

    // pRadiusGridPoints
    numBytes += (long long)numRadiusGridPoints * sizeof( float );
    
    // pppUp
    numBytes += (long long)numTransmitWaves * numTransducerElements *
	    upFactor * numSamplesPerLine * sizeof( float );
    
    // ppElementPositions
    numBytes += (long long)3 * numTransducerElements * sizeof( float );
    
    // pDelayOffsetSec
    numBytes += (long long)numTransmitWaves * sizeof( float );
    
    // pDelayIndices
    numBytes += ((long long)numThetaGridPoints * numRadiusGridPoints *
            numTransmitWaves * numTransducerElements + 1) *
            sizeof( int );
    
    // pKStart + pKEnd
    numBytes += (long long)2 * numRadiusGridPoints * numThetaGridPoints *
        sizeof( int );
    
    return numBytes;
}

//------------------------------------------------------------------------------
// Creates an Algo instance with the specified parameters
//------------------------------------------------------------------------------
IusCwc4DwdInstance * iusCwc4DwdCreate
(
    // parameters defining the transducer
    float               centerFrequency,
    int                 numTransducerElements,
    float               elementWidth,
    float               kerf,
    // parameters defining the input data from diverging waves driving
    int                 numTransmitWaves,
    float               focusRadius,
    float * const       pFocusAngle,
    int                 numLinesPerWave,
    int                 numSamplesPerLine,
    float               sampleFrequency,
    float               speedOfSound,
    // parameters for computing the output
    float * const       pApodR,
    IusGrid *           pSampleGrid,
    //int                 numThetaGridPoints,
    //int                 numRadiusGridPoints,
    int                 upFactor,
    int                 applyWeightedCompounding
)
{
    int i, num;
    float dx, x1;
    IusCwc4DwdInstance * pInst =
        (IusCwc4DwdInstance *)malloc( sizeof( IusCwc4DwdInstance ) );

    IUS_ASSERT_MEMORY( pInst != NULL );

    // Assertion on input arguments
    IUS_ASSERT_VALUE( centerFrequency > 0.0f );
    IUS_ASSERT_VALUE( numTransducerElements > 0 );
    IUS_ASSERT_VALUE( elementWidth > 0.0f );
    //IUS_ASSERT_VALUE( kerf >= 0.0f );
    IUS_ASSERT_VALUE( numTransmitWaves > 0 );
    IUS_ASSERT_VALUE( focusRadius > 0.0f );
    IUS_ASSERT( pFocusAngle != NULL );
    IUS_ASSERT_VALUE( numLinesPerWave > 0 );
    IUS_ASSERT_VALUE( numSamplesPerLine > 0 );
    IUS_ASSERT_VALUE( sampleFrequency > 0.0f );
    IUS_ASSERT_VALUE( speedOfSound > 0.0f );
    IUS_ASSERT( pApodR != NULL );
    IUS_ASSERT( pSampleGrid != NULL );
    IUS_ASSERT_VALUE( pSampleGrid->numPoints1 > 0 );
    IUS_ASSERT_VALUE( pSampleGrid->numPoints2 > 0 );
    IUS_ASSERT_VALUE( upFactor > 0 );
    IUS_ASSERT_VALUE( numTransducerElements == numLinesPerWave );

    //--------------------------------------------------------------------------
    // Copy input arguments into algorithm instance
    //--------------------------------------------------------------------------
    pInst->centerFrequency          = centerFrequency;
    pInst->numTransducerElements    = numTransducerElements;
    pInst->elementWidth             = elementWidth;
    pInst->kerf                     = kerf;
    pInst->numTransmitWaves         = numTransmitWaves;
    pInst->focusRadius              = focusRadius;
    pInst->pFocusAngle              = pFocusAngle; // TODO: copy all, not ptr
    pInst->numLinesPerWave          = numLinesPerWave;
    pInst->numSamplesPerLine        = numSamplesPerLine;
    pInst->sampleFrequency          = sampleFrequency;
    pInst->speedOfSound             = speedOfSound;
    pInst->pApodR                   = pApodR; // TODO: copy all, not ptr
    pInst->pSampleGrid              = pSampleGrid; // TODO: copy all, not ptr
    pInst->upFactor                 = upFactor;
    pInst->applyWeightedCompounding = applyWeightedCompounding;
	
    //--------------------------------------------------------------------------
    // Allocate memory for intermediate data
    //--------------------------------------------------------------------------
    pInst->pppUp = iusAllocFloatMatrix3D( numTransmitWaves,
        numTransducerElements, pInst->upFactor * numSamplesPerLine );
    pInst->ppElementPositions = iusAllocFloatMatrix( 3, numTransducerElements );
    pInst->pDelayOffsetSec    = iusAllocFloatVector( numTransmitWaves );
    num = pInst->pSampleGrid->numPoints2 * pInst->pSampleGrid->numPoints1 *
        numTransmitWaves * numTransducerElements + 1;
    pInst->pDelayIndices = iusAllocIntVector( num );
    pInst->pKStart = iusAllocIntVector( pInst->pSampleGrid->numPoints1 * 
                                        pInst->pSampleGrid->numPoints2 );
    pInst->pKEnd   = iusAllocIntVector( pInst->pSampleGrid->numPoints1 * 
                                        pInst->pSampleGrid->numPoints2 );

    //--------------------------------------------------------------------------
    // Initialize array with element positions
    //--------------------------------------------------------------------------
    dx = elementWidth + kerf;
    // x-coordinate first element. TODO: verify for even and odd
    //                                   numTransducerElements
    x1 = -dx * ((float)(numTransducerElements - 1) / 2.0f);
    for ( i = 0; i < numTransducerElements; i++ )
    {
        // Initialize array with element positions
        pInst->ppElementPositions[0][i] = x1 + i * dx;  // x-coordinates [m]
        pInst->ppElementPositions[1][i] = 0.0f;         // y-coordinates [m]
        pInst->ppElementPositions[2][i] = 0.0f;         // z-coordinates [m]
    }

    for ( i = 0; i < numTransmitWaves; i++ )
    {
        // Initialize array with element positions
        // This code is thoroughly verified against Verasonics delay offsets
        pInst->pDelayOffsetSec[i] = 
            LF_ComputeFocus2ElementsMinPropDelay( pInst->ppElementPositions,
                numTransducerElements, focusRadius, pFocusAngle[i],
                speedOfSound );
    }

    pInst->outputGridWasSet = 0;

    return pInst;
}

//------------------------------------------------------------------------------
// For a new output grid (of the same size as defined at Create) compute the
// look-up table with indices for compounding
//------------------------------------------------------------------------------
void iusCwc4DwdSetNewOutputGrid
(
    IusCwc4DwdInstance * const        pInst,
    const IusGrid * const             pSampleGrid,
    const iusFloatIntervalLUT * const pNumOverlapBeamsLUT
)
{
    //int * pDelayIdx = pInst->pDelayIndices;
    int     idx = 0;
    int     i, j, k, n;
    int     kIdx;
    int     numTransducerElements;
    int     numSamplesPerLine;
    int     numTransmitWaves;
    float   focusRadius;
    float * pFocusAngle;
    float   speedOfSound;
    float   sampleFrequency;

    //--------------------------------------------------------------------------
    // Early return when input invalid
    //--------------------------------------------------------------------------
    IUS_ASSERT_MEMORY( pInst );
    IUS_ASSERT_MEMORY( pSampleGrid );
    IUS_ASSERT_MEMORY( pNumOverlapBeamsLUT );
    IUS_ASSERT_MEMORY( pSampleGrid->pPoints1 );
    IUS_ASSERT_MEMORY( pSampleGrid->pPoints2 );
    IUS_ASSERT_VALUE(pSampleGrid->numPoints1 == pInst->pSampleGrid->numPoints1);
    IUS_ASSERT_VALUE(pSampleGrid->numPoints2 == pInst->pSampleGrid->numPoints2);

    //--------------------------------------------------------------------------
    // Assertions on output grid values
    //--------------------------------------------------------------------------
    for ( i = 0; i < pSampleGrid->numPoints1; i++ )
    {
        float val = pSampleGrid->pPoints1[i];
        IUS_ASSERT_VALUE( val >= -IUS_PI / 2.0f );
        IUS_ASSERT_VALUE( val <= IUS_PI / 2.0f );
    }
    for ( i = 0; i < pSampleGrid->numPoints1; i++ )
    {
        // must be monotonically increasing to facilitate efficient
        // interpolation between lines
        IUS_ASSERT_VALUE( pSampleGrid->pPoints1[i] >
	    pSampleGrid->pPoints1[i-1] );
        IUS_ASSERT_VALUE( pSampleGrid->pPoints1[i] > -IUS_PI / 2.0f );
        IUS_ASSERT_VALUE( pSampleGrid->pPoints1[i] <  IUS_PI / 2.0f );
    }
    for ( i = 0; i < pSampleGrid->numPoints2; i++ )
    {
        // ensure all grid points are in front of the array
        // (thus having a non-negative axial position value)
        IUS_ASSERT_VALUE( pSampleGrid->pPoints2[i] >= 0.0f );
    }

    //--------------------------------------------------------------------------
    // 
    //--------------------------------------------------------------------------
    numTransducerElements     = pInst->numTransducerElements;
    numSamplesPerLine         = pInst->numSamplesPerLine;
    numTransmitWaves          = pInst->numTransmitWaves;
    focusRadius               = pInst->focusRadius;
    pFocusAngle               = pInst->pFocusAngle;
    speedOfSound              = pInst->speedOfSound;
    sampleFrequency           = pInst->sampleFrequency;

    //--------------------------------------------------------------------------
    // Compute new indices
    //--------------------------------------------------------------------------
    kIdx = 0;
    for ( j = 0; j < pSampleGrid->numPoints2; j++ ) // radial
    {
        // rP is the radius for the image point on the image grid
        float rP = pSampleGrid->pPoints2[j];
        float arraySize =
	    pInst->ppElementPositions[0][numTransducerElements-1] - 
            pInst->ppElementPositions[0][0];
        int numOverlapWaves;

        // compute number of overlapping beams as a function of depth normalized
        // to the array size
        numOverlapWaves = (int)iusLookUpFloatIntervalLUT( pNumOverlapBeamsLUT, 
	                                                  rP / arraySize );
 
        for ( i = 0; i < pSampleGrid->numPoints1; i++ )
        {
            // thP is the angle for the image point on the image grid
            float thP = pSampleGrid->pPoints1[i];

            // Compute the x- and z-coordinates in [m] for the current
            // output data sample which is needed to compute delays for
            // coherent compounding.
            float xP = rP * sinf(thP);
            float zP = rP * cosf(thP);

            if ( pInst->applyWeightedCompounding )
            {
                float minVal = IUS_MAXFLOAT;
                int   minIdx = 0;
                for ( k = 0; k < numTransmitWaves; k++ )
                {
                    // Find the index of the transmit focus angle which
                    // is closest to the angle thP belonging to the
                    // current image point on the image grid
                    if ( minVal > IUS_ABS(pFocusAngle[k] - IUS_PI - thP) )
                    {
                        minVal = IUS_ABS(pFocusAngle[k] - IUS_PI - thP);
                        minIdx = k;
                    }
                }
                pInst->pKStart[kIdx] = IUS_MAX( minIdx - numOverlapWaves, 0);
                pInst->pKEnd[kIdx] =
		    IUS_MIN( minIdx + numOverlapWaves + 1, numTransmitWaves );
            }
            else
            {
                pInst->pKStart[kIdx] = 0;
                pInst->pKEnd[kIdx] = numTransmitWaves;
            }

            for ( k = pInst->pKStart[kIdx]; k < pInst->pKEnd[kIdx]; k++ )
            {
                float   thF = pFocusAngle[k];
                float   xF  = focusRadius * sinf( thF );
                float   zF  = focusRadius * cosf( thF );

                float tauEc = -pInst->pDelayOffsetSec[k] +
                    sqrtf( (xP - xF) * (xP - xF) + (zP - zF) * (zP - zF) ) / 
                    speedOfSound;

                for ( n = 0; n < numTransducerElements; n++ )
                {
                    float xN = pInst->ppElementPositions[0][n];
                    float tauRec = sqrtf( zP * zP + (xP - xN) * (xP - xN) ) /
                        speedOfSound;
                    float tau = tauEc + tauRec;
                    int sampleIdx =
		        (int)( (float)pInst->upFactor * tau * sampleFrequency );

                    sampleIdx = IUS_MIN( sampleIdx,
                        pInst->upFactor * numSamplesPerLine - 1 );
                    sampleIdx = IUS_MAX( sampleIdx, 0 );

                    pInst->pDelayIndices[idx] = sampleIdx;
                    idx++;
                }
            }
            kIdx++;
        }
    }
    pInst->outputGridWasSet = 1;
}


//------------------------------------------------------------------------------
// Runs an Cwc4Dwd instance
//------------------------------------------------------------------------------
void iusCwc4DwdRun
(
    IusCwc4DwdInstance * const          pInst,
    const float * const * const * const pppIn,
    float * const * const               ppOut
)
{
    int i, j, k, n;
    int numTransmitWaves      = pInst->numTransmitWaves;
    int numTransducerElements = pInst->numTransducerElements;
    int numSamplesPerLine     = pInst->numSamplesPerLine;
    int numThetaGridPoints    = pInst->pSampleGrid->numPoints1;
    int numRadiusGridPoints   = pInst->pSampleGrid->numPoints2;
    int upFactor              = pInst->upFactor;
    float *** pppUp           = pInst->pppUp;
    //int * pDelayIdx           = pInst->pDelayIndices;;
    int idx = 0;
    int kIdx;

    IUS_ASSERT( pInst != NULL);
    IUS_ASSERT( pppIn != NULL);
    IUS_ASSERT_VALUE( pInst->outputGridWasSet == 1 ); // TODO: make error and
                                                      // return also in Release

    //--------------------------------------------------------------------------
    // Up-sample the input, line-by-line, and for each wave. This is needed
    // to achieve sub-sample accuracy in the delays applied to the sensor
    // data while accumulating data coherently for coherent wave compounding.
    //--------------------------------------------------------------------------
    for ( i = 0; i < numTransmitWaves; i++ )
    {
        for ( j = 0; j < numTransducerElements; j++ )
        {
            // treat the j-th input line for the i-th wave
            const float * pSrc = iusMatrixRow( pppIn[i], j);
            float * pDest      = iusMatrixRow( pppUp[i], j);
            iusInterpolateFloatVector( pDest, pSrc, numSamplesPerLine,
	        upFactor );

            // Apply receive apodisation
            iusScaleFloatVector( pDest, pDest, pInst->pApodR[j],
                 upFactor * numSamplesPerLine );
        }
    }

    //--------------------------------------------------------------------------
    // Apply coherent wave compounding combining all (up-sampled) sensor data
    // from all waves into one compounded data matrix on the desired output
    // grid points.
    //--------------------------------------------------------------------------
    iusZeroFloatMatrix( ppOut, numRadiusGridPoints, numThetaGridPoints );

    kIdx = 0;
    for ( j = 0; j < numRadiusGridPoints; j++ )
    {
        for ( i = 0; i < numThetaGridPoints; i++ )
        {
            for ( k = pInst->pKStart[kIdx]; k < pInst->pKEnd[kIdx]; k++ )
            {
                float** ppUp = pppUp[k]; // select the k-th wave

                int sampleIdx;

                // Run across transducers
                for ( n = 0; n < numTransducerElements; n++ )
                {
                    sampleIdx = pInst->pDelayIndices[idx];
                    ppOut[j][i] += ppUp[n][sampleIdx];
                    idx++;

                }
            }
            kIdx++;
        }
    }
}

//------------------------------------------------------------------------------
// Releases all Cwc4Dwd resources
//------------------------------------------------------------------------------
void iusCwc4DwdDestroy
(
    IusCwc4DwdInstance * pInst
)
{
    IUS_ASSERT( pInst != NULL );

    // memory for general
    iusFreeIntVector( pInst->pKEnd );
    pInst->pKEnd = NULL;
    iusFreeIntVector( pInst->pKStart );
    pInst->pKStart = NULL;
    iusFreeIntVector( pInst->pDelayIndices );
    pInst->pDelayIndices = NULL;
    //iusFreeFloatVector(pInst->pApodR); pInst->pApodR = NULL;
    iusFreeFloatVector(pInst->pDelayOffsetSec);
    pInst->pDelayOffsetSec=NULL;
    iusFreeFloatMatrix(pInst->ppElementPositions, 3);
    pInst->ppElementPositions = NULL;
    iusFreeFloatMatrix3D( pInst->pppUp, pInst->numTransmitWaves,
                          pInst->numTransducerElements);
    pInst->pppUp = NULL;

    free( pInst );
    pInst = NULL;
}

//==============================================================================
// S T A T I C   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Compute propagation times from focal point to all active elements
//------------------------------------------------------------------------------
static float LF_ComputeFocus2ElementsMinPropDelay
(
    float * * ppElemPos,             /**< x,y,z coordinates of elements in [m]*/
    int       numTransducerElements, /**< nr of transducer elements in array  */
    float     rF,                    /**< radius [m] focus point behind array */
    float     thF,                   /**< in radians                          */
    float     speedOfSound           /**< speed of sound in the medium [m/s]  */
)
{
    int i;
    float delay;     // in sec
    float delayMin;  // in sec
    float xF = rF * sinf( thF );
    float zF = rF * cosf( thF );
    float dX2, dZ2;

    IUS_ASSERT( ppElemPos != NULL );
    IUS_ASSERT_VALUE( numTransducerElements > 0 );
    IUS_ASSERT_VALUE( speedOfSound > 0.0f );

    delayMin = IUS_MAXFLOAT;
    for ( i = 0; i < numTransducerElements; i++ )
    {
        dX2 = (xF - ppElemPos[0][i]) * (xF - ppElemPos[0][i]);
        dZ2 = (zF - ppElemPos[2][i]) * (zF - ppElemPos[2][i]);
        delay = sqrtf(dX2 + dZ2) / speedOfSound;
        if ( delay < delayMin )
        {
            delayMin = delay;
        }
    }

    return delayMin;
}
