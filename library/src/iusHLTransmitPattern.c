//
// Created by Ruijzendaal on 04/04/2018.
//

#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLTransmitPattern.h>


// ADT
/** \brief Transmitpattern point (time, index) i.e. source triggers at time */
struct IusTransmitPattern
{
    int    sourceIndex;       /**< index of transmit source */
    int    pulseIndex;   /**< index of transmit pulse */
    float  time;        /**< time to the source transmits */
} ;



int iusHLTransmitPatternSet
(
    iutpa_t pattern,
    float time,
    int sourceIndex,
    int pulseIndex
)
{
    int status = IUS_E_OK;
    if( pulseIndex < 0 ) return IUS_ERR_VALUE;
    pattern->pulseIndex = pulseIndex;
    pattern->sourceIndex = sourceIndex;
    pattern->time = time;
    return status;
}


float iusHLTransmitPatternGetTime
(
    iutpa_t pattern
)
{
    if( pattern == NULL ) return NAN;
    return pattern->time;
}


int iusHLTransmitPatternGetSourceIndex
(
    iutpa_t pattern
)
{
    if( pattern == NULL ) return -1;
    return pattern->sourceIndex;
}

int iusHLTransmitPatternGetPulseIndex
(
    iutpa_t pattern
)
{
    if( pattern == NULL ) return -1;
    return pattern->pulseIndex;
}

int iusHLTransmitPatternCompare
(
iutpa_t reference,
iutpa_t actual
)
{
    if( reference->pulseIndex != actual->pulseIndex ) return IUS_FALSE;
    if( reference->sourceIndex != actual->sourceIndex ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->time, actual->time) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}
