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
#include <iusPulse.h>
#include <iusPulseImp.h>
#include <iusParametricPulseImp.h>
#include <iusNonParametricPulseImp.h>



#define PULSETYPEFMT "%s/pulseType"
#define LABELFMT "%s/pulseLabel"
#define TOSTR(x)    #x


iup_t  iusPulseCreate
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
    if( strcmp(reference->label,actual->label) != 0 )
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

char *iusPulseGetLabel
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



int iusBasePulseSave
(
    iup_t pulse,
    char *parentPath,
    hid_t handle
)
{
    int status=IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, PULSETYPEFMT, parentPath);
    status |= iusWritePulseType(group_id, path, pulse->type);
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(group_id, path, pulse->label, 1);
    status |= H5Gclose(group_id );
    return status;
}

// serialization
int iusPulseSave
(
    iup_t pulse,
    char *parentPath,
    hid_t handle
)
{
    int status=IUS_ERR_VALUE;

    // Make dataset for Experiment
    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
        status = iusParametricPulseSave((iupp_t)pulse, parentPath, handle);

    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
        status = iusNonParametricPulseSave((iunpp_t)pulse, parentPath, handle);

    return status;
}

iup_t iusBasePulseLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status = 0;
    IusPulseType type;
    const char *label;
    char path[IUS_MAX_HDF5_PATH];

    sprintf(path, PULSETYPEFMT, parentPath);
    status |= iusReadPulseType( handle, path, &(type));
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5ReadString( handle, path, &(label));
    if( status < 0 )
        return NULL;

    return iusPulseCreate(type, label);
}

iup_t iusPulseLoad
(
    hid_t handle,
    char *parentPath
)
{
    iup_t pulse=NULL;

    pulse = iusBasePulseLoad(handle,parentPath);
    if( pulse->type == IUS_PARAMETRIC_PULSETYPE )
          pulse = (iup_t) iusParametricPulseLoad(handle, parentPath, pulse->label);
    if( pulse->type == IUS_NON_PARAMETRIC_PULSETYPE )
          pulse = (iup_t) iusNonParametricPulseLoad(handle, parentPath, pulse->label);
    return pulse;
}
