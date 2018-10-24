//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusSourcePrivate.h>
#include <iusPositionPrivate.h>

struct Ius2DNonParametricSource
{
    struct IusSource base;
    int locationCount;
    struct Ius2DPosition *pLocations;
};

// ADT
iu2dnps_t ius2DNonParametricSourceCreate
(
    int numLocations
)
{
    if ( numLocations <= 0 ) return  NULL;
    iu2dnps_t created = calloc(1,sizeof(Ius2DNonParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc((size_t)numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_NON_PARAMETRIC_SOURCE;
    created->locationCount = numLocations;
    return created;
}

int ius2DNonParametricSourceDelete
(
    iu2dnps_t ius2DNonParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DNonParametricSource != NULL)
    {
        free(ius2DNonParametricSource);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->locationCount != actual->locationCount) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
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


iu2dp_t ius2DNonParametricSourceGetPosition
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
int ius2DNonParametricSourceSetPosition
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
static int ius2DNonParametricSourceSaveLocations
(
    iu2dnps_t pSource,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t sourceElement;

	hid_t locationList_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    int i, size = pSource->locationCount;
    int status = iusHdf5WriteInt(locationList_id, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &pSource->pLocations[i];
        if(sourceElement == IU2DP_INVALID) continue;
		sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, i);
		location_id = H5Gcreate(locationList_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = ius2DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUS_E_OK) break;
    }

	H5Gclose(locationList_id);
    return status;
}

static int ius2DNonParametricSourceLoadLocations
(
    iu2dnps_t source,
    hid_t handle
)
{
    int p, status = IUS_E_OK;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t pos;
	hid_t locationList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    for (p = 0; p < source->locationCount; p++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_SOURCE_LOCATION, p);
		hid_t location_id = H5Gopen(locationList_id, path, H5P_DEFAULT);
        pos = ius2DPositionLoad(location_id);
        if (pos == IU2DP_INVALID)
        {
            status = IUS_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        ius2DNonParametricSourceSetPosition(source, pos, p);
    }
	H5Gclose(locationList_id);

    return status;
}

int ius2DNonParametricSourceSave
(
    iu2dnps_t source,
    hid_t handle
)
{
    // Base
    int status = iusBaseSourceSave((ius_t)source, handle);

    // Save locations
    status |= ius2DNonParametricSourceSaveLocations(source, handle);
    return status;
}


iu2dnps_t ius2DNonParametricSourceLoad
(
    hid_t handle
)
{
    int locationCount;
    iu2dnps_t  source;
	hid_t locationList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    int status = iusHdf5ReadInt(locationList_id, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(locationCount));
	H5Gclose(locationList_id);
    if (status < 0)
        return IU2DNPS_INVALID;

    source = ius2DNonParametricSourceCreate(locationCount);
    status = ius2DNonParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return IU2DNPS_INVALID;
    return source;
}
