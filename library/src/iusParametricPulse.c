//
// Created by Ruijzendaal on 10/04/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusPulsePrivate.h>

// ADT
struct IusParametricPulse
{
    struct  IusPulse base;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses;           /**< number of cycles that the pulse represents */
} ;

iupp_t iusParametricPulseCreate
(
    float pulseFrequency,
    float pulseAmplitude,
    int numPulses
)
{
    IusParametricPulse *pulse;
    IUS_ERR_EVAL_N_RETURN(numPulses < 0, IUPP_INVALID);
    IUS_ERR_EVAL_N_RETURN(pulseFrequency < 0.0f, IUPP_INVALID);

    pulse = (IusParametricPulse *) calloc (1,sizeof(IusParametricPulse));
    IUS_ERR_ALLOC_NULL_N_RETURN(pulse, IusParametricPulse, IUPP_INVALID);

    pulse->pulseFrequency = pulseFrequency;
    pulse->pulseAmplitude = pulseAmplitude;
    pulse->numPulses = numPulses;
    pulse->base.type = IUS_PARAMETRIC_PULSETYPE;
    return pulse;
}



int iusParametricPulseDelete
(
    iupp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType((iup_t)pulse ) != IUS_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
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
    if( reference->numPulses != actual->numPulses ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseAmplitude, actual->pulseAmplitude) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->pulseFrequency, actual->pulseFrequency) == IUS_FALSE ) return IUS_FALSE;
    return iusBasePulseCompare((iup_t)reference,(iup_t)actual);
}


float iusParametricPulseGetFrequency
(
    iupp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, NAN);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE, NAN);
    return ((IusParametricPulse *)pulse)->pulseFrequency;
}


float iusParametricPulseGetPulseAmplitude
(
    iupp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, NAN);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE, NAN);
    return ((IusParametricPulse *)pulse)->pulseAmplitude;
}

int iusParametricPulseGetNumPulses
(
    iupp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, -1);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE, -1);
    return pulse->numPulses;
}

int iusParametricPulseSave
(
    iupp_t pulse,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t) pulse ) != IUS_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status = iusBasePulseSave((iup_t)pulse, handle);
    if (status == IUS_ERR_VALUE) return status;
    status = iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulse->pulseFrequency), 1);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulse->pulseAmplitude), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_NUMPULSES, &(pulse->numPulses), 1);
    return status;
}

iupp_t iusParametricPulseLoad
(
    hid_t handle
)
{
    int     status = 0;
    float   pulseFrequency;       /**< frequency that the pulse represents in Hz */
    float   pulseAmplitude;       /**< (max) amplitude of the pulse in Volts */
    int     numPulses;           /**< number of cycles that the pulse represents */
    iupp_t  pulse;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPP_INVALID);
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_PULSE_FREQUENCY, &(pulseFrequency));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_PULSE_PULSEAMPLITUDES, &(pulseAmplitude));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_NUMPULSES, &(numPulses));

    if( status < 0 )
        return NULL;

    pulse = iusParametricPulseCreate(pulseFrequency,pulseAmplitude,numPulses);
    return pulse;
}
