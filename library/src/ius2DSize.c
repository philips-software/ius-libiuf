
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <ius2DSizePrivate.h>

// ADT
iu2ds_t ius2DSizeCreate
(
    float sx,
    float sz
)
{
    IUS_ERR_EVAL_N_RETURN(sx == NAN, IU2DS_INVALID);
    IUS_ERR_EVAL_N_RETURN(sz == NAN, IU2DS_INVALID);
    iu2ds_t created = calloc(1,sizeof(Ius2DSize));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DSize, IU2DS_INVALID);
    created->sx = sx;
    created->sz = sz;
    return created;
}

int ius2DSizeDelete
(
    iu2ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(size, IUS_ERR_VALUE);
    free(size);
    return IUS_E_OK;
}


// operations
int ius2DSizeCompare
(
    iu2ds_t reference,
    iu2ds_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sx, actual->sx ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sz, actual->sz ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}


iu2ds_t ius2DSizeLoad
(
    hid_t handle
)
{
    int status=0;
    float sx,sz;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DS_INVALID);
	hid_t size_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SIZE, H5P_DEFAULT);
    status |= iusHdf5ReadFloat(size_id, IUS_INPUTFILE_PATH_SIZE_X, &(sx));
    status |= iusHdf5ReadFloat(size_id, IUS_INPUTFILE_PATH_SIZE_Z, &(sz));
	H5Gclose(size_id);
    if (status < 0)
        return IU2DS_INVALID;

    return ius2DSizeCreate(sx,sz);
}

int ius2DSizeSave
(
    iu2ds_t size,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(size, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SIZE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_X, &(size->sx), 1);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Z, &(size->sz), 1);
    status |= H5Gclose(group_id );
    return status;
}

