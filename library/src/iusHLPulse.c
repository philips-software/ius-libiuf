//
// Created by Ruijzendaal on 30/03/2018.
//
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <hdf5.h>
#include <hdf5_hl.h>

#include <ius.h>
#include <iusHDF5.h>
#include <iusUtil.h>
#include <iusError.h>
#include <iusHLPulse.h>
#include <iusHLPulseImp.h>
#include <iusHLParametricPulseImp.h>
#include <iusHLNonParametricPulseImp.h>



#define PULSETYPE_STR "pulseType"
#define PULSELABEL_STR "pulseLabel"
#define TOSTR(x)    #x


iup_t  iusHLPulseCreate
(
    IusPulseType type,
    const char *label
)
{
    iup_t transmitPulse = IUP_INVALID;
    if( label == NULL ) return transmitPulse;

    if( type != IUS_PARAMETRIC_PULSETYPE &&
        type != IUS_NON_PARAMETRIC_PULSETYPE )
        return transmitPulse;

    transmitPulse = calloc(1, sizeof(struct IusPulse));
    transmitPulse->type = type;
    transmitPulse->label = strdup(label);
    return transmitPulse;
}

int iusHLPulseDelete
(
    iup_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    free(pulse);
    return IUS_E_OK;
}

IUS_BOOL iusHLBasePulseCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->type != actual->type )
        return IUS_FALSE;
    if( strcmp(reference->label,actual->label) != 0 )
        return IUS_FALSE;
    return IUS_TRUE;
}

IUS_BOOL iusHLPulseCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->type == IUS_NON_PARAMETRIC_PULSETYPE )
        return iusHLNonParametricPulseCompare((iunpp_t) reference, (iunpp_t)actual);
    if( reference->type == IUS_PARAMETRIC_PULSETYPE )
        return iusHLParametricPulseCompare((iupp_t) reference, (iupp_t)actual);
    return IUS_FALSE;
}

// getters
IusPulseType iusHLPulseGetType
(
    iup_t pulse
)
{
    if( pulse == NULL ) return  IUS_INVALID_PULSETYPE;
    return pulse->type;
}

char *iusHLPulseGetLabel
(
    iup_t pulse
)
{
    if( pulse == NULL ) return  NULL;
    return pulse->label;
}

static herr_t iusWritePulseType
(
    hid_t group_id,
    char *pVariableString,
    IusPulseType type
)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_pulseType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_INVALID_PULSETYPE) ,
                              (enumValue=IUS_INVALID_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_PARAMETRIC_PULSETYPE),
                              (enumValue=IUS_PARAMETRIC_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_NON_PARAMETRIC_PULSETYPE),
                              (enumValue=IUS_NON_PARAMETRIC_PULSETYPE, &enumValue) );

    enumValue = type;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_pulseType, &enumValue );
    return status;
}


static int iusReadPulseType
(
    hid_t handle,
    char *pVariableString,
    IusPulseType *pType
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_pulseType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_INVALID_PULSETYPE) ,
                              (enumValue=IUS_INVALID_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_PARAMETRIC_PULSETYPE),
                              (enumValue=IUS_PARAMETRIC_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUS_NON_PARAMETRIC_PULSETYPE),
                              (enumValue=IUS_NON_PARAMETRIC_PULSETYPE, &enumValue) );
    *pType = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_pulseType, pType );
    return status;
}



int iusHLBasePulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    int status=IUS_E_OK;
    //char path[IUS_MAX_HDF5_PATH];

    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, PULSETYPEFMT, parentPath);
    status |= iusWritePulseType(handle, PULSETYPE_STR, pulse->type);
    //sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(handle, PULSELABEL_STR, pulse->label, 1);
    //status |= H5Gclose(group_id );
    return status;
}

// serialization
int iusHLPulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    int status=IUS_ERR_VALUE;
    // create a pulse group in "Pulses" and create Pulses if not existing
	hid_t pulses_id;
	status = H5Gget_objinfo(handle, "Pulses", 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		pulses_id = H5Gcreate(handle, "Pulse", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		pulses_id = H5Gopen(handle, "Pulses", H5P_DEFAULT);
	}

	hid_t pulse_id = H5Gcreate(pulses_id, pulse->label, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    // Make dataset for Experiment
    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        status = iusHLParametricPulseSave((iupp_t)pulse, pulse_id);

    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        status = iusHLNonParametricPulseSave((iunpp_t)pulse, pulse_id);

	H5Gclose(pulse_id);
	H5Gclose(pulses_id);

    return status;
}

iup_t iusHLBasePulseLoad
(
	hid_t handle
)
{
	int status = IUS_E_OK;
	IusPulseType type;
	const char *label;

	status |= iusReadPulseType(handle, PULSETYPE_STR, &(type));
	status |= iusHdf5ReadString(handle, PULSELABEL_STR, &(label));
	if (status < 0)
		return NULL;

	return iusHLPulseCreate(type, label);
}

iup_t iusHLPulseLoad
(
	hid_t handle,
	char *label
)
{
	iup_t pulse = NULL;
	char path[IUS_MAX_HDF5_PATH];

	sprintf(path, "%s/%s", "Pulses/%s", label);
	hid_t pulse_id = H5Gopen(handle, path, H5P_DEFAULT);

	pulse = iusHLBasePulseLoad(pulse_id);
	if (pulse == NULL) return IUP_INVALID;

	switch (pulse->type)
	{
	case IUS_PARAMETRIC_PULSETYPE:
		pulse = (iup_t)iusHLParametricPulseLoad(pulse_id); // TODO: decide if it is okay that the base pulse object is discarded during load? Is it causing a memory leak?
	case IUS_NON_PARAMETRIC_PULSETYPE:
		pulse = (iup_t)iusHLNonParametricPulseLoad(pulse_id); // TODO: decide if it is okay that the base pulse object is discarded during load? Is it causing a memory leak?
	}
	return pulse;
}
