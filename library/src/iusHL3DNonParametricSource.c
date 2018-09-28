//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
// todo: check if the "include" filder below is needed
#include <include/iusHLSourceImp.h>
#include <include/iusHLPositionImp.h>
#include <include/iusHL3DNonParametricSource.h>
#include <include/iusHDF5.h>

struct Ius3DNonParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius3DPosition *pLocations;
} ;

// ADT
iu3dnps_t iusHL3DNonParametricSourceCreate
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

int iusHL3DNonParametricSourceDelete
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
int iusHL3DNonParametricSourceCompare
(
    iu3dnps_t reference,
    iu3dnps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusHLBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->locationCount; i++ )
    {
        if( iusHL3DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
iu3dp_t iusHL3DNonParametricSourceGetPosition
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
int iusHL3DNonParametricSourceSetPosition
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
static int iusHL3DNonParametricSourceSaveLocations
(
    iu3dnps_t pSource,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUS_MAX_HDF5_PATH];
    iu3dp_t sourceElement;

    int i, size = pSource->locationCount;
    int status = iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU3DP_INVALID) continue;
        sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, i);
		location_id = H5Gcreate(handle, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iusHL3DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUS_E_OK) break;
    }
    return status;
}

static int iusHL3DNonParametricSourceLoadLocations
(
    iu3dnps_t source,
    hid_t handle
)
{
    int p, status = IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu3dp_t pos;

    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, p);
		hid_t location_id = H5Gopen(handle, path, H5P_DEFAULT);
		pos = iusHL3DPositionLoad(location_id);
        if (pos == IU3DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        iusHL3DNonParametricSourceSetPosition(source, pos, p);
    }
    return status;
};

int iusHL3DNonParametricSourceSave
(
    iu3dnps_t source,
    hid_t handle
)
{
    // Base
    int status = iusHLBaseSourceSave((ius_t)source, handle);

    // Save locations
    status |= iusHL3DNonParametricSourceSaveLocations(source, handle);
    return status;
}


iu3dnps_t iusHL3DNonParametricSourceLoad
(
    hid_t handle,
    char *label
)
{
    int locationCount;
    iu3dnps_t  source;

    int status = iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(locationCount));
    if (status < 0)
        return NULL;

    source = iusHL3DNonParametricSourceCreate(label,locationCount);
    status = iusHL3DNonParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return NULL;
    return source;
}
