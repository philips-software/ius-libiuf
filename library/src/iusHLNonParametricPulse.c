//
// Created by Ruijzendaal on 10/04/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLPulse.h>

#include <iusHLNonParametricPulse.h>
#include <iusHLPulseImp.h>
#include <include/iusHDF5.h>

struct IusNonParametricPulse
{
    struct IusPulse base;
    int     numPulseValues;       /**< number of points to describe waveform, 0 implies a parametric description only */
    float * pRawPulseAmplitudes;  /**< shape of waveform [in Volts] */
    float * pRawPulseTimes;       /**< corresponding timestamps of amplitudes [in seconds] */
} ;


iunpp_t iusHLNonParametricPulseCreate
(
    char *label,
    int numPulseValues
)
{
    IusNonParametricPulse *pulse;
    if( label == NULL ) return NULL;
    if( strcmp(label,"") == 0 ) return NULL;
    if( numPulseValues < 0 ) return NULL;

    pulse = (IusNonParametricPulse *) calloc (1,sizeof(IusNonParametricPulse));
    if(pulse == NULL) return NULL;

    pulse->pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pulse->pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));
    if(pulse->pRawPulseAmplitudes == NULL || pulse->pRawPulseTimes == NULL ) return NULL;

    pulse->numPulseValues = numPulseValues;
    pulse->base.type = IUS_NON_PARAMETRIC_PULSETYPE;
    pulse->base.label = strdup(label);
    return pulse;
}


int iusHLNonParametricPulseDelete
(
    iunpp_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    if(iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE ) return IUS_ERR_VALUE;

    free(pulse->pRawPulseTimes);
    free(pulse->pRawPulseAmplitudes);
    free(pulse);
    return IUS_E_OK;
}



int iusHLNonParametricPulseCompare
(
    iunpp_t reference,
    iunpp_t actual
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
    iunpp_t pulse
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusNonParametricPulse *)pulse)->numPulseValues;
}

float iusHLNonParametricPulseGetAmplitude
(
    iunpp_t pulse,
    int index
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return pulse->pRawPulseAmplitudes[index];
}


float iusHLNonParametricPulseGetTime
(
    iunpp_t pulse,
    int index
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return pulse->pRawPulseTimes[index];
}


int iusHLNonParametricPulseSetAmplitudeTime
(
    iunpp_t pulse,
    float pulseTime,
    float pulseAmplitude,
    int index
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    if(pulseTime < 0.0f)
        return IUS_ERR_VALUE;
    if( index < 0 || index >= pulse->numPulseValues )
        return IUS_ERR_VALUE;
    pulse->pRawPulseTimes[index] = pulseTime;
    pulse->pRawPulseAmplitudes[index] = pulseAmplitude;
    return IUS_E_OK;
}


#define NUMPULSEVALUESFMT "%s/numpulseValues"

int iusHLNonParametricPulseSave
(
    iunpp_t pulse,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[64];

    status |= iusHLPulseSave((iup_t)pulse,parentPath,handle);
    sprintf(path, NUMPULSEVALUESFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(pulse->numPulseValues), 1);
    return status;
}

iunpp_t iusHLNonParametricPulseLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status = 0;
    char *label;
    char path[64];
    int  numPulseValues;

    iunpp_t  pulse;

    iup_t basePulse = iusHLPulseLoad( handle, parentPath);
    if( basePulse == IUP_INVALID )
        return IUNPP_INVALID;

    label = iusHLPulseGetLabel(basePulse);
    sprintf(path, NUMPULSEVALUESFMT, parentPath);
    status |= iusHdf5ReadInt(handle, path, &(numPulseValues));
    if( status < 0 )
        return NULL;
    pulse = iusHLNonParametricPulseCreate(label,numPulseValues);
    return pulse;
}

