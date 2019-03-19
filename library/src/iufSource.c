
//
// Created by nlv09165 on 24/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <iuf.h>
#include <iuf2DNonParametricSourcePrivate.h>
#include <iuf2DParametricSourcePrivate.h>
#include <iuf3DNonParametricSourcePrivate.h>
#include <iuf3DParametricSourcePrivate.h>
#include <iufSourcePrivate.h>


// ADT
int iufSourceDelete
(
    ius_t source
)
{
    if (source == IUS_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "source argument is NULL");
        return IUF_ERR_VALUE;
    }

    switch (source->type)
    {
        case IUF_2D_NON_PARAMETRIC_SOURCE:
        {
            return iuf2DNonParametricSourceDelete((iu2dnps_t) source);
        }
        case IUF_2D_PARAMETRIC_SOURCE:
        {
            return iuf2DParametricSourceDelete((iu2dps_t) source);
        }
        case IUF_3D_NON_PARAMETRIC_SOURCE:
        {
            return iuf3DNonParametricSourceDelete((iu3dnps_t) source);
        }
        case IUF_3D_PARAMETRIC_SOURCE:
        {
            return iuf3DParametricSourceDelete((iu3dps_t) source);
        }
        case IUF_INVALID_SOURCE_TYPE:
            break;
    }
    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported source type: '%d'", source->type);
    return IUF_ERR_VALUE;
}


IUF_BOOL iufBaseSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
    if ( reference == actual ) return IUF_TRUE;
    if ( reference == NULL || actual == NULL ) return IUF_FALSE;
    if ( reference->type != actual->type ) return IUF_FALSE;
    return IUF_TRUE;
}

IUF_BOOL iufSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
    if( reference == actual )
        return IUF_TRUE;
    if( reference == NULL || actual == NULL )
        return IUF_FALSE;
    switch (reference->type)
    {
        case IUF_2D_NON_PARAMETRIC_SOURCE:
        {
            return iuf2DNonParametricSourceCompare((iu2dnps_t) reference, (iu2dnps_t)actual);
        }
        case IUF_2D_PARAMETRIC_SOURCE:
        {
            return iuf2DParametricSourceCompare((iu2dps_t) reference, (iu2dps_t)actual);
        }
        case IUF_3D_NON_PARAMETRIC_SOURCE:
        {
            return iuf3DNonParametricSourceCompare((iu3dnps_t) reference, (iu3dnps_t)actual);
        }
        case IUF_3D_PARAMETRIC_SOURCE:
        {
            return iuf3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
        }
        case IUF_INVALID_SOURCE_TYPE:
            break;
    }

  return IUF_FALSE;
}


// Getters
IufSourceType iufSourceGetType
(
    ius_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_INVALID_SOURCE_TYPE);
    return source->type;
}

// Serialization
#define TOSTR(x)    #x
static herr_t iufWriteSourceType
(
    hid_t group_id,
    char *pVariableString,
    IufSourceType type
)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_sourceType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_INVALID_SOURCE_TYPE) ,
                              (enumValue=IUF_INVALID_SOURCE_TYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_2D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUF_2D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_3D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUF_3D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_2D_PARAMETRIC_SOURCE),
                              (enumValue=IUF_2D_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_3D_PARAMETRIC_SOURCE),
                              (enumValue=IUF_3D_PARAMETRIC_SOURCE, &enumValue) );

    enumValue = type;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_sourceType, &enumValue );
    return status;
}

static int iufReadSourceType
(
    hid_t handle,
    char *pVariableString,
    IufSourceType *pType
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_sourceType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_INVALID_SOURCE_TYPE) ,
                              (enumValue=IUF_INVALID_SOURCE_TYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_2D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUF_2D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_3D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUF_3D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_2D_PARAMETRIC_SOURCE),
                              (enumValue=IUF_2D_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUF_3D_PARAMETRIC_SOURCE),
                              (enumValue=IUF_3D_PARAMETRIC_SOURCE, &enumValue) );

    *pType = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_sourceType, pType );
    return status;
}


int iufBaseSourceSave
(
    ius_t source,
    hid_t handle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    return iufWriteSourceType(handle, IUF_INPUTFILE_PATH_SOURCE_SOURCETYPE, source->type);
}

static ius_t iufSourceCreate
(
    IufSourceType type
)
{
    ius_t source = (ius_t) calloc (1,sizeof(ius_t));
    IUF_ERR_ALLOC_NULL_N_RETURN(source, IufSource, IUS_INVALID);
    source->type = type;
    return source;
}


ius_t iufBaseSourceLoad
(
    hid_t handle
)
{
    IufSourceType type;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_INVALID);
    int status = iufReadSourceType(handle, IUF_INPUTFILE_PATH_SOURCE_SOURCETYPE, &(type));
    if (status != IUF_E_OK)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during read of %s",
                           IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES);
        return IUS_INVALID;
    }
    return iufSourceCreate(type);
}

ius_t iufSourceLoad
(
    hid_t handle
)
{
	ius_t base;
    ius_t source;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_INVALID);

    base = iufBaseSourceLoad(handle);
	if (base == IUS_INVALID) return IUS_INVALID;

    switch(base->type)
    {
        case IUF_2D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iuf2DNonParametricSourceLoad(handle);
            break;
        }
        case IUF_2D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iuf2DParametricSourceLoad(handle);
            break;
        }
        case IUF_3D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iuf3DNonParametricSourceLoad(handle);
            break;
        }
        case IUF_3D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iuf3DParametricSourceLoad(handle);
            break;
        }

        case IUF_INVALID_SOURCE_TYPE:
        default:
        {
            IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported type loaded: '%d'",
                               base->type);
            source = IUS_INVALID;
            break;
        }
    }
    free(base);
    return source;
}

int iufSourceSave
(
    ius_t source,
    hid_t handle
)
{
    if (source == IUS_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "source argument is NULL");
        return IUF_ERR_VALUE;
    }

    switch (source->type)
    {
        case IUF_2D_NON_PARAMETRIC_SOURCE:
        {
            return iuf2DNonParametricSourceSave((iu2dnps_t) source, handle);
        }
        case IUF_2D_PARAMETRIC_SOURCE:
        {
            return iuf2DParametricSourceSave((iu2dps_t) source, handle);
        }
        case IUF_3D_NON_PARAMETRIC_SOURCE:
        {
            return iuf3DNonParametricSourceSave((iu3dnps_t) source, handle);
        }
        case IUF_3D_PARAMETRIC_SOURCE:
        {
            return iuf3DParametricSourceSave((iu3dps_t) source, handle);
        }

        default:
        case IUF_INVALID_SOURCE_TYPE:
            break;
    }

    IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "unsupported type: '%d'",
                       source->type);
    return IUF_ERR_VALUE;
}
