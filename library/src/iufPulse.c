#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iufPulsePrivate.h>
#include <iufParametricPulsePrivate.h>
#include <iufNonParametricPulsePrivate.h>

#define TOSTR(x)    #x


iup_t  iufPulseCreate
(
    IufPulseType type
)
{
    iup_t transmitPulse;

    if( type != IUF_PARAMETRIC_PULSETYPE &&
        type != IUF_NON_PARAMETRIC_PULSETYPE )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", type);
        return IUP_INVALID;

    }

    transmitPulse = calloc(1, sizeof(struct IufPulse));
    if(transmitPulse == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for IufPulse");
        return IUP_INVALID;
    }

    transmitPulse->type = type;
    return transmitPulse;
}

int iufPulseDelete
(
    iup_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    if( pulse->type == IUF_NON_PARAMETRIC_PULSETYPE )
        return iufNonParametricPulseDelete((iunpp_t) pulse);
    if( pulse->type == IUF_PARAMETRIC_PULSETYPE )
        return iufParametricPulseDelete((iupp_t) pulse);
    return IUF_ERR_VALUE;
}

IUF_BOOL iufBasePulseCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual )
        return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;
    if( reference->type != actual->type )
        return IUF_FALSE;
    return IUF_TRUE;
}

IUF_BOOL iufPulseCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual )
        return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;
    if( reference->type == IUF_NON_PARAMETRIC_PULSETYPE )
        return iufNonParametricPulseCompare((iunpp_t) reference, (iunpp_t)actual);
    if( reference->type == IUF_PARAMETRIC_PULSETYPE )
        return iufParametricPulseCompare((iupp_t) reference, (iupp_t)actual);
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", reference->type);
    return IUF_FALSE;
}

// getters
IufPulseType iufPulseGetType
(
    iup_t pulse
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_INVALID_PULSETYPE);
    return pulse->type;
}

static herr_t iufWritePulseType
(
    hid_t group_id,
    char *pVariableString,
    IufPulseType type
)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_pulseType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_INVALID_PULSETYPE) ,
                              (enumValue=IUF_INVALID_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_PARAMETRIC_PULSETYPE),
                              (enumValue=IUF_PARAMETRIC_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_NON_PARAMETRIC_PULSETYPE),
                              (enumValue=IUF_NON_PARAMETRIC_PULSETYPE, &enumValue) );

    enumValue = type;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_pulseType, &enumValue );
    return status;
}


static int iufReadPulseType
(
    hid_t handle,
    char *pVariableString,
    IufPulseType *pType
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_pulseType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_INVALID_PULSETYPE) ,
                              (enumValue=IUF_INVALID_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_PARAMETRIC_PULSETYPE),
                              (enumValue=IUF_PARAMETRIC_PULSETYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_pulseType, TOSTR(IUF_NON_PARAMETRIC_PULSETYPE),
                              (enumValue=IUF_NON_PARAMETRIC_PULSETYPE, &enumValue) );
    *pType = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_pulseType, pType );
    return status;
}



int iufBasePulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    int status=IUF_E_OK;
    if( pulse == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "pulse argument is NULL");
        return IUF_INVALID_PULSETYPE;
    }

    status |= iufWritePulseType(handle, IUF_INPUTFILE_PATH_PULSE_PULSETYPE, pulse->type);
    if (status != IUF_E_OK)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during save of %s",
                           IUF_INPUTFILE_PATH_PULSE_PULSETYPE);
    }
    return status;
}

// serialization
int iufPulseSave
(
    iup_t pulse,
    hid_t handle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pulse, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    if( pulse->type == IUF_PARAMETRIC_PULSETYPE )
        return iufParametricPulseSave((iupp_t)pulse, handle);

    if( pulse->type == IUF_NON_PARAMETRIC_PULSETYPE )
        return iufNonParametricPulseSave((iunpp_t)pulse, handle);

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", pulse->type);
    return IUF_ERR_VALUE;
}

iup_t iufBasePulseLoad
(
	hid_t handle
)
{
	int status = IUF_E_OK;
	IufPulseType type;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUP_INVALID);
	status |= iufReadPulseType(handle, IUF_INPUTFILE_PATH_PULSE_PULSETYPE, &(type));
    if (status != IUF_E_OK)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during load of %s",
                           IUF_INPUTFILE_PATH_PULSE_PULSETYPE);
    }
	return iufPulseCreate(type);
}

iup_t iufPulseLoad
(
	hid_t handle
)
{
    int status = IUF_E_OK;
    IufPulseType type;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUP_INVALID);
    status |= iufReadPulseType(handle, IUF_INPUTFILE_PATH_PULSE_PULSETYPE, &(type));
    if (status != IUF_E_OK)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during load of %s",
                           IUF_INPUTFILE_PATH_PULSE_PULSETYPE);
        return IUP_INVALID;
    }

    if( type == IUF_PARAMETRIC_PULSETYPE )
        return (iup_t) iufParametricPulseLoad(handle);
    if( type == IUF_NON_PARAMETRIC_PULSETYPE )
        return (iup_t) iufNonParametricPulseLoad(handle);
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported pulse type: '%d'", type);
	return IUP_INVALID;
}
