#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include <iuf.h>
#include <iufSourcePrivate.h>
#include <iufPositionPrivate.h>

struct Iuf3DNonParametricSource
{
    struct IufSource base;
    int numLocations;
    struct Iuf3DPosition *pLocations;
    IUF_BOOL deepDelete;
} ;

// ADT
iu3dnps_t iuf3DNonParametricSourceCreate
(
    int numLocations
)
{
    IUF_ERR_EVAL_N_RETURN(numLocations <= 0, IU3DNPS_INVALID);
    iu3dnps_t created = calloc(1,sizeof(Iuf3DNonParametricSource));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DNonParametricSource, IU3DNPS_INVALID);
    created->pLocations = (struct Iuf3DPosition *) calloc((size_t)numLocations, sizeof(Iuf3DPosition));
    if( created->pLocations == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IU3DNPS_INVALID;
    }

    created->deepDelete = IUF_FALSE;
    created->base.type = IUF_3D_NON_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    return created;
}

int iuf3DNonParametricSourceDelete
(
    iu3dnps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUF_E_OK;
}


// operations
int iuf3DNonParametricSourceCompare
(
    iu3dnps_t reference,
    iu3dnps_t actual
)
{
    if (reference == actual ) return IUF_TRUE;
    if (reference == NULL || actual == NULL ) return IUF_FALSE;
    if (reference->numLocations != actual->numLocations) return IUF_FALSE;
    if (iufBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUF_FALSE ) return IUF_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
    {
        if( iuf3DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUF_FALSE )
        {
            return IUF_FALSE;
        }
    }
    return IUF_TRUE;
}

// Getters
iu3dp_t iuf3DNonParametricSourceGetPosition
(
    iu3dnps_t source,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IU3DP_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0 || index >= source->numLocations, IU3DP_INVALID);
    return &source->pLocations[index];
}

int iuf3DNonParametricSourceGetNumLocations
(
    iu3dnps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numLocations;
}

// Setters
int iuf3DNonParametricSourceSetPosition
(
    iu3dnps_t source,
    iu3dp_t  pos,
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
static int iuf3DNonParametricSourceSaveLocations
(
    iu3dnps_t source,
    hid_t handle
)
{
	hid_t location_id;
    char path[IUF_MAX_HDF5_PATH];
    iu3dp_t sourceElement;
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t locationList_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    int i, size = source->numLocations;
    int status = iufHdf5WriteInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = &source->pLocations[i];
        if(sourceElement == IU3DP_INVALID) continue;
        sprintf(path, IUF_INPUTFILE_PATH_SOURCE_LOCATION, i);
		location_id = H5Gcreate(locationList_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
        status = iuf3DPositionSave(sourceElement, location_id);
		H5Gclose(location_id);
        if(status != IUF_E_OK) break;
    }
	H5Gclose(locationList_id);

    return status;
}

static int iuf3DNonParametricSourceLoadLocations
(
    iu3dnps_t source,
    hid_t handle
)
{
    int p, status;
    char path[IUF_MAX_HDF5_PATH];
    iu3dp_t pos;
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t locationList_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
	status = iufHdf5ReadInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(source->numLocations));
    for (p = 0; p < source->numLocations; p++)
    {
        sprintf(path, IUF_INPUTFILE_PATH_SOURCE_LOCATION, p);
		hid_t location_id = H5Gopen(locationList_id, path, H5P_DEFAULT);
		pos = iuf3DPositionLoad(location_id);
        if (pos == IU3DP_INVALID)
        {
            status = IUF_ERR_VALUE;
            break;
        }
		H5Gclose(location_id);
        iuf3DNonParametricSourceSetPosition(source, pos, p);
        iuf3DPositionDelete(pos);
    }
	H5Gclose(locationList_id);
    return status;
}

int iuf3DNonParametricSourceSave
(
    iu3dnps_t source,
    hid_t handle
)
{
    // Base
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    int status = iufBaseSourceSave((ius_t)source, handle);

    // Save locations
    status |= iuf3DNonParametricSourceSaveLocations(source, handle);
    return status;
}


iu3dnps_t iuf3DNonParametricSourceLoad
(
    hid_t handle
)
{
    int numLocations;
    iu3dnps_t  source;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DNPS_INVALID);
    hid_t locationList_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLIST, H5P_DEFAULT);
    int status = iufHdf5ReadInt(locationList_id, IUF_INPUTFILE_PATH_SOURCE_LOCATIONLISTSIZE, &(numLocations));
	H5Gclose(locationList_id);
    if (status < 0)
        return IU3DNPS_INVALID;

    source = iuf3DNonParametricSourceCreate(numLocations);
    status = iuf3DNonParametricSourceLoadLocations(source, handle);
    source->deepDelete = IUF_TRUE;
    if (status <-0)
        return IU3DNPS_INVALID;
    return source;
}
