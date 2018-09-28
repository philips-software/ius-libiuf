//
// Created by Ruijzendaal on 10/04/2018.
//
#include <stdlib.h>
#include <string.h>

#include <hdf5.h>
#include <hdf5_hl.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHLPulse.h>
#include <iusInputFileStructure.h>
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
    return iusHLBasePulseCompare((iup_t)reference,(iup_t)actual);
}


int iusHLNonParametricPulseGetNumValues
(
    iunpp_t pulse
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return -1;
    return ((IusNonParametricPulse *)pulse)->numPulseValues;
}

float iusHLNonParametricPulseGetValueAmplitude
(
    iunpp_t pulse,
    int index
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return pulse->pRawPulseAmplitudes[index];
}


float iusHLNonParametricPulseGetValueTime
(
    iunpp_t pulse,
    int index
)
{
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    return pulse->pRawPulseTimes[index];
}


int iusHLNonParametricPulseSetValue
(
    iunpp_t pulse,
    int index,
    float pulseTime,
    float pulseAmplitude
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

int iusHLNonParametricPulseSave
(
    iunpp_t pulse,
    hid_t handle
)
{
    int status=0;

	if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;


    status = iusHLBasePulseSave((iup_t)pulse, handle);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(pulse->numPulseValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = pulse->numPulseValues;
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, 1, dims, pulse->pRawPulseAmplitudes );
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSETIMES, 1, dims, pulse->pRawPulseTimes );
    return status;
}

iunpp_t iusHLNonParametricPulseLoad
(
    hid_t handle
)
{
    int status = 0;
    char *label;
    int  numPulseValues;

    iunpp_t  pulse;

    if (handle == H5I_INVALID_HID)
        return NULL;

    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_PULSE_NUMPULSEVALUES, &(numPulseValues));
	status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PULSE_PULSELABEL, &(label));
    if( status < 0 )
        return NULL;

    pulse = iusHLNonParametricPulseCreate(label, numPulseValues);
    status |= H5LTread_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSEAMPLITUDES, pulse->pRawPulseAmplitudes );
    status |= H5LTread_dataset_float(handle, IUS_INPUTFILE_PATH_PULSE_RAWPULSETIMES, pulse->pRawPulseTimes );

	if( status < 0 )
      return NULL;
    return pulse;
}

