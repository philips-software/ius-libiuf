//
// Created by Ruijzendaal on 10/04/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusError.h>
#include <iusPulse.h>
#include <iusUtil.h>
#include <iusHDF5.h>
#include <iusInputFileStructure.h>
#include <iusParametricPulse.h>
#include <iusPulsePrivate.h>

// ADT
struct IusParametricPulse
{
    struct  IusPulse base;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount;           /**< number of cycles that the pulse represents */
} ;

iupp_t iusParametricPulseCreate
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



int iusParametricPulseDelete
(
    iupp_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    if(iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE ) return IUS_ERR_VALUE;
    free(pulse);
    return IUS_E_OK;
}


IUS_BOOL iusParametricPulseCompare
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
    return iusBasePulseCompare((iup_t)reference,(iup_t)actual);
}


float iusParametricPulseGetFrequency
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricPulse *)pulse)->pulseFrequency;
}


float iusParametricPulseGetPulseAmplitude
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return NAN;
    return ((IusParametricPulse *)pulse)->pulseAmplitude;
}

int iusParametricPulseGetCount
(
    iupp_t pulse
)
{
    if(pulse == NULL || iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return ((IusParametricPulse *)pulse)->pulseCount;
}

int iusParametricPulseSave
(
    iupp_t pulse,
    hid_t handle
)
{
    int status=0;
	if (pulse == NULL || iusPulseGetType((iup_t)pulse) != IUS_PARAMETRIC_PULSETYPE)
		return IUS_ERR_VALUE;
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	//TODO chekc if /Pulses exist
    status = iusBasePulseSave((iup_t)pulse, handle);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulse->pulseFrequency), 1);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulse->pulseAmplitude), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PULSE_COUNT, &(pulse->pulseCount), 1);

    return status;
}

iupp_t iusParametricPulseLoad
(
    hid_t handle
)
{
    int     status = 0;
    char    *label;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     pulseCount;           /**< number of cycles that the pulse represents */
    iupp_t  pulse;

	if (handle == H5I_INVALID_HID)
		return NULL;
	status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PULSE_PULSELABEL, (const char **)&(label));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulseFrequency));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulseAmplitude));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_PULSE_COUNT, &(pulseCount));

    if( status < 0 )
        return NULL;

    pulse = iusParametricPulseCreate(label,pulseFrequency,pulseAmplitude,pulseCount);
    return pulse;
}
