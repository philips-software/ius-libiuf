//
// Created by Ruijzendaal on 30/03/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <include/iusInput.h>
#include <include/iusHLTransmitPulse.h>
#include <math.h>
#include <stdlib.h>
#include <include/ius.h>
#include <include/iusUtil.h>
#include <include/iusError.h>


IusTransmitPulse *iusHLCreateNonParametricPulse
(
    int     numPulseValues
)
{
    IusNonParametricTransmitPulse *pulse = (IusNonParametricTransmitPulse *) calloc (1,sizeof(IusNonParametricTransmitPulse));
    if(pulse == NULL) return NULL;

    pulse->pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pulse->pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));
    if(pulse->pRawPulseAmplitudes == NULL || pulse->pRawPulseTimes == NULL ) return NULL;

    pulse->numPulseValues = numPulseValues;
    pulse->base.type = IUS_NON_PARAMETRIC_PULSETYPE;
    return (IusTransmitPulse *)pulse;
}


int iusHLDeleteTransmitPulse
(
    IusTransmitPulse *pulse
)
{
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

int iusCompareParametricPulse
    (
        IusParametricTransmitPulse *reference,
        IusParametricTransmitPulse *actual
    )
{
    int i;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->pulseCount != actual->pulseCount ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseAmplitude, actual->pulseAmplitude) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseFrequency, actual->pulseFrequency) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

int iusCompareTransmitPulse
(
    IusTransmitPulse *reference,
    IusTransmitPulse *actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;

    if( reference->type != actual->type )
        return IUS_FALSE;

    if( reference->type == IUS_NON_PARAMETRIC_PULSETYPE  )
    {
        return iusCompareNonParametricPulse((IusNonParametricTransmitPulse *)reference,(IusNonParametricTransmitPulse *)actual);
    }

    if( reference->type == IUS_PARAMETRIC_PULSETYPE  )
    {
        return iusCompareParametricPulse((IusParametricTransmitPulse *)reference,(IusParametricTransmitPulse *)actual);
    }
    return IUS_FALSE;
}

float iusHLParametricPulseGetFrequency
(
    iutp_t transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseFrequency;
}


float iusHLParametricPulseGetPulseAmplitude
(
    iutp_t transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseAmplitude;
}

int iusHLParametricPulseGetCount
(
    iutp_t transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseCount;
}


int iusHLNonParametricPulseGetCount
(
    iutp_t transmitPulse
)
{
    if(transmitPulse == NULL || transmitPulse->type != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusNonParametricTransmitPulse *)transmitPulse)->numPulseValues;
}

int iusHLNonParametricPulseSetAmplitudeTime
(
    iutp_t transmitPulse,
    float pulseTime,
    float pulseAmplitude,
    int index
)
{
    IusNonParametricTransmitPulse  *pulse = (IusNonParametricTransmitPulse   *) transmitPulse;
    if(transmitPulse == NULL || transmitPulse->type != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    if( index >= pulse->numPulseValues )
        return IUS_ERR_VALUE;
    pulse->pRawPulseTimes[index] = pulseTime;
    pulse->pRawPulseAmplitudes[index] = pulseAmplitude;
}