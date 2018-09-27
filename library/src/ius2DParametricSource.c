//
// Created by nlv09165 on 30/07/2018.
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
#include <include/ius2DParametricSource.h>
#include <include/iusHDF5.h>

struct Ius2DParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius2DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
} ;

#define FNUMBERFMT       "%s/fNumber"
#define ANGULARDELTAFMT  "%s/angularDelta"
#define STARTANGLEFMT    "%s/startAngle"
#define DELTAPHIFMT      "%s/deltaPhi"
#define STARTPHIFMT      "%s/startPhi"
#define LOCATIONSFMT     "%s/Locations"
#define LOCATIONSSIZEFMT "%s/Size"
#define LOCATIONFMT      "%s/Location[%d]"



// ADT
iu2dps_t ius2DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc(numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->fNumber = fNumber;
    return created;
}

int ius2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DParametricSource != NULL)
    {
        free(ius2DParametricSource->pLocations);
        free(ius2DParametricSource->base.label);
        free(ius2DParametricSource);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startAngle, actual->startAngle) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->angularDelta, actual->angularDelta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->locationCount; i++ )
    {
        if( ius2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius2DParametricSourceGetFNumber
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->fNumber;
}

float ius2DParametricSourceGetAngularDelta
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->angularDelta;
}

float ius2DParametricSourceGetStartAngle
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->startAngle;
}

iu2dp_t ius2DParametricSourceGetPosition
(
    iu2dps_t ius2DParametricSource,
    int index
)
{
    if ( ius2DParametricSource == NULL  ) return IU2DP_INVALID;
    if ( index >= ius2DParametricSource->locationCount || index < 0) return IU2DP_INVALID;
    return &ius2DParametricSource->pLocations[index];
}

// Setters
int ius2DParametricSourceSetPosition
(
    iu2dps_t ius2DParametricSource,
    iu2dp_t  pos,
    int index
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= ius2DParametricSource->locationCount) return IUS_ERR_VALUE;

    ius2DParametricSource->pLocations[index] = *pos;
    return IUS_E_OK;
}

int ius2DParametricSourceSetFNumber
(
    iu2dps_t ius2DParametricSource,
    float FNumber
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->fNumber = FNumber;
    return IUS_E_OK;
}

int ius2DParametricSourceSetAngularDelta
(
    iu2dps_t ius2DParametricSource,
    float angularDelta
    )
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->angularDelta = angularDelta;
    return IUS_E_OK;
}

int ius2DParametricSourceSetStartAngle
(
    iu2dps_t ius2DParametricSource,
    float startAngle
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->startAngle = startAngle;
    return IUS_E_OK;
}


// serialization
static int ius2DParametricSourceSaveLocations
(
    iu2dps_t pSource,
    char *parentPath,
    hid_t handle
)
{
    char path[64];
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu2dp_t sourceElement;
    int i,size = pSource->locationCount;
    sprintf(path, LOCATIONSSIZEFMT, parentPath);
    int status = iusHdf5WriteInt(group_id, path, &(size), 1);

//     iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU2DP_INVALID) continue;
        sprintf(path, LOCATIONFMT, parentPath, i);
        status = ius2DPositionSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}

static int ius2DParametricSourceLoadLocations
(
    iu2dps_t source,
    char *parentPath,
    hid_t handle
)
{
    int p,status=IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t pos;

    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, LOCATIONFMT, parentPath, p);
        pos = ius2DPositionLoad(handle,path);
        if (pos == IU2DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        ius2DParametricSourceSetPosition(source, pos, p);
    }
    return status;
}

int ius2DParametricSourceSave
(
    iu2dps_t source,
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
    sprintf(path, LOCATIONSFMT, parentPath);

    // Save locations
    status |= ius2DParametricSourceSaveLocations(source,path,handle);
    return status;
}


iu2dps_t ius2DParametricSourceLoad
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
    int locationCount;
    iu2dps_t  source;


    sprintf(path, FNUMBERFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(fNumber));
    sprintf(path, ANGULARDELTAFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(angularDelta));
    sprintf(path, STARTANGLEFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, path, &(startAngle));
    sprintf(lpath, LOCATIONSFMT, parentPath);
    sprintf(path, LOCATIONSSIZEFMT, lpath);
    status |= iusHdf5ReadInt(handle, path, &(locationCount));
    if (status < 0)
        return NULL;

    source = ius2DParametricSourceCreate(label,locationCount,fNumber,angularDelta,startAngle);

    sprintf(path, LOCATIONSFMT, parentPath);
    status = ius2DParametricSourceLoadLocations(source,path,handle);
    if (status <-0)
        return NULL;
    return source;
}
