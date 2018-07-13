//
// Created by Ruijzendaal on 10/04/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusError.h>
#include <iusHLPulse.h>
#include <iusUtil.h>
#include <iusHDF5.h>

#include <iusHLParametricPulse.h>
#include <iusHLPulseImp.h>

// ADT
struct IusParametricPulse
{
    struct  IusPulse base;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount;           /**< number of cycles that the pulse represents */
} ;


iupp_t iusHLParametricPulseCreate
(
    char *label,
    float pulseFrequency,
    float pulseAmplitude,
    int pulseCount
)
{
    IusParametricPulse *pulse;
    if( label == NULL ) return NULL;
    if( strcmp(label,"") == 0 ) return NULL;
    if( pulseCount < 0 ) return NULL;
    if( pulseFrequency < 0.0f ) return NULL;

    pulse = (IusParametricPulse *) calloc (1,sizeof(IusParametricPulse));
    if(pulse == NULL) return NULL;

    pulse->pulseFrequency = pulseFrequency;
    pulse->pulseAmplitude = pulseAmplitude;
    pulse->pulseCount = pulseCount;
    pulse->base.type = IUS_PARAMETRIC_PULSETYPE;
    pulse->base.label = strdup(label);
    return pulse;
}



int iusHLParametricPulseDelete
(
    iupp_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    if(iusHLPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE ) return IUS_ERR_VALUE;
    free(pulse);
    return IUS_E_OK;
}


IUS_BOOL iusHLParametricPulseCompare
(
    iupp_t reference,
    iupp_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->pulseCount != actual->pulseCount ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseAmplitude, actual->pulseAmplitude) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseFrequency, actual->pulseFrequency) == IUS_FALSE ) return IUS_FALSE;
    return iusHLBasePulseCompare((iup_t)reference,(iup_t)actual);
}


float iusHLParametricPulseGetFrequency
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricPulse *)pulse)->pulseFrequency;
}


float iusHLParametricPulseGetPulseAmplitude
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricPulse *)pulse)->pulseAmplitude;
}

int iusHLParametricPulseGetCount
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusParametricPulse *)pulse)->pulseCount;
}


// float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
// float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
// int     pulseCount;           /**< number of cycles that the pulse represents */
#define FREQUENCYFMT "%s/pulseFrequency"
#define AMPLITUDEFMT "%s/pulseAmplitude"
#define COUNTFMT "%s/pulseCount"

int iusHLParametricPulseSave
(
    iupp_t pulse,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[64];

    status = iusHLBasePulseSave((iup_t)pulse,parentPath,handle);
    sprintf(path, FREQUENCYFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, path, &(pulse->pulseFrequency), 1);
    sprintf(path, AMPLITUDEFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, path, &(pulse->pulseAmplitude), 1);
    sprintf(path, COUNTFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(pulse->pulseCount), 1);
    return status;
}

iupp_t iusHLParametricPulseLoad
(
    hid_t handle,
    char *parentPath,
    char *label
)
{
    int status = 0;
    char path[64];
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount;           /**< number of cycles that the pulse represents */
    iupp_t  pulse;


    sprintf(path, FREQUENCYFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(pulseFrequency));
    sprintf(path, AMPLITUDEFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(pulseAmplitude));
    sprintf(path, COUNTFMT, parentPath);
    status |= iusHdf5ReadInt( handle, path, &(pulseCount));

    if( status < 0 )
        return NULL;
    pulse = iusHLParametricPulseCreate(label,pulseFrequency,pulseAmplitude,pulseCount);
    return pulse;
}
