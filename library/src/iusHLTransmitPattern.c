//
// Created by Ruijzendaal on 04/04/2018.
//

#define IUSLIBRARY_IMPLEMENTATION

#include <include/iusInput.h>
#include <iusHLTransmitPattern.h>
#include <stdlib.h>
#include <include/ius.h>
#include <math.h>
#include <include/iusError.h>
#include <include/iusUtil.h>


int iusHLTransmitPatternSet
(
    IusTransmitPattern *pattern,
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
    IusTransmitPattern *pattern
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

int iusCompareTransmitPattern
(
    IusTransmitPattern *reference,
    IusTransmitPattern *actual
)
{
    if( reference->pulseIndex != actual->pulseIndex ) return IUS_FALSE;
    if( reference->sourceIndex != actual->sourceIndex ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->time, actual->time) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}
