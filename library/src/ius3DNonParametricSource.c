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
#include <include/ius3DNonParametricSource.h>
#include <include/iusHDF5.h>

struct Ius3DNonParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius3DPosition *pLocations;
} ;

#define DELTAPHIFMT      "%s/deltaPhi"
#define STARTPHIFMT      "%s/startPhi"
#define LOCATIONSFMT     "%s/Locations"
#define LOCATIONSSIZEFMT "%s/Size"

#define LOCATIONFMT     "%s/Location[%d]"



// ADT
iu3dnps_t ius3DNonParametricSourceCreate
(
char *pLabel,
int numLocations
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu3dnps_t created = calloc(1,sizeof(Ius3DNonParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius3DPosition *) calloc(numLocations, sizeof(Ius3DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_3D_NON_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    return created;
}

int ius3DNonParametricSourceDelete
(
iu3dnps_t ius3DNonParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DNonParametricSource != NULL)
    {
        free(ius3DNonParametricSource);
        ius3DNonParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius3DNonParametricSourceCompare
(
iu3dnps_t reference,
iu3dnps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
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


iu3dp_t ius3DNonParametricSourceGetPosition
(
    iu3dnps_t ius3DNonParametricSource,
    int index
)
{
    if ( ius3DNonParametricSource == NULL  ) return IU3DP_INVALID;
    if ( index >= ius3DNonParametricSource->locationCount || index < 0) return IU3DP_INVALID;
    return &ius3DNonParametricSource->pLocations[index];
}

// Setters
int ius3DNonParametricSourceSetPosition
(
    iu3dnps_t ius3DNonParametricSource,
    iu3dp_t  pos,
    int index
)
{
    if (ius3DNonParametricSource == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= ius3DNonParametricSource->locationCount) return IUS_ERR_VALUE;

    ius3DNonParametricSource->pLocations[index] = *pos;
    return IUS_E_OK;
}

// serialization
static int ius3DNonParametricSourceSaveLocations
(
    iu3dnps_t pSource,
    char *parentPath,
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu3dp_t sourceElement;

    int i, status, size = pSource->locationCount;
    sprintf(path, LOCATIONSSIZEFMT, parentPath);
    status = iusHdf5WriteInt(handle, path, &(size), 1);

    // iterate over source list elements and save'em
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

static int ius3DNonParametricSourceLoadLocations
(
    iu3dnps_t source,
    char *parentPath,
    hid_t handle
)
{
    int p, status = IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu3dp_t pos;


    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, LOCATIONFMT, parentPath, p);
        pos = ius3DPositionLoad(handle,path);
        if (pos == IU3DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
        ius3DNonParametricSourceSetPosition(source, pos, p);
    }
    return status;
};

int ius3DNonParametricSourceSave
(
    iu3dnps_t source,
    char *parentPath,
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];

    // Base
    int status = iusBaseSourceSave((ius_t)source,parentPath,handle);

    // Save locations
    sprintf(path, LOCATIONSFMT, parentPath);
    status |= ius3DNonParametricSourceSaveLocations(source,path,handle);
    return status;
}


iu3dnps_t ius3DNonParametricSourceLoad
(
    hid_t handle,
    char *parentPath,
    char *label
)
{
    char path[IUS_MAX_HDF5_PATH];
    char lpath[IUS_MAX_HDF5_PATH];

    int locationCount;
    iu3dnps_t  source;



    sprintf(lpath, LOCATIONSFMT, parentPath);
    sprintf(path, LOCATIONSSIZEFMT, lpath);
    int status = iusHdf5ReadInt(handle, path, &(locationCount));
    if (status < 0)
        return NULL;

    source = ius3DNonParametricSourceCreate(label,locationCount);

    sprintf(path, LOCATIONSFMT, parentPath);
    status = ius3DNonParametricSourceLoadLocations(source,path,handle);
    if (status <-0)
        return NULL;
    return source;
}
