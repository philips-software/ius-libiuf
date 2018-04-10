//
// Created by Ruijzendaal on 10/04/2018.
//
#define IUSLIBRARY_IMPLEMENTATION
#include <stdlib.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusInput.h>
#include <include/iusHLTransmitPulse.h>
#include <include/iusUtil.h>
#include <math.h>

#include "iusHLParametricTransmitPulse.h"



IusParametricTransmitPulse *iusHLCreateParametricPulse
(
    float   pulseFrequency,
    float   pulseAmplitude,
    int     pulseCount
)
{
    IusParametricTransmitPulse *pulse;
    if( pulseCount < 0 ) return NULL;
    if( pulseFrequency < 0.0f ) return NULL;

    pulse = (IusParametricTransmitPulse *) calloc (1,sizeof(IusParametricTransmitPulse));
    if(pulse == NULL) return NULL;

    pulse->pulseFrequency = pulseFrequency;
    pulse->pulseAmplitude = pulseAmplitude;
    pulse->pulseCount = pulseCount;
    pulse->base.type = IUS_PARAMETRIC_PULSETYPE;
    return pulse;
}



int iusHLDeleteParametricPulse
(
    IusParametricTransmitPulse *pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    if( pulse->base.type != IUS_PARAMETRIC_PULSETYPE ) return IUS_ERR_VALUE;

    free(pulse);
    return IUS_E_OK;
}


int iusCompareParametricPulse
(
    IusParametricTransmitPulse *reference,
    IusParametricTransmitPulse *actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->pulseCount != actual->pulseCount ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseAmplitude, actual->pulseAmplitude) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseFrequency, actual->pulseFrequency) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}


float iusHLParametricPulseGetFrequency
(
    IusParametricTransmitPulse *transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->base.type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseFrequency;
}


float iusHLParametricPulseGetPulseAmplitude
(
    IusParametricTransmitPulse *transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->base.type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseAmplitude;
}

int iusHLParametricPulseGetCount
(
    IusParametricTransmitPulse *transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->base.type != IUS_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseCount;
}
