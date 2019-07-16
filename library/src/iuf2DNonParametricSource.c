#include <stdlib.h>

#include <iuf.h>
#include <iufSourcePrivate.h>
#include <iufPositionPrivate.h>

struct Iuf2DNonParametricSource
{
    struct IufSource base;
    int numLocations;
    struct Iuf2DPosition *pLocations;
};

// ADT
iu2dnps_t iuf2DNonParametricSourceCreate
(
    int numLocations
)
{
    IUF_ERR_EVAL_N_RETURN(numLocations <= 0, IU2DNPS_INVALID);
    iu2dnps_t created = calloc(1,sizeof(Iuf2DNonParametricSource));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DNonParametricSource, IU2DNPS_INVALID);
    created->pLocations = (struct Iuf2DPosition *) calloc((size_t)numLocations, sizeof(Iuf2DPosition));
    if( created->pLocations == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IU2DNPS_INVALID;
    }

    created->base.type = IUF_2D_NON_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    return created;
}

int iuf2DNonParametricSourceDelete
(
    iu2dnps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUF_E_OK;
}


// operations
int iuf2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
)
{
    if (reference == actual ) return IUF_TRUE;
    if (reference == NULL || actual == NULL ) return IUF_FALSE;
    if (reference->numLocations != actual->numLocations) return IUF_FALSE;
    if (iufBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUF_FALSE ) return IUF_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
    {
        if( iuf2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUF_FALSE )
        {
            return IUF_FALSE;
        }
    }
    return IUF_TRUE;
}

// Getters


iu2dp_t iuf2DNonParametricSourceGetPosition
(
    iu2dnps_t source,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IU2DP_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0 || index >= source->numLocations, IU2DP_INVALID);
    return &source->pLocations[index];
}


int iuf2DNonParametricSourceGetNumLocations
(
    iu2dnps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numLocations;
}


// Setters
int iuf2DNonParametricSourceSetPosition
(
    iu2dnps_t source,
    iu2dp_t  pos,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(pos, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0 || index >= source->numLocations, IUF_ERR_VALUE);
    source->pLocations[index] = *pos;
    return IUF_E_OK;
}

// serialization
static int iuf2DNonParametricSourceSaveLocations
(
    iu2dnps_t source,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUF_MAX_HDF5_PATH];
    iu2dp_t sourceElement;

    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t locationList_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    int i, size = source->numLocations;
    int status = iufHdf5WriteInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &source->pLocations[i];
        if(sourceElement == IU2DP_INVALID) continue;
		sprintf(path, IUF_INPUTFILE_PATH_SOURCE_LOCATION, i);
		location_id = H5Gcreate(locationList_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iuf2DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUF_E_OK) break;
    }

	H5Gclose(locationList_id);
    return status;
}

static int iuf2DNonParametricSourceLoadLocations
(
    iu2dnps_t source,
    hid_t handle
)
{
    int p, status = IUF_E_OK;
    char path[IUF_MAX_HDF5_PATH];
    iu2dp_t pos;

    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

	hid_t locationList_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    status = iufHdf5ReadInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(source->numLocations));
    for (p = 0; p < source->numLocations; p++)
    {
        sprintf(path, IUF_INPUTFILE_PATH_SOURCE_LOCATION, p);
		hid_t location_id = H5Gopen(locationList_id, path, H5P_DEFAULT);
        pos = iuf2DPositionLoad(location_id);
        if (pos == IU2DP_INVALID)
        {
            status = IUF_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        iuf2DNonParametricSourceSetPosition(source, pos, p);
        iuf2DPositionDelete(pos);
    }
	H5Gclose(locationList_id);
    return status;
}

int iuf2DNonParametricSourceSave
(
    iu2dnps_t source,
    hid_t handle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    // Base
    int status = iufBaseSourceSave((ius_t)source, handle);

    // Save locations
    status |= iuf2DNonParametricSourceSaveLocations(source, handle);
    return status;
}


iu2dnps_t iuf2DNonParametricSourceLoad
(
    hid_t handle
)
{
    int numLocations;
    iu2dnps_t  source;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DNPS_INVALID);
	hid_t locationList_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    int status = iufHdf5ReadInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(numLocations));
	H5Gclose(locationList_id);
    if (status < 0)
        return IU2DNPS_INVALID;

    source = iuf2DNonParametricSourceCreate(numLocations);
    status = iuf2DNonParametricSourceLoadLocations(source, handle);
    if (status <-0)
        return IU2DNPS_INVALID;
    return source;
}
