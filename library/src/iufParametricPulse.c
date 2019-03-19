//
// Created by Ruijzendaal on 10/04/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufPulsePrivate.h>

// ADT
struct IufParametricPulse
{
    struct  IufPulse base;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses;           /**< number of cycles that the pulse represents */
} ;

iupp_t iufParametricPulseCreate
(
    float pulseFrequency,
    float pulseAmplitude,
    int numPulses
)
{
    IufParametricPulse *pulse;
    IUF_ERR_EVAL_N_RETURN(numPulses < 0, IUPP_INVALID);
    IUF_ERR_EVAL_N_RETURN(pulseFrequency < 0.0f, IUPP_INVALID);

    pulse = (IufParametricPulse *) calloc (1,sizeof(IufParametricPulse));
    IUF_ERR_ALLOC_NULL_N_RETURN(pulse, IufParametricPulse, IUPP_INVALID);

    pulse->pulseFrequency = pulseFrequency;
    pulse->pulseAmplitude = pulseAmplitude;
    pulse->numPulses = numPulses;
    pulse->base.type = IUF_PARAMETRIC_PULSETYPE;
    return pulse;
}



int iufParametricPulseDelete
(
    iupp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType((iup_t)pulse ) != IUF_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    free(pulse);
    return IUF_E_OK;
}


IUF_BOOL iufParametricPulseCompare
(
    iupp_t reference,
    iupp_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numPulses != actual->numPulses ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->pulseAmplitude, actual->pulseAmplitude) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->pulseFrequency, actual->pulseFrequency) == IUF_FALSE ) return IUF_FALSE;
    return iufBasePulseCompare((iup_t)reference,(iup_t)actual);
}


float iufParametricPulseGetFrequency
(
    iupp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, NAN);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t) pulse ) != IUF_PARAMETRIC_PULSETYPE, NAN);
    return ((IufParametricPulse *)pulse)->pulseFrequency;
}


float iufParametricPulseGetPulseAmplitude
(
    iupp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, NAN);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t) pulse ) != IUF_PARAMETRIC_PULSETYPE, NAN);
    return ((IufParametricPulse *)pulse)->pulseAmplitude;
}

int iufParametricPulseGetNumPulses
(
    iupp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, -1);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t) pulse ) != IUF_PARAMETRIC_PULSETYPE, -1);
    return pulse->numPulses;
}

int iufParametricPulseSave
(
    iupp_t pulse,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t) pulse ) != IUF_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = iufBasePulseSave((iup_t)pulse, handle);
    if (status == IUF_ERR_VALUE) return status;
    status = iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulse->pulseFrequency), 1);
    status |= iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulse->pulseAmplitude), 1);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_NUMPULSES, &(pulse->numPulses), 1);
    return status;
}

iupp_t iufParametricPulseLoad
(
    hid_t handle
)
{
    int     status = 0;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses;           /**< number of cycles that the pulse represents */
    iupp_t  pulse;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPP_INVALID);
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulseFrequency));
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulseAmplitude));
    status |= iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_NUMPULSES, &(numPulses));

    if( status < 0 )
        return NULL;

    pulse = iufParametricPulseCreate(pulseFrequency,pulseAmplitude,numPulses);
    return pulse;
}
