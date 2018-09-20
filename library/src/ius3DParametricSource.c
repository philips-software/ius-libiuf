
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <include/iusSourceImp.h>
#include <include/iusPositionImp.h>
#include <include/ius3DParametricSource.h>
#include <include/iusHDF5.h>

struct Ius3DParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius3DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< angle in [rad] between sources */
} ;

#define FNUMBERFMT       "%s/fNumber"
#define ANGULARDELTAFMT  "%s/angularDelta"
#define STARTANGLEFMT    "%s/startAngle"
#define DELTAPHIFMT      "%s/deltaPhi"
#define STARTPHIFMT      "%s/startPhi"
#define LOCATIONSFMT     "%s/Locations"

#define LOCATIONFMT     "%s/Location[%d]"
#define LOCATIONSSIZEFMT "%s/Size"



// ADT
iu3dps_t ius3DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle,
    float deltaPhi,
    float startPhi
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu3dps_t created = calloc(1,sizeof(Ius3DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius3DPosition *) calloc(numLocations, sizeof(Ius3DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_3D_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->deltaPhi = deltaPhi;
    created->startPhi = startPhi;
    created->fNumber = fNumber;
    return created;
}

int ius3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DParametricSource != NULL)
    {
        free(ius3DParametricSource);
        ius3DParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startAngle, actual->startAngle) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->angularDelta, actual->angularDelta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->locationCount; i++ )
    {
        if( ius3DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius3DParametricSourceGetFNumber
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->fNumber;
}

float ius3DParametricSourceGetAngularDelta
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->angularDelta;
}

float ius3DParametricSourceGetStartAngle
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->startAngle;
}

float ius3DParametricSourceGetDeltaPhi
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->deltaPhi;
}

float ius3DParametricSourceGetStartPhi
(
    iu3dps_t ius3DParametricSource
)
{
    if ( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->startPhi;
}

iu3dp_t ius3DParametricSourceGetPosition
(
    iu3dps_t ius3DParametricSource,
    int index
)
{
    if ( ius3DParametricSource == NULL  ) return IU3DP_INVALID;
    if ( index >= ius3DParametricSource->locationCount || index < 0) return IU3DP_INVALID;
    return &ius3DParametricSource->pLocations[index];
}

// Setters
int ius3DParametricSourceSetPosition
(
    iu3dps_t ius3DParametricSource,
    iu3dp_t  pos,
    int index
)
{
  if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
  if (pos == NULL) return IUS_ERR_VALUE;
  if (index >= ius3DParametricSource->locationCount) return IUS_ERR_VALUE;

  ius3DParametricSource->pLocations[index] = *pos;
  return IUS_E_OK;
}

int ius3DParametricSourceSetFNumber
(
    iu3dps_t ius3DParametricSource,
    float FNumber
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->fNumber = FNumber;
    return IUS_E_OK;
}

int ius3DParametricSourceSetAngularDelta
(
    iu3dps_t ius3DParametricSource,
    float angularDelta
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->angularDelta = angularDelta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartAngle
(
    iu3dps_t ius3DParametricSource,
    float startAngle
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->startAngle = startAngle;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaPhi
(
    iu3dps_t ius3DParametricSource,
    float deltaPhi
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->deltaPhi = deltaPhi;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartPhi
(
    iu3dps_t ius3DParametricSource,
    float startPhi
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->startPhi = startPhi;
    return IUS_E_OK;
}



// serialization
static int ius3DParametricSourceSaveLocations
(
    iu3dps_t pSource,
    char *parentPath,
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dp_t sourceElement;
    int i,size = pSource->locationCount;
    sprintf(path, LOCATIONSSIZEFMT, parentPath);
    int status = iusHdf5WriteInt(group_id, path, &(size), 1);

//     iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU3DP_INVALID) continue;
        sprintf(path, LOCATIONFMT, parentPath, i);
        status = ius3DPositionSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}

static int ius3DParametricSourceLoadLocations
(
    iu3dps_t source,
    char *parentPath,
    hid_t handle
)
{
    int p;
    char path[IUS_MAX_HDF5_PATH];
    iu3dp_t pos;

    sprintf(path, LOCATIONSSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(handle, path, &(source->locationCount));

    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, LOCATIONFMT, parentPath, p);
        pos = ius3DPositionLoad(handle,path);
        if (pos == IU3DP_INVALID)
        {
            status = IUS_E_OK;
            break;
        }
        ius3DParametricSourceSetPosition(source, pos, p);
    }
    return status;
}

int ius3DParametricSourceSave
(
    iu3dps_t source,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];

    // Base
    status = iusBaseSourceSave((ius_t)source,parentPath,handle);

    // Parametric stuff
    sprintf(path, FNUMBERFMT, parentPath);
    status |= iusHdf5WriteFloat( handle, path, &(source->fNumber), 1);
    sprintf(path, ANGULARDELTAFMT, parentPath);
    status |= iusHdf5WriteFloat( handle, path, &(source->angularDelta), 1);
    sprintf(path, STARTANGLEFMT, parentPath);
    status |= iusHdf5WriteFloat( handle, path, &(source->startAngle), 1);
    sprintf(path, DELTAPHIFMT, parentPath);
    status |= iusHdf5WriteFloat( handle, path, &(source->deltaPhi), 1);
    sprintf(path, STARTPHIFMT, parentPath);
    status |= iusHdf5WriteFloat( handle, path, &(source->startPhi), 1);
    sprintf(path, LOCATIONSFMT, parentPath);

    // Save locations
    status |= ius3DParametricSourceSaveLocations(source,path,handle);
    return status;
}


iu3dps_t ius3DParametricSourceLoad
(
    hid_t handle,
    char *parentPath,
    char *label
)
{
    int status = 0;
    char path[IUS_MAX_HDF5_PATH];
    char lpath[IUS_MAX_HDF5_PATH];

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< angle in [rad] between sources */
    int locationCount;
    iu3dps_t  source;


    sprintf(path, FNUMBERFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(fNumber));
    sprintf(path, ANGULARDELTAFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(angularDelta));
    sprintf(path, STARTANGLEFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(startAngle));
    sprintf(path, DELTAPHIFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(deltaPhi));
    sprintf(path, STARTPHIFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(startPhi));
    sprintf(lpath, LOCATIONSFMT, parentPath);
    sprintf(path, LOCATIONSSIZEFMT, lpath);
    status |= iusHdf5ReadInt(handle, path, &(locationCount));
    if (status < 0)
        return NULL;

    source = ius3DParametricSourceCreate(label,locationCount,fNumber,angularDelta,startAngle,deltaPhi,startPhi);

    sprintf(path, LOCATIONSFMT, parentPath);
    status = ius3DParametricSourceLoadLocations(source,path,handle);
    if (status <-0)
        return NULL;
    return source;
}
