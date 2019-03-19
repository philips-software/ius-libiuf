//
// Created by Ruijzendaal on 10/04/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufPulsePrivate.h>

struct IufNonParametricPulse
{
    struct IufPulse base;         /**< The basic/common properties of a pulse  */
    int     numPulseValues;       /**< number of points to describe waveform, 0 implies a parametric description only */
    float * pRawPulseAmplitudes;  /**< shape of waveform [in Volts] */
    float * pRawPulseTimes;       /**< corresponding timestamps of amplitudes [in seconds] */
} ;

iunpp_t iufNonParametricPulseCreate
(
    int numPulseValues
)
{
    IufNonParametricPulse *pulse;

    IUF_ERR_EVAL_N_RETURN(numPulseValues < 0, IUNPP_INVALID);
    pulse = (IufNonParametricPulse *) calloc (1,sizeof(IufNonParametricPulse));
    IUF_ERR_ALLOC_NULL_N_RETURN(pulse, IufNonParametricPulse, IUNPP_INVALID);

    pulse->pRawPulseAmplitudes = (float *)calloc(numPulseValues, sizeof(float));
    pulse->pRawPulseTimes = (float *)calloc(numPulseValues, sizeof(float));
    if(pulse->pRawPulseAmplitudes == NULL || pulse->pRawPulseTimes == NULL ) return NULL;

    pulse->numPulseValues = numPulseValues;
    pulse->base.type = IUF_NON_PARAMETRIC_PULSETYPE;
    return pulse;
}


int iufNonParametricPulseDelete
(
    iunpp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    free(pulse->pRawPulseTimes);
    free(pulse->pRawPulseAmplitudes);
    free(pulse);
    return IUF_E_OK;
}


int iufNonParametricPulseCompare
(
    iunpp_t reference,
    iunpp_t actual
)
{
    int i;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numPulseValues != actual->numPulseValues ) return IUF_FALSE;

    for(i=0; i< reference->numPulseValues ; i++)
    {
        if( IUF_EQUAL_FLOAT(reference->pRawPulseTimes[i], actual->pRawPulseTimes[i]) == IUF_FALSE ) return IUF_FALSE;
        if( IUF_EQUAL_FLOAT(reference->pRawPulseAmplitudes[i], actual->pRawPulseAmplitudes[i]) == IUF_FALSE ) return IUF_FALSE;
    }
    return iufBasePulseCompare((iup_t)reference,(iup_t)actual);
}


int iufNonParametricPulseGetNumValues
(
    iunpp_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, -1);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, -1);
    return pulse->numPulseValues;
}

float iufNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    return pulse->pRawPulseAmplitudes[index];
}


float iufNonParametricPulseGetValueTime
(
    iunpp_t pulse,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    return pulse->pRawPulseTimes[index];
}


int iufNonParametricPulseSetValue
(
    iunpp_t pulse,
    int index,
    float pulseTime,
    float pulseAmplitude
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(pulseTime < 0.0f, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0 || index >= pulse->numPulseValues, IUF_ERR_VALUE);
    pulse->pRawPulseTimes[index] = pulseTime;
    pulse->pRawPulseAmplitudes[index] = pulseAmplitude;
    return IUF_E_OK;
}

int iufNonParametricPulseSave
(
    iunpp_t pulse,
    hid_t handle
)
{
    int status=0;

    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(iufPulseGetType( (iup_t)pulse ) != IUF_NON_PARAMETRIC_PULSETYPE, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status = iufBasePulseSave((iup_t)pulse, handle);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(pulse->numPulseValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = pulse->numPulseValues;
    status |= H5LTmake_dataset_float(handle, IUF_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, 1, dims, pulse->pRawPulseAmplitudes );
    status |= H5LTmake_dataset_float(handle, IUF_INPUTFILE_PATH_PULSE_RAWPULSETIMES, 1, dims, pulse->pRawPulseTimes );
    return status;
}

iunpp_t iufNonParametricPulseLoad
(
    hid_t handle
)
{
    int status = 0;
    int  numPulseValues;

    iunpp_t  pulse;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUNPP_INVALID);
    status |= iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(numPulseValues));
    if( status < 0 )
        return IUNPP_INVALID;

    pulse = iufNonParametricPulseCreate(numPulseValues);
    status |= H5LTread_dataset_float(handle, IUF_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, pulse->pRawPulseAmplitudes );
    status |= H5LTread_dataset_float(handle, IUF_INPUTFILE_PATH_PULSE_RAWPULSETIMES, pulse->pRawPulseTimes );

	if( status < 0 )
      return IUNPP_INVALID;
    return pulse;
}

