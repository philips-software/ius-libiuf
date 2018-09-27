
//
// Created by nlv09165 on 24/07/2018.
//
#include <stdlib.h>
#include <stdarg.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <string.h>
#include <include/ius3DNonParametricSource.h>
#include <include/ius2DParametricSource.h>
#include <include/ius2DNonParametricSource.h>
#include <include/iusHDF5.h>
#include <hdf5_hl.h>
#include <include/ius2DNonParametricSourceImp.h>
#include <include/ius2DParametricSourceImp.h>
#include <include/ius3DNonParametricSourceImp.h>
#include <include/ius3DParametricSourceImp.h>

#include "include/iusSourceImp.h"
#include "include/ius3DParametricSource.h"


// ADT
int iusSourceDelete
(
    ius_t iusSource
)
{
    int status = IUS_ERR_VALUE;
    if( iusSource == IUS_INVALID ) return status;
    switch (iusSource->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return ius2DNonParametricSourceDelete((iu2dnps_t) iusSource);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return ius2DParametricSourceDelete((iu2dps_t) iusSource);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return ius3DNonParametricSourceDelete((iu3dnps_t) iusSource);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return ius3DParametricSourceDelete((iu3dps_t) iusSource);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }
    free(iusSource->label);
    free(iusSource);
    return status;
}


IUS_BOOL iusBaseSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
    if ( reference == actual ) return IUS_TRUE;
    if ( reference == NULL || actual == NULL ) return IUS_FALSE;
    if ( reference->type != actual->type ) return IUS_FALSE;
    if ( strcmp(reference->label,actual->label) != 0 ) return IUS_FALSE;
    return IUS_TRUE;
}

IUS_BOOL iusSourceCompare
(
  ius_t reference,
  ius_t actual
)
{
    if( reference == actual )
        return IUS_TRUE;
    if( reference == NULL || actual == NULL )
        return IUS_FALSE;
    if( reference->type == IUS_3D_PARAMETRIC_SOURCE )
        return ius3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
    switch (reference->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return ius2DNonParametricSourceCompare((iu2dnps_t) reference, (iu2dnps_t)actual);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return ius2DParametricSourceCompare((iu2dps_t) reference, (iu2dps_t)actual);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return ius3DNonParametricSourceCompare((iu3dnps_t) reference, (iu3dnps_t)actual);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return ius3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }

  return IUS_FALSE;
}


// Getters
IusSourceType iusSourceGetType
(
    ius_t iusSource
)
{
    if( iusSource == IUS_INVALID )
    {
        return IUS_INVALID_SOURCE_TYPE;
    }
    return iusSource->type;
}

char * iusSourceGetLabel
(
    ius_t iusSource
)
{
    if( iusSource == IUS_INVALID )
    {
        return NULL;
    }
    return iusSource->label;
}

// Serialization
#define SOURCETYPEFMT "%s/SourceType"
#define LABELFMT "%s/SourceLabel"
#define TOSTR(x)    #x

static herr_t iusWriteSourceType
(
    hid_t group_id,
    char *pVariableString,
    IusSourceType type
)
{
    herr_t status=0;
    hsize_t dims[1] = {1};
    /* Based on a native signed short */
    hid_t hdf_sourceType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_INVALID_SOURCE_TYPE) ,
                              (enumValue=IUS_INVALID_SOURCE_TYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_2D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUS_2D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_3D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUS_3D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_2D_PARAMETRIC_SOURCE),
                              (enumValue=IUS_2D_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_3D_PARAMETRIC_SOURCE),
                              (enumValue=IUS_3D_PARAMETRIC_SOURCE, &enumValue) );

    enumValue = type;
    status |= H5LTmake_dataset( group_id, pVariableString, 1, dims, hdf_sourceType, &enumValue );
    return status;
}


static int iusReadSourceType
(
    hid_t handle,
    char *pVariableString,
    IusSourceType *pType
)
{
    herr_t status = 0;
    /* Based on a native signed short */
    hid_t hdf_sourceType = H5Tcreate( H5T_ENUM, sizeof(short) );
    short enumValue;
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_INVALID_SOURCE_TYPE) ,
                              (enumValue=IUS_INVALID_SOURCE_TYPE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_2D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUS_2D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_3D_NON_PARAMETRIC_SOURCE),
                              (enumValue=IUS_3D_NON_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_2D_PARAMETRIC_SOURCE),
                              (enumValue=IUS_2D_PARAMETRIC_SOURCE, &enumValue) );
    status |= H5Tenum_insert( hdf_sourceType, TOSTR(IUS_3D_PARAMETRIC_SOURCE),
                              (enumValue=IUS_3D_PARAMETRIC_SOURCE, &enumValue) );

    *pType = 0;
    status |= H5LTread_dataset( handle, pVariableString , hdf_sourceType, pType );
    return status;
}


int iusBaseSourceSave
(
    ius_t source,
    char *parentPath,
    hid_t handle
)
{
    int status=IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];

    if( source == IUS_INVALID )
    {
        return IUS_ERR_VALUE;
    }

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, SOURCETYPEFMT, parentPath);
    status |= iusWriteSourceType(group_id, path, source->type);
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(group_id, path, source->label);
    status |= H5Gclose(group_id );
    return status;
}

static ius_t iusSourceCreate
(
    IusSourceType type,
    const char *label
)
{
    ius_t source = (ius_t) calloc (1,sizeof(IusSource));
    if(source == NULL) return IUS_INVALID;
    source->label = strdup(label);
    source->type = type;
    return source;
}


ius_t iusBaseSourceLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status = 0;
    IusSourceType type;
    char label[256];
    char path[IUS_MAX_HDF5_PATH];

    sprintf(path, SOURCETYPEFMT, parentPath);
    status |= iusReadSourceType( handle, path, &(type));
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5ReadString( handle, path, label);
    if( status < 0 )
        return NULL;

    return iusSourceCreate(type, label);
}

ius_t iusSourceLoad
(
    hid_t handle,
    char *parentPath
)
{
    IusSourceType type;
    char label[256];
    char path[IUS_MAX_HDF5_PATH];

    sprintf(path, SOURCETYPEFMT, parentPath);
    int status = iusReadSourceType( handle, path, &(type));
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5ReadString( handle, path, label);

    if( status < 0 )
        return IUS_INVALID;

    ius_t source = NULL;
    switch (type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius2DNonParametricSourceLoad(handle,parentPath,label);
            break;
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius2DParametricSourceLoad(handle,parentPath,label);
            break;
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius3DNonParametricSourceLoad(handle,parentPath,label);
            break;
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius3DParametricSourceLoad(handle,parentPath,label);
            break;
        }
        case IUS_INVALID_SOURCE_TYPE:
        {
            source = IUS_INVALID;
            break;
        }
    }
    return source;
}



int iusSourceSave
(
    ius_t source,
    char *parentPath,
    hid_t handle
)
{
    if( source == IUS_INVALID )
    {
        return IUS_ERR_VALUE;
    }
    switch (source->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return ius2DNonParametricSourceSave((iu2dnps_t) source,parentPath,handle);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return ius2DParametricSourceSave((iu2dps_t) source,parentPath,handle);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return ius3DNonParametricSourceSave((iu3dnps_t) source,parentPath,handle);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return ius3DParametricSourceSave((iu3dps_t) source,parentPath,handle);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }
    return IUS_ERR_VALUE;
}
