//
// Created by Ruijzendaal on 10/04/2018.
//
#define IUSLIBRARY_IMPLEMENTATION
#include <stdlib.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusInput.h>
#include <include/iusUtil.h>

#include "iusHLNonParametricTransmitPulse.h"



IusNonParametricTransmitPulse *iusHLCreateNonParametricPulse
(
    int numPulseValues
)
{
    IusNonParametricTransmitPulse *pulse;
    if( numPulseValues < 0 ) return NULL;

    pulse = (IusNonParametricTransmitPulse *) calloc (1,sizeof(IusNonParametricTransmitPulse));
    if(pulse == NULL) return NULL;

    pulse->pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pulse->pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));
    if(pulse->pRawPulseAmplitudes == NULL || pulse->pRawPulseTimes == NULL ) return NULL;

    pulse->numPulseValues = numPulseValues;
    pulse->base.type = IUS_NON_PARAMETRIC_PULSETYPE;
    return pulse;
}


int iusHLDeleteNonParametricPulse
(
    IusNonParametricTransmitPulse *pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    if( pulse->base.type != IUS_NON_PARAMETRIC_PULSETYPE ) return IUS_ERR_VALUE;

    free(pulse->pRawPulseTimes);
    free(pulse->pRawPulseAmplitudes);
    free(pulse);
    return IUS_E_OK;
}



int iusCompareNonParametricPulse
(
    IusNonParametricTransmitPulse *reference,
    IusNonParametricTransmitPulse *actual
)
{
    int i;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numPulseValues != actual->numPulseValues ) return IUS_FALSE;

    for(i=0; i< reference->numPulseValues ; i++)
    {
        if( IUS_EQUAL_FLOAT(reference->pRawPulseTimes[i], actual->pRawPulseTimes[i]) == IUS_FALSE ) return IUS_FALSE;
        if( IUS_EQUAL_FLOAT(reference->pRawPulseAmplitudes[i], actual->pRawPulseAmplitudes[i]) == IUS_FALSE ) return IUS_FALSE;
    }
    return IUS_TRUE;
}



int iusHLNonParametricPulseGetNumValues
(
    IusNonParametricTransmitPulse *transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->base.type != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusNonParametricTransmitPulse *)transmitPulse)->numPulseValues;
}

int iusHLNonParametricPulseSetAmplitudeTime
(
    IusNonParametricTransmitPulse *transmitPulse,
    float pulseTime,
    float pulseAmplitude,
    int index
)
{
    IusNonParametricTransmitPulse  *pulse = (IusNonParametricTransmitPulse   *) transmitPulse;
    if(transmitPulse == NULL || transmitPulse->base.type != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    if( index >= pulse->numPulseValues )
        return IUS_ERR_VALUE;
    pulse->pRawPulseTimes[index] = pulseTime;
    pulse->pRawPulseAmplitudes[index] = pulseAmplitude;
    return IUS_E_OK;
}



