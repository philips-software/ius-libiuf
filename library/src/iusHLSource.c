
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
#include <include/iusHL3DNonParametricSource.h>
#include <include/iusHL2DParametricSource.h>
#include <include/iusHL2DNonParametricSource.h>
#include <include/iusHDF5.h>
#include <hdf5_hl.h>
#include <include/iusHL2DNonParametricSourceImp.h>
#include <include/iusHL2DParametricSourceImp.h>
#include <include/iusHL3DNonParametricSourceImp.h>
#include <include/iusHL3DParametricSourceImp.h>

#include "include/iusHLSourceImp.h"
#include "include/iusHL3DParametricSource.h"


// ADT
int iusHLSourceDelete
(
    ius_t iusSource
)
{
    int status = IUS_ERR_VALUE;
    if(iusSource != NULL)
    {
        free(iusSource);
        iusSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


IUS_BOOL iusHLBaseSourceCompare
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

IUS_BOOL iusHLSourceCompare
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
        return iusHL3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
    switch (reference->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return iusHL2DNonParametricSourceCompare((iu2dnps_t) reference, (iu2dnps_t)actual);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return iusHL2DParametricSourceCompare((iu2dps_t) reference, (iu2dps_t)actual);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return iusHL3DNonParametricSourceCompare((iu3dnps_t) reference, (iu3dnps_t)actual);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return iusHL3DParametricSourceCompare((iu3dps_t) reference, (iu3dps_t)actual);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }

  return IUS_FALSE;
}


// Getters
IusSourceType iusHLSourceGetType
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

char * iusHLSourceGetLabel
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


int iusHLBaseSourceSave
(
    ius_t source,
    hid_t handle
)
{
    int status=IUS_E_OK;
    //char path[IUS_MAX_HDF5_PATH];

    if( source == IUS_INVALID )
    {
        return IUS_ERR_VALUE;
    }

    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, SOURCETYPEFMT, parentPath);
    status |= iusWriteSourceType(handle, "SourceType", source->type);
    //sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(handle, "SourceLabel", source->label, 1);
    //status |= H5Gclose(group_id );
    return status;
}

static ius_t iusHLSourceCreate
(
    IusSourceType type,
    const char *label
)
{
    ius_t source = (ius_t) calloc (1,sizeof(ius_t));
    if(source == NULL) return IUS_INVALID;
    source->label = strdup(label);
    source->type = type;
    return source;
}


ius_t iusHLBaseSourceLoad
(
    hid_t handle
)
{
    int status = 0;
    IusSourceType type;
    const char *label;
    //char path[IUS_MAX_HDF5_PATH];

    //sprintf(path, SOURCETYPEFMT, parentPath);
    status |= iusReadSourceType( handle, "SourceType", &(type));
    //sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5ReadString( handle, "SourceLabel", &(label));  //todo find out when+how this string is freed
    if( status < 0 )
        return NULL;

    return iusHLSourceCreate(type, label);
}

ius_t iusHLSourceLoad
(
    hid_t handle,
	char *sourceLabel
)
{
    ius_t source=NULL;
	hid_t sources_id = H5Gopen(handle, "Sources", H5P_DEFAULT);
	hid_t source_id = H5Gopen(sources_id, sourceLabel, H5P_DEFAULT);
    source = iusHLBaseSourceLoad(source_id);
    switch(source->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iusHL2DNonParametricSourceLoad(source_id, source->label);
            break;
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iusHL2DParametricSourceLoad(source_id, source->label);
            break;
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iusHL3DNonParametricSourceLoad(source_id, source->label);
            break;
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            source = (ius_t) iusHL3DParametricSourceLoad(source_id, source->label);
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



int iusHLSourceSave
(
    ius_t source,
    hid_t handle
)
{
    if( source == IUS_INVALID )
    {
        return IUS_ERR_VALUE;
    }

	hid_t sources_id;
	int status = H5Gget_objinfo(handle, "Sources", 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		sources_id = H5Gcreate(handle, "Sources", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		sources_id = H5Gopen(handle, "Sources", H5P_DEFAULT);
	}
	hid_t label_id = H5Gcreate(sources_id, source->label, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    switch (source->type)
    {
        case IUS_2D_NON_PARAMETRIC_SOURCE:
        {
            return iusHL2DNonParametricSourceSave((iu2dnps_t) source, label_id);
        }
        case IUS_2D_PARAMETRIC_SOURCE:
        {
            return iusHL2DParametricSourceSave((iu2dps_t) source, label_id);
        }
        case IUS_3D_NON_PARAMETRIC_SOURCE:
        {
            return iusHL3DNonParametricSourceSave((iu3dnps_t) source, label_id);
        }
        case IUS_3D_PARAMETRIC_SOURCE:
        {
            return iusHL3DParametricSourceSave((iu3dps_t) source, label_id);
        }
        case IUS_INVALID_SOURCE_TYPE:
            break;
    }
    return IUS_ERR_VALUE;
}
