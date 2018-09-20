//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <math.h>


#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <include/iusHLSourceImp.h>
#include <include/iusHLPositionImp.h>
#include <include/iusHL2DNonParametricSource.h>
#include <include/iusHDF5.h>

struct Ius2DNonParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius2DPosition *pLocations;
} ;

#define DELTAPHIFMT      "%s/deltaPhi"
#define STARTPHIFMT      "%s/startPhi"
#define LOCATIONSFMT     "%s/Locations"
#define LOCATIONSSIZEFMT "%s/Size"

#define LOCATIONFMT     "%s/Location[%d]"



// ADT
iu2dnps_t iusHL2DNonParametricSourceCreate
(
    char *pLabel,
    int numLocations
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu2dnps_t created = calloc(1,sizeof(Ius2DNonParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc(numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_NON_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    return created;
}

int iusHL2DNonParametricSourceDelete
(
    iu2dnps_t ius2DNonParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DNonParametricSource != NULL)
    {
        free(ius2DNonParametricSource);
        ius2DNonParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusHLBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->locationCount; i++ )
    {
        if( iusHL2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters


iu2dp_t iusHL2DNonParametricSourceGetPosition
(
    iu2dnps_t ius2DNonParametricSource,
    int index
)
{
    if ( ius2DNonParametricSource == NULL  ) return IU2DP_INVALID;
    if ( index >= ius2DNonParametricSource->locationCount || index < 0) return IU2DP_INVALID;
    return &ius2DNonParametricSource->pLocations[index];
}

// Setters
int iusHL2DNonParametricSourceSetPosition
(
    iu2dnps_t ius2DNonParametricSource,
    iu2dp_t  pos,
    int index
)
{
    if (ius2DNonParametricSource == NULL) return IUS_ERR_VALUE;
    if (pos == NULL) return IUS_ERR_VALUE;
    if (index >= ius2DNonParametricSource->locationCount) return IUS_ERR_VALUE;

    ius2DNonParametricSource->pLocations[index] = *pos;
    return IUS_E_OK;
}

// serialization
static int iusHL2DNonParametricSourceSaveLocations
(
    iu2dnps_t pSource,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUS_MAX_HDF5_PATH];
    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iu2dp_t sourceElement;

    int i, size = pSource->locationCount;
    //sprintf(path, LOCATIONSSIZEFMT, parentPath);
    int status = iusHdf5WriteInt(handle, "Size", &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU2DP_INVALID) continue;
		sprintf(path, "Location[%d]", i);
		location_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iusHL2DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUS_E_OK) break;
    }

    //status |= H5Gclose(location_id);
    return status;
}

static int iusHL2DNonParametricSourceLoadLocations
(
    iu2dnps_t source,
    hid_t handle
)
{
    int p, status = IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t pos;

    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, "Location[%d]", p);
		hid_t location_id = H5Gopen(handle, path, H5P_DEFAULT);
        pos = iusHL2DPositionLoad(location_id);
        if (pos == IU2DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        iusHL2DNonParametricSourceSetPosition(source, pos, p);
    }
    return status;
};

int iusHL2DNonParametricSourceSave
(
    iu2dnps_t source,
    hid_t handle
)
{
    //char path[IUS_MAX_HDF5_PATH];
    // Base
    int status = iusHLBaseSourceSave((ius_t)source, handle);

    // Save locations
    //sprintf(path, LOCATIONSFMT, parentPath);
    status |= iusHL2DNonParametricSourceSaveLocations(source, handle);
    return status;
}


iu2dnps_t iusHL2DNonParametricSourceLoad
(
    hid_t handle,
    char *label
)
{
//    char path[IUS_MAX_HDF5_PATH];
//    char lpath[IUS_MAX_HDF5_PATH];

    int locationCount;
    iu2dnps_t  source;

    //sprintf(lpath, LOCATIONSFMT, parentPath);
    //sprintf(path, LOCATIONSSIZEFMT, lpath);
    int status = iusHdf5ReadInt(handle, "Size", &(locationCount));
    if (status < 0)
        return NULL;

    source = iusHL2DNonParametricSourceCreate(label,locationCount);
    //sprintf(path, LOCATIONSFMT, parentPath);
    status = iusHL2DNonParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return NULL;
    return source;
}
