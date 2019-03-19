
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <iuf.h>
#include <iuf2DSizePrivate.h>

// ADT
iu2ds_t iuf2DSizeCreate
(
    float sx,
    float sz
)
{
    IUF_ERR_EVAL_N_RETURN(sx == NAN, IU2DS_INVALID);
    IUF_ERR_EVAL_N_RETURN(sz == NAN, IU2DS_INVALID);
    iu2ds_t created = calloc(1,sizeof(Iuf2DSize));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DSize, IU2DS_INVALID);
    created->sx = sx;
    created->sz = sz;
    return created;
}

int iuf2DSizeDelete
(
    iu2ds_t size
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(size, IUF_ERR_VALUE);
    free(size);
    return IUF_E_OK;
}


// operations
int iuf2DSizeCompare
(
    iu2ds_t reference,
    iu2ds_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sx, actual->sx ) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sz, actual->sz ) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}


iu2ds_t iuf2DSizeLoad
(
    hid_t handle
)
{
    int status=0;
    float sx,sz;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DS_INVALID);
	hid_t size_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SIZE, H5P_DEFAULT);
    status |= iufHdf5ReadFloat(size_id, IUF_INPUTFILE_PATH_SIZE_X, &(sx));
    status |= iufHdf5ReadFloat(size_id, IUF_INPUTFILE_PATH_SIZE_Z, &(sz));
	H5Gclose(size_id);
    if (status < 0)
        return IU2DS_INVALID;

    return iuf2DSizeCreate(sx,sz);
}

int iuf2DSizeSave
(
    iu2ds_t size,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(size, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    hid_t group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_SIZE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_SIZE_X, &(size->sx), 1);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_SIZE_Z, &(size->sz), 1);
    status |= H5Gclose(group_id );
    return status;
}

