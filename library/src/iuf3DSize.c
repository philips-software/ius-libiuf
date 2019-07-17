
#include <math.h>
#include <stdlib.h>

#include <iuf.h>


// ADT
iu3ds_t iuf3DSizeCreate
(
    float sx,
    float sy,
    float sz
)
{
    IUF_ERR_EVAL_N_RETURN(sx == NAN, IU3DS_INVALID);
    IUF_ERR_EVAL_N_RETURN(sy == NAN, IU3DS_INVALID);
    IUF_ERR_EVAL_N_RETURN(sz == NAN, IU3DS_INVALID);
    iu3ds_t created = calloc(1,sizeof(Iuf3DSize));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DSize, IU3DS_INVALID);
    created->sx = sx;
    created->sy = sy;
    created->sz = sz;
    return created;
}

int iuf3DSizeDelete
(
    iu3ds_t size
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(size, IUF_ERR_VALUE);
    free(size);
    return IUF_E_OK;
}


// operations
int iuf3DSizeCompare
(
    iu3ds_t reference,
    iu3ds_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sx, actual->sx ) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sy, actual->sy ) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sz, actual->sz ) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}

iu3ds_t iuf3DSizeLoad
(
    hid_t handle
)
{
    int status=0;
    float sx,sy,sz;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DS_INVALID);
	hid_t group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_SIZE, H5P_DEFAULT);
    status |= iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_SIZE_X, &(sx));
    status |= iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_SIZE_Y, &(sy));
    status |= iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_SIZE_Z, &(sz));
    if (status < 0)
        return IU3DS_INVALID;
    return iuf3DSizeCreate(sx,sy,sz);
}

int iuf3DSizeSave
(
    iu3ds_t size,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(size, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    hid_t group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_SIZE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_SIZE_X, &(size->sx), 1);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_SIZE_Y, &(size->sy), 1);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_SIZE_Z, &(size->sz), 1);
    status |= H5Gclose(group_id);

    return status;
}

