//
// Created by Ruijzendaal on 30/03/2018.
//



#define IUSLIBRARY_IMPLEMENTATION
#include <include/iusInput.h>
#include <include/iusHLTransmitPulse.h>
#include <math.h>
#include <stdlib.h>
#include <include/ius.h>


IusTransmitPulse *iusHLCreateNonParametricPulse
(
    float * pRawPulseAmplitudes,
    float * pRawPulseTimes,
    int     numPulseValues
)
{
    IusNonParametricTransmitPulse *pulse = (IusNonParametricTransmitPulse *) calloc (1,sizeof(IusNonParametricTransmitPulse));
    pulse->pRawPulseAmplitudes = pRawPulseAmplitudes;
    pulse->numPulseValues = numPulseValues;
    pulse->pRawPulseTimes = pRawPulseTimes;
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



float iusHLParametricPulseGetFrequency
(
    iutp_t transmitPulse
)
{
    if(transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseFrequency;
}


float iusHLParametricPulseGetPulseAmplitude
(
    iutp_t transmitPulse
)
{
    if(transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseAmplitude;
}

int iusHLParametricPulseGetCount
(
    iutp_t transmitPulse
)
{
    if(transmitPulse->type != IUS_PARAMETRIC_PULSETYPE)
        return -1;
    return ((IusParametricTransmitPulse *)transmitPulse)->pulseCount;
}
