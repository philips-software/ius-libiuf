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
#include <iusInputFileStructure.h>
#include <iusPulse.h>
#include <iusPulsePrivate.h>
#include <iusParametricPulsePrivate.h>
#include <iusNonParametricPulsePrivate.h>


#define TOSTR(x)    #x


iup_t  iusPulseCreate
(
    IusPulseType type
)
{
    iup_t transmitPulse = IUP_INVALID;

    if( type != IUS_PARAMETRIC_PULSETYPE &&
        type != IUS_NON_PARAMETRIC_PULSETYPE )
        return transmitPulse;

    transmitPulse = calloc(1, sizeof(struct IusPulse));
    transmitPulse->type = type;
    return transmitPulse;
}

int iusPulseDelete
(
    iup_t pulse
)
{
    if( pulse == NULL ) return IUS_ERR_VALUE;
    free(pulse);
    return IUS_E_OK;
}

IUS_BOOL iusBasePulseCompare
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
    return IUS_TRUE;
}

IUS_BOOL iusPulseCompare
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
        return iusNonParametricPulseCompare((iunpp_t) reference, (iunpp_t)actual);
    if( reference->type == IUS_PARAMETRIC_PULSETYPE )
        return iusParametricPulseCompare((iupp_t) reference, (iupp_t)actual);
    return IUS_FALSE;
}

// getters
IusPulseType iusPulseGetType
(
    iup_t pulse
)
{
    if( pulse == NULL ) return  IUS_INVALID_PULSETYPE;
    return pulse->type;
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



int iusBasePulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    int status=IUS_E_OK;

	status |= iusWritePulseType(handle, IUS_INPUTFILE_PATH_PULSE_PULSETYPE, pulse->type);
    return status;
}

// serialization
int iusPulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    int status=IUS_ERR_VALUE;

    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        status = iusParametricPulseSave((iupp_t)pulse, handle);

    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        status = iusNonParametricPulseSave((iunpp_t)pulse, handle);

    return status;
}

iup_t iusBasePulseLoad
(
	hid_t handle
)
{
	int status = IUS_E_OK;
	IusPulseType type;

	status |= iusReadPulseType(handle, IUS_INPUTFILE_PATH_PULSE_PULSETYPE, &(type));
	if (status < 0)
		return NULL;

	return iusPulseCreate(type);
}

iup_t iusPulseLoad
(
	hid_t handle
)
{
	iup_t pulse = NULL;
	pulse = iusBasePulseLoad(handle);
	if (pulse == NULL) return IUP_INVALID;
	//todo free pulse here to prevent memory loss?

    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        pulse = (iup_t) iusParametricPulseLoad(handle);
    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        pulse = (iup_t) iusNonParametricPulseLoad(handle);

	return pulse;
}
