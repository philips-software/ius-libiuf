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


#define IUS_NONPARAMPULSE_NUMPULSEVALUES  "numPulseValues"
#define IUS_NONPARAMPULSE_PULSEAMPLITUDES "rawPulseAmplitudes"
#define IUS_NONPARAMPULSE_PULSETIMES      "rawPulseTimes"

int iusHLNonParametricPulseSave
(
    iunpp_t pulse,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    if(pulse == NULL || iusHLPulseGetType( (iup_t)pulse ) != IUS_NON_PARAMETRIC_PULSETYPE)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	//TODO chekc if /Pulses exist
	hid_t pulses_id;
	status = H5Gget_objinfo(handle, "Pulses", 0, NULL); // todo centralize the path
	if (status != 0) // the group does not exist yet
	{
		pulses_id = H5Gcreate(handle, "Pulses", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		pulses_id = H5Gopen(handle, "Pulses", H5P_DEFAULT);
	}
	hid_t label_id = H5Gcreate(pulses_id, pulse->base.label, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = iusHLBasePulseSave((iup_t)pulse, label_id);
    //sprintf(path, IUS_NONPARAMPULSE_NUMPULSEVALUESFMT, parentPath);
    status |= iusHdf5WriteInt(label_id, IUS_NONPARAMPULSE_NUMPULSEVALUES, &(pulse->numPulseValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = pulse->numPulseValues;
    //sprintf(path, IUS_NONPARAMPULSE_PULSEAMPLITUDES, parentPath);
    status |= H5LTmake_dataset_float(label_id, IUS_NONPARAMPULSE_PULSEAMPLITUDES, 1, dims, pulse->pRawPulseAmplitudes );
    //sprintf(path, PULSETIMESFMT, parentPath);
    status |= H5LTmake_dataset_float(label_id, IUS_NONPARAMPULSE_PULSETIMES, 1, dims, pulse->pRawPulseTimes );
	H5Gclose(label_id);
	H5Gclose(pulses_id);
    return status;
}

iunpp_t iusHLNonParametricPulseLoad
(
    hid_t handle
)
{
    int status = 0;
    //char path[IUS_MAX_HDF5_PATH];
    int  numPulseValues;
	char *label;
    iunpp_t  pulse;

    if (handle == H5I_INVALID_HID)
        return NULL;

	//sprintf(path, "Pulses/%s", label);
	//hid_t pulse_id = H5Gopen(handle, path, H5P_DEFAULT);
    //sprintf(path, NUMPULSEVALUESFMT, parentPath);
    status |= iusHdf5ReadInt(handle, IUS_NONPARAMPULSE_NUMPULSEVALUES, &(numPulseValues));
	status |= iusHdf5ReadString(handle, IUS_NONPARAMPULSE_NUMPULSEVALUES, &(label));
    if( status < 0 )
        return NULL;

    pulse = iusHLNonParametricPulseCreate(label, numPulseValues);
    //sprintf(path, PULSEAMPLITUDESFMT, parentPath);
    status |= H5LTread_dataset_float(handle, IUS_NONPARAMPULSE_PULSEAMPLITUDES, pulse->pRawPulseAmplitudes );
    //sprintf(path, PULSETIMESFMT, parentPath);
    status |= H5LTread_dataset_float(handle, IUS_NONPARAMPULSE_PULSETIMES,      pulse->pRawPulseTimes );

	//H5Gclose(pulse_id);
	if( status < 0 )
      return NULL;
    return pulse;
}

