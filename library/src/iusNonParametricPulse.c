//
// Created by Ruijzendaal on 10/04/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusPulsePrivate.h>

struct IusNonParametricPulse
{
    struct IusPulse base;         /**< The basic/common properties of a pulse  */
    int     numPulseValues;       /**< number of points to describe waveform, 0 implies a parametric description only */
    float * pRawPulseAmplitudes;  /**< shape of waveform [in Volts] */
    float * pRawPulseTimes;       /**< corresponding timestamps of amplitudes [in seconds] */
} ;

iunpp_t iusNonParametricPulseCreate
(
    int numPulseValues
)
{
    IusNonParametricPulse *pulse;

    IUS_ERR_EVAL_N_RETURN(numPulseValues < 0, IUNPP_INVALID);
    pulse = (IusNonParametricPulse *) calloc (1,sizeof(IusNonParametricPulse));
    IUS_ERR_ALLOC_NULL_N_RETURN(pulse, IusNonParametricPulse, IUNPP_INVALID);

    pulse->pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pulse->pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));
    if(pulse->pRawPulseAmplitudes == NULL || pulse->pRawPulseTimes == NULL ) return NULL;

    pulse->numPulseValues = numPulseValues;
    pulse->base.type = IUS_NON_PARAMETRIC_PULSETYPE;
    return pulse;
}


int iusNonParametricPulseDelete
(
    iunpp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    free(pulse->pRawPulseTimes);
    free(pulse->pRawPulseAmplitudes);
    free(pulse);
    return IUS_E_OK;
}


int iusNonParametricPulseCompare
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
    return iusBasePulseCompare((iup_t)reference,(iup_t)actual);
}


int iusNonParametricPulseGetNumValues
(
    iunpp_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, -1);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, -1);
    return pulse->numPulseValues;
}

float iusNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    return pulse->pRawPulseAmplitudes[index];
}


float iusNonParametricPulseGetValueTime
(
    iunpp_t pulse,
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    return pulse->pRawPulseTimes[index];
}


int iusNonParametricPulseSetValue
(
    iunpp_t pulse,
    int index,
    float pulseTime,
    float pulseAmplitude
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(pulseTime < 0.0f, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(index < 0 || index >= pulse->numPulseValues, IUS_ERR_VALUE);
    pulse->pRawPulseTimes[index] = pulseTime;
    pulse->pRawPulseAmplitudes[index] = pulseAmplitude;
    return IUS_E_OK;
}

int iusNonParametricPulseSave
(
    iunpp_t pulse,
    hid_t handle
)
{
    int status=0;

    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(iusPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status = iusBasePulseSave((iup_t)pulse, handle);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(pulse->numPulseValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = pulse->numPulseValues;
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, 1, dims, pulse->pRawPulseAmplitudes );
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSETIMES, 1, dims, pulse->pRawPulseTimes );
    return status;
}

iunpp_t iusNonParametricPulseLoad
(
    hid_t handle
)
{
    int status = 0;
    int  numPulseValues;

    iunpp_t  pulse;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUNPP_INVALID);
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(numPulseValues));
    if( status < 0 )
        return IUNPP_INVALID;

    pulse = iusNonParametricPulseCreate(numPulseValues);
    status |= H5LTread_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, pulse->pRawPulseAmplitudes );
    status |= H5LTread_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSETIMES, pulse->pRawPulseTimes );

	if( status < 0 )
      return IUNPP_INVALID;
    return pulse;
}

