
//
// Created by nlv09165 on 24/07/2018.
//
#include <stdlib.h>
#include <string.h>

#include <ius.h>
#include <ius2DNonParametricSourcePrivate.h>
#include <ius2DParametricSourcePrivate.h>
#include <ius3DNonParametricSourcePrivate.h>
#include <ius3DParametricSourcePrivate.h>
#include <iusSourcePrivate.h>


// ADT
int iusSourceDelete
(
    ius_t source
)
{
    if( source == IUS_INVALID) return IUS_ERR_VALUE;
    switch (source->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return ius2DNonParametricSourceDelete((iu2dnps_t) source);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return ius2DParametricSourceDelete((iu2dps_t) source);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return ius3DNonParametricSourceDelete((iu3dnps_t) source);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return ius3DParametricSourceDelete((iu3dps_t) source);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }
    return IUS_ERR_VALUE;
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

// Serialization
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
    hid_t handle
)
{
    int status=IUS_E_OK;

    if( source == IUS_INVALID )
    {
        return IUS_ERR_VALUE;
    }
    status |= iusWriteSourceType(handle, IUS_INPUTFILE_PATH_SOURCE_SOURCETYPE, source->type);
    return status;
}

static ius_t iusSourceCreate
(
    IusSourceType type
)
{
    ius_t source = (ius_t) calloc (1,sizeof(ius_t));
    if(source == NULL) return IUS_INVALID;
    source->type = type;
    return source;
}


ius_t iusBaseSourceLoad
(
    hid_t handle
)
{
    int status = 0;
    IusSourceType type;
    
    status = iusReadSourceType( handle, IUS_INPUTFILE_PATH_SOURCE_SOURCETYPE, &(type));
    if( status < 0 )
        return NULL;

    return iusSourceCreate(type);
}

ius_t iusSourceLoad
(
    hid_t handle
)
{
	ius_t base;
    ius_t source = IUS_INVALID;

    base = iusBaseSourceLoad(handle);
	if (base == NULL) return IUS_INVALID;

    switch(base->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius2DNonParametricSourceLoad(handle);
            break;
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius2DParametricSourceLoad(handle);
            break;
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius3DNonParametricSourceLoad(handle);
            break;
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) ius3DParametricSourceLoad(handle);
            break;
        }
        case IUS_INVALID_SOURCE_TYPE:
        {
            source = IUS_INVALID;
            break;
        }
    }
    free(base);
    return source;
}

int iusSourceSave
(
    ius_t source,
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
            return ius2DNonParametricSourceSave((iu2dnps_t) source, handle);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return ius2DParametricSourceSave((iu2dps_t) source, handle);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return ius3DNonParametricSourceSave((iu3dnps_t) source, handle);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return ius3DParametricSourceSave((iu3dps_t) source, handle);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }
    return IUS_ERR_VALUE;
}
