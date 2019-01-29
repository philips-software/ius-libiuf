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
    int numLocations;
    struct Ius2DPosition *pLocations;
};

// ADT
iu2dnps_t ius2DNonParametricSourceCreate
(
    int numLocations
)
{
    IUS_ERR_EVAL_N_RETURN(numLocations <= 0, IU2DNPS_INVALID);
    iu2dnps_t created = calloc(1,sizeof(Ius2DNonParametricSource));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DNonParametricSource, IU2DNPS_INVALID);
    created->pLocations = (struct Ius2DPosition *) calloc((size_t)numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IU2DNPS_INVALID;
    }

    created->base.type = IUS_2D_NON_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    return created;
}

int ius2DNonParametricSourceDelete
(
    iu2dnps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUS_E_OK;
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
    if (reference->numLocations != actual->numLocations) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
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
    iu2dnps_t source,
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IU2DP_INVALID);
    IUS_ERR_EVAL_N_RETURN(index < 0 || index >= source->numLocations, IU2DP_INVALID);
    return &source->pLocations[index];
}


int ius2DNonParametricSourceGetNumLocations
(
    iu2dnps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numLocations;
}


// Setters
int ius2DNonParametricSourceSetPosition
(
    iu2dnps_t source,
    iu2dp_t  pos,
    int index
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_CHECK_NULL_N_RETURN(pos, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(index < 0 || index >= source->numLocations, IUS_ERR_VALUE);
    source->pLocations[index] = *pos;
    return IUS_E_OK;
}

// serialization
static int ius2DNonParametricSourceSaveLocations
(
    iu2dnps_t source,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUS_MAX_HDF5_PATH];
    iu2dp_t sourceElement;

    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    hid_t locationList_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    int i, size = source->numLocations;
    int status = iusHdf5WriteInt(locationList_id, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &source->pLocations[i];
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

    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

	hid_t locationList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    status = iusHdf5ReadInt(locationList_id, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(source->numLocations));
    for (p = 0; p < source->numLocations; p++)
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
        ius2DPositionDelete(pos);
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
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

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
    int numLocations;
    iu2dnps_t  source;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DNPS_INVALID);
	hid_t locationList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    int status = iusHdf5ReadInt(locationList_id, IUS_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(numLocations));
	H5Gclose(locationList_id);
    if (status < 0)
        return IU2DNPS_INVALID;

    source = ius2DNonParametricSourceCreate(numLocations);
    status = ius2DNonParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return IU2DNPS_INVALID;
    return source;
}
