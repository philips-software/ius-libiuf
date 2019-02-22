//
// Created by Ruijzendaal on 28/03/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusPositionPrivate.h>

IUS_BOOL ius3DPositionCompare
(
    Ius3DPosition *reference,
    Ius3DPosition *actual
)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
    IUS_EQUAL_FLOAT(reference->y, actual->y) &&
    IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}


IUS_BOOL ius2DPositionCompare
(
    Ius2DPosition *reference,
    Ius2DPosition *actual
)
{
    if (IUS_EQUAL_FLOAT(reference->x, actual->x) &&
    IUS_EQUAL_FLOAT(reference->z, actual->z) )
        return IUS_TRUE;
    else
        return IUS_FALSE;
}

iu3dp_t ius3DPositionCreate
(
    float x,
    float y,
    float z
)
{
    iu3dp_t created = calloc(1, sizeof(Ius3DPosition));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius3DPosition, IU3DP_INVALID);
    created->x=x;
    created->y=y;
    created->z=z;
    return created;
}


iu2dp_t ius2DPositionCreate
(
    float x,
    float z
)
{
    Ius2DPosition *created = calloc(1, sizeof(Ius2DPosition));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DPosition, IU2DP_INVALID);
    created->x=x;
    created->z=z;
    return created;
}

float ius2DPositionGetX
(
   iu2dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, NAN);
    return pos->x;
}

float ius2DPositionGetZ
(
   iu2dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, NAN);
    return pos->z;
}

void ius3DPositionDelete
(
    iu3dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, /**/);
    free(pos);
}

void ius2DPositionDelete
(
    iu2dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, /**/);
    free(pos);
}

float ius3DPositionGetX
(
   iu3dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, NAN);
    return pos->x;
}

float ius3DPositionGetY
(
   iu3dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, NAN);
    return pos->y;
}

float ius3DPositionGetZ
(
   iu3dp_t pos
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pos, NAN);
    return pos->z;
}

iu3dp_t ius3DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    float x,y,z;
    hid_t position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);

    status |= iusHdf5ReadFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(x));
    status |= iusHdf5ReadFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Y, &(y));
    status |= iusHdf5ReadFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(z));

    if (status < 0)
        return IU3DP_INVALID;
    H5Gclose(position_id);
    return ius3DPositionCreate(x,y,z);
}

iu2dp_t ius2DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DP_INVALID);

    float x,z;
    hid_t position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);

    status |= iusHdf5ReadFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(x));
    status |= iusHdf5ReadFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(z));
    if (status < 0)
        return IU2DP_INVALID;

    H5Gclose(position_id);
    return ius2DPositionCreate(x,z);
}

int ius3DPositionSave
(
    iu3dp_t position,
    hid_t handle
)
{
    hid_t position_id;
    IUS_ERR_CHECK_NULL_N_RETURN(position, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    int status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_POSITION, 0, NULL);
    if (status != 0) // the group does not exist yet
    {
        position_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_POSITION,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
    }
    status = iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(position->x), 1);
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Y, &(position->y), 1);
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(position->z), 1);
    H5Gclose(position_id);
    return status;
}

int ius2DPositionSave
(
    iu2dp_t position,
    hid_t handle
)
{
    hid_t position_id;
    IUS_ERR_CHECK_NULL_N_RETURN(position, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    int status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_POSITION, 0, NULL);
    if (status != 0) // the group does not exist yet
    {
        position_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_POSITION,
                                H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    }
    else
    {
        position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
    }
    status = iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(position->x), 1);
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(position->z), 1);
    H5Gclose(position_id);

    return status;
}
