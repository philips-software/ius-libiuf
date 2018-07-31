
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

#include "include/iusHLSourceImp.h"
#include "include/iusHL3DParametricSource.h"


// ADT
ius_t iusHLSourceCreate
(
    IusSourceType sourceType,
    char *pSourceLabel,
    int numLocations,
    ...
)
{
    va_list ap;
    ius_t source = IUS_INVALID;
    va_start(ap,numLocations);
    if (sourceType == IUS_2D_PARAMETRIC_SOURCE)
    {
        float fNumber = va_arg(ap,double);
        float angularDelta = va_arg(ap,double);
        float startAngle = va_arg(ap,double);
        iu2dps_t obj = iusHL2DParametricSourceCreate(pSourceLabel,
                                                     numLocations,
                                                     fNumber,
                                                     angularDelta,
                                                     startAngle);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_3D_PARAMETRIC_SOURCE)
    {
        float fNumber = va_arg(ap,double);
        float angularDelta = va_arg(ap,double);
        float startAngle = va_arg(ap,double);
        float deltaPhi = va_arg(ap,double);
        float startPhi = va_arg(ap,double);
        iu3dps_t obj = iusHL3DParametricSourceCreate(pSourceLabel,
                                                    numLocations,
                                                    fNumber,
                                                    angularDelta,
                                                    startAngle,
                                                    deltaPhi,
                                                    startPhi);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_2D_NON_PARAMETRIC_SOURCE)
    {
        iu2dnps_t obj = iusHL2DNonParametricSourceCreate(pSourceLabel,numLocations);
        source = (ius_t) obj;
    }
    if (sourceType == IUS_3D_NON_PARAMETRIC_SOURCE)
    {
        iu3dnps_t obj = iusHL3DNonParametricSourceCreate(pSourceLabel,numLocations);
        source = (ius_t) obj;
    }
    va_end(ap);
    return source;
}


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
    char *parentPath,
    hid_t handle
)
{
    int status=IUS_E_OK;
    char path[64];

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, SOURCETYPEFMT, parentPath);
    status |= iusWriteSourceType(group_id, path, source->type);
    sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(group_id, path, source->label, 1);
    status |= H5Gclose(group_id );
    return status;
}

