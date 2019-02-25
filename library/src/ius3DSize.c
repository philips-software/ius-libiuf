
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>


// ADT
iu3ds_t ius3DSizeCreate
(
    float sx,
    float sy,
    float sz
)
{
    IUS_ERR_EVAL_N_RETURN(sx == NAN, IU3DS_INVALID);
    IUS_ERR_EVAL_N_RETURN(sy == NAN, IU3DS_INVALID);
    IUS_ERR_EVAL_N_RETURN(sz == NAN, IU3DS_INVALID);
    iu3ds_t created = calloc(1,sizeof(Ius3DSize));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius3DSize, IU3DS_INVALID);
    created->sx = sx;
    created->sy = sy;
    created->sz = sz;
    return created;
}

int ius3DSizeDelete
(
    iu3ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(size, IUS_ERR_VALUE);
    free(size);
    return IUS_E_OK;
}


// operations
int ius3DSizeCompare
(
    iu3ds_t reference,
    iu3ds_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sx, actual->sx ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sy, actual->sy ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sz, actual->sz ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

float ius3DSizeGetX
(
    iu3ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(size, NAN);
    return size->sx;
}

    float ius3DSizeGetY
(
    iu3ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(size, NAN);
    return size->sy;
}

float ius3DSizeGetZ
(
    iu3ds_t size
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(size, NAN);
    return size->sz;
}

iu3ds_t ius3DSizeLoad
(
    hid_t handle
)
{
    int status=0;
    float sx,sy,sz;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DS_INVALID);
	hid_t group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_SIZE, H5P_DEFAULT);
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_SIZE_X, &(sx));
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Y, &(sy));
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Z, &(sz));
    if (status < 0)
        return IU3DS_INVALID;
    return ius3DSizeCreate(sx,sy,sz);
}

int ius3DSizeSave
(
    iu3ds_t size,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(size, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SIZE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_X, &(size->sx), 1);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Y, &(size->sy), 1);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Z, &(size->sz), 1);
    status |= H5Gclose(group_id);

    return status;
}

