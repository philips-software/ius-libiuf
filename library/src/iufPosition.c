//
// Created by Ruijzendaal on 28/03/2018.
//
#include <stdlib.h>

#include <iuf.h>
#include <iufPositionPrivate.h>

IUF_BOOL iuf3DPositionCompare
(
    Iuf3DPosition *reference,
    Iuf3DPosition *actual
)
{
    if (IUF_EQUAL_FLOAT(reference->x, actual->x) &&
    IUF_EQUAL_FLOAT(reference->y, actual->y) &&
    IUF_EQUAL_FLOAT(reference->z, actual->z) )
        return IUF_TRUE;
    else
        return IUF_FALSE;
}


IUF_BOOL iuf2DPositionCompare
(
    Iuf2DPosition *reference,
    Iuf2DPosition *actual
)
{
    if (IUF_EQUAL_FLOAT(reference->x, actual->x) &&
    IUF_EQUAL_FLOAT(reference->z, actual->z) )
        return IUF_TRUE;
    else
        return IUF_FALSE;
}

iu3dp_t iuf3DPositionCreate
(
    float x,
    float y,
    float z
)
{
    iu3dp_t created = calloc(1, sizeof(Iuf3DPosition));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DPosition, IU3DP_INVALID);
    created->x=x;
    created->y=y;
    created->z=z;
    return created;
}


iu2dp_t iuf2DPositionCreate
(
    float x,
    float z
)
{
    Iuf2DPosition *created = calloc(1, sizeof(Iuf2DPosition));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DPosition, IU2DP_INVALID);
    created->x=x;
    created->z=z;
    return created;
}

void iuf3DPositionDelete
(
    iu3dp_t pos
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pos, /**/);
    free(pos);
}

void iuf2DPositionDelete
(
    iu2dp_t pos
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pos, /**/);
    free(pos);
}

iu3dp_t iuf3DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    float x,y,z;
    hid_t position_id = H5Gopen(handle, IUF_INPUTFILE_PATH_POSITION, H5P_DEFAULT);

    status |= iufHdf5ReadFloat(position_id, IUF_INPUTFILE_PATH_POSITION_X, &(x));
    status |= iufHdf5ReadFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Y, &(y));
    status |= iufHdf5ReadFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Z, &(z));

    if (status < 0)
        return IU3DP_INVALID;
    H5Gclose(position_id);
    return iuf3DPositionCreate(x,y,z);
}

iu2dp_t iuf2DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DP_INVALID);

    float x,z;
    hid_t position_id = H5Gopen(handle, IUF_INPUTFILE_PATH_POSITION, H5P_DEFAULT);

    status |= iufHdf5ReadFloat(position_id, IUF_INPUTFILE_PATH_POSITION_X, &(x));
    status |= iufHdf5ReadFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Z, &(z));
    if (status < 0)
        return IU2DP_INVALID;

    H5Gclose(position_id);
    return iuf2DPositionCreate(x,z);
}

int iuf3DPositionSave
(
    iu3dp_t position,
    hid_t handle
)
{
    hid_t position_id;
    IUF_ERR_CHECK_NULL_N_RETURN(position, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    int status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_POSITION, 0, NULL);
    if (status != 0) // the group does not exist yet
    {
        position_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_POSITION,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        position_id = H5Gopen(handle, IUF_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
    }
    status = iufHdf5WriteFloat(position_id, IUF_INPUTFILE_PATH_POSITION_X, &(position->x), 1);
    status |= iufHdf5WriteFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Y, &(position->y), 1);
    status |= iufHdf5WriteFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Z, &(position->z), 1);
    H5Gclose(position_id);
    return status;
}

int iuf2DPositionSave
(
    iu2dp_t position,
    hid_t handle
)
{
    hid_t position_id;
    IUF_ERR_CHECK_NULL_N_RETURN(position, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    int status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_POSITION, 0, NULL);
    if (status != 0) // the group does not exist yet
    {
        position_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_POSITION,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        position_id = H5Gopen(handle, IUF_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
    }
    status = iufHdf5WriteFloat(position_id, IUF_INPUTFILE_PATH_POSITION_X, &(position->x), 1);
    status |= iufHdf5WriteFloat(position_id, IUF_INPUTFILE_PATH_POSITION_Z, &(position->z), 1);
    H5Gclose(position_id);

    return status;
}
