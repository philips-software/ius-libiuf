//
// Created by Ruijzendaal on 30/03/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <iusPulsePrivate.h>
#include <iusParametricPulsePrivate.h>
#include <iusNonParametricPulsePrivate.h>

#define TOSTR(x)    #x


iup_t  iusPulseCreate
(
    IusPulseType type
)
{
    iup_t transmitPulse;

    if( type != IUS_PARAMETRIC_PULSETYPE &&
        type != IUS_NON_PARAMETRIC_PULSETYPE )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", type);
        return IUP_INVALID;

    }

    transmitPulse = calloc(1, sizeof(struct IusPulse));
    if(transmitPulse == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for IusPulse");
        return IUP_INVALID;
    }

    transmitPulse->type = type;
    return transmitPulse;
}

int iusPulseDelete
(
    iup_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        return iusNonParametricPulseDelete((iunpp_t) pulse);
    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        return iusParametricPulseDelete((iupp_t) pulse);
    return IUS_ERR_VALUE;
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
    IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", reference->type);
    return IUS_FALSE;
}

// getters
IusPulseType iusPulseGetType
(
    iup_t pulse
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_INVALID_PULSETYPE);
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
    if( pulse == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "pulse argument is NULL");
        return IUS_INVALID_PULSETYPE;
    }

    status |= iusWritePulseType(handle, IUS_INPUTFILE_PATH_PULSE_PULSETYPE, pulse->type);
    if (status != IUS_E_OK)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during save of %s",
                           IUS_INPUTFILE_PATH_PULSE_PULSETYPE);
    }
    return status;
}

// serialization
int iusPulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pulse, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        return iusParametricPulseSave((iupp_t)pulse, handle);

    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        return iusNonParametricPulseSave((iunpp_t)pulse, handle);

    IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", pulse->type);
    return IUS_ERR_VALUE;
}

iup_t iusBasePulseLoad
(
	hid_t handle
)
{
	int status = IUS_E_OK;
	IusPulseType type;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUP_INVALID);
	status |= iusReadPulseType(handle, IUS_INPUTFILE_PATH_PULSE_PULSETYPE, &(type));
    if (status != IUS_E_OK)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during load of %s",
                           IUS_INPUTFILE_PATH_PULSE_PULSETYPE);
    }
	return iusPulseCreate(type);
}

iup_t iusPulseLoad
(
	hid_t handle
)
{
    int status = IUS_E_OK;
    IusPulseType type;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUP_INVALID);
    status |= iusReadPulseType(handle, IUS_INPUTFILE_PATH_PULSE_PULSETYPE, &(type));
    if (status != IUS_E_OK)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during load of %s",
                           IUS_INPUTFILE_PATH_PULSE_PULSETYPE);
        return IUP_INVALID;
    }

    if( type == IUS_PARAMETRIC_PULSETYPE )
        return (iup_t) iusParametricPulseLoad(handle);
    if( type == IUS_NON_PARAMETRIC_PULSETYPE )
        return (iup_t) iusNonParametricPulseLoad(handle);
    IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", type);
	return IUP_INVALID;
}
