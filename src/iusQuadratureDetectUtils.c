//==============================================================================
//    COPYRIGHT 2016 PHILIPS RESEARCH
//==============================================================================
//
//  Source Name   : iusQuadratureDetectUtils.c
//
//  iusVersion    : 255.255.255.255
//
//==============================================================================


#include "iusQuadratureDetectUtils.h"
#include "iusBasicFunctions.h"        // for IUS_FLT_EPS
#include "iusComplex.h"
#include "iusError.h"                      //
#include <math.h>                          // for atan2()
#include <stdio.h>

//==============================================================================
// L O C A L   D E F I N E S   A N D   V A R I A B L E S
//==============================================================================

//==============================================================================
// S T A T I C   F U N C T I O N   P R O T O T Y P E S
//==============================================================================

//==============================================================================
// E X P O R T E D   F U N C T I O N S
//==============================================================================

//------------------------------------------------------------------------------
// Runs a QuadratureDetect4SamplesPerWave instance
//------------------------------------------------------------------------------
void iusQuadratureDetect4SamplesPerWave
(
    const float * const * const   ppRfLinesIn,
    int                           numLines,
    int                           numSamplesPerLine,
    iusComplexFloatMatrix * const pIQ,
    int                           numLinesOut,
    int                           numSamplesPerLineOut
)
{
    int i, j;
    float pW[] = { 1.0f, -1.0f };
    IUS_UNUSED(numLinesOut);       // avoid compiler warning on unreferenced parameter
    IUS_UNUSED(numSamplesPerLine); // avoid compiler warning on unreferenced parameter

    IUS_ASSERT( ppRfLinesIn != NULL );
	IUS_ASSERT( pIQ != NULL );
	IUS_ASSERT_VALUE( numLinesOut == numLines );
	IUS_ASSERT_VALUE( numSamplesPerLineOut == numSamplesPerLine / 2 );

    for ( i = 0; (i < numLines); i++ )
    {
        const float * const pR = ppRfLinesIn[i];
        float * const pI       = pIQ->ppRe[i];
        float * const pQ       = pIQ->ppIm[i];
        for ( j = 0; j < numSamplesPerLineOut; j++ )
        {
            float w = pW[j % 2];
            
            pI[j] = w * pR[2 * j];
            pQ[j] = w * pR[2 * j + 1];
        }
    }
}

//------------------------------------------------------------------------------
// Compute from a packet of quadrature component vectors I and Q one vector of
// velocity values (in amount of samples displacement per subsequent line
// period) and one vector of velocity std values.
//------------------------------------------------------------------------------
void iusKasaiCorr1D

(
    const iusComplexFloatMatrix * const pIQ,
    int                                 ensembleSize,
    int                                 numSamplesPerLine,
    int                                 packetSize,
    float * const                       pVOut,
    float * const                       pSigmaVOut,
    int                                 numElemOut
)
{
    int i, j, k;
    IUS_UNUSED(numSamplesPerLine); // avoid compiler warning on unreferenced parameter

	IUS_ASSERT( (pVOut != NULL) && (pSigmaVOut != NULL) && (pIQ != NULL) );
	IUS_ASSERT_VALUE( numElemOut == numSamplesPerLine / packetSize );

    for ( k = 0; k < numElemOut; k++ )
    {
        float s1sum = 0.0f;
        float s2sum = 0.0f;
        float wdsum = IUS_FLT_EPS;

        for ( j = 0; j < packetSize; j++ )
        {
            for ( i = 0; i < ensembleSize - 1; i++ )
            {
                float prvI = pIQ->ppRe[i][k * packetSize + j];
                float prvQ = pIQ->ppIm[i][k * packetSize + j];
                float curI = pIQ->ppRe[i + 1][k * packetSize + j];
                float curQ = pIQ->ppIm[i + 1][k * packetSize + j];
                s1sum += curQ * prvI - prvQ * curI;
                s2sum += curI * prvI + curQ * prvQ;
                wdsum += curI * curI + curQ * curQ;
            }
        }
        pVOut[k] = (float)atan2( s1sum, s2sum );
        pSigmaVOut[k] = IUS_MAX( 1.0f - (float)sqrt( s1sum * s1sum + s2sum * s2sum ) / wdsum, 0.0f );
    }
}


//------------------------------------------------------------------------------
// Compute from a packet of quadrature component vectors I and Q one velocity
// value (in amount of samples displacement per subsequent line period) and one
// velocity std value.
//------------------------------------------------------------------------------
void iusKasaiPerPacketCorr1D
(
    const iusComplexFloatVector * const pIQ,
    int                                 ensembleSize,
    float                               vOut,
    float                               sigmaVOut
)
{
    int i;

	IUS_ASSERT( (pIQ != NULL) );

    float s1sum = 0.0f;
    float s2sum = 0.0f;
    float wdsum = IUS_FLT_EPS;
    for ( i = 0; i < ensembleSize - 1; i++ )
    {
        float prvI = pIQ->pRe[i];
        float prvQ = pIQ->pIm[i];
        float curI = pIQ->pRe[i + 1];
        float curQ = pIQ->pIm[i + 1];
        s1sum += curQ * prvI - prvQ * curI;
        s2sum += curI * prvI + curQ * prvQ;
        wdsum += curI * curI + curQ * curQ;
    }

    vOut = (float)atan2(s1sum, s2sum);
    sigmaVOut = IUS_MAX(1.0f - (float)sqrt(s1sum * s1sum + s2sum * s2sum) / wdsum, 0.0f);
}

//------------------------------------------------------------------------------
// Compute from a packet of quadrature component vectors I and Q one velocity
// value (in amount of samples displacement per subsequent line period) and one
// velocity std value.
//------------------------------------------------------------------------------
void iusKasaiMultiplePacketsCorr1D
(
    const iusComplexFloatMatrix * const pIqPacketData,
    int                                 numPackets,
    int                                 ensembleSize,
    float * const                       pVOut,
    float * const                       pSigmaVOut
)
{
    int i, j;
    float s1sum, s2sum, wdsum;

    IUS_ASSERT( (pIqPacketData != NULL) );

	for ( j = 0; j < numPackets; j++ )
    {
        s1sum = 0.0f;
        s2sum = 0.0f;
        wdsum = IUS_FLT_EPS;

        for ( i = 0; i < ensembleSize - 1; i++ )
        {
            float prvI = pIqPacketData->ppRe[j][i];
            float prvQ = pIqPacketData->ppIm[j][i];
            float curI = pIqPacketData->ppRe[j][i + 1];
            float curQ = pIqPacketData->ppIm[j][i + 1];
            s1sum += curQ * prvI - prvQ * curI;
            s2sum += curI * prvI + curQ * prvQ;
            wdsum += curI * curI + curQ * curQ;
        }

        pVOut[j] = (float)atan2(s1sum, s2sum);
        pSigmaVOut[j] = IUS_MAX(1.0f - (float)sqrt(s1sum * s1sum + s2sum * s2sum) / wdsum, 0.0f);

	}
}
