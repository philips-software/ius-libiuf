//
// Created by Ruijzendaal on 28/03/2018.
//
#include <stdlib.h>

#include <iusHLPositionImp.h>
#include <iusUtil.h>
#include <include/iusHDF5.h>

IUS_BOOL iusHL3DPositionCompare
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


IUS_BOOL iusHL2DPositionCompare
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

Ius3DPosition *iusHL3DPositionCreate
(
    float x,
    float y,
    float z
)
{
    Ius3DPosition *iusPos = calloc(1, sizeof(Ius3DPosition));
    iusPos->x=x;
    iusPos->y=y;
    iusPos->z=z;
    return iusPos;
}


Ius2DPosition *iusHL2DPositionCreate
(
    float x,
    float z
)
{
    Ius2DPosition *iusPos = calloc(1, sizeof(Ius2DPosition));
    iusPos->x=x;
    iusPos->z=z;
    return iusPos;
}

void iusHL3DPositionDelete
(
    Ius3DPosition *iusPos
)
{
    free(iusPos);
}

void iusHL2DPositionDelete
(
    Ius2DPosition *iusPos
)
{
    free(iusPos);
}


//#define SIZEXFMT "%s/x"
//#define SIZEYFMT "%s/y"
//#define SIZEZFMT "%s/z"

iu3dp_t iusHL3DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    float x,y,z;

    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, "x", &(x));
    //sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, "y", &(y));
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, "z", &(z));
    if (status < 0)
        return IU3DP_INVALID;
    return iusHL3DPositionCreate(x,y,z);
}

iu2dp_t iusHL2DPositionLoad
(
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    float x,z;

    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, "x", &(x));
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, "z", &(z));
    if (status < 0)
        return IU2DP_INVALID;
    return iusHL2DPositionCreate(x,z);
}

int iusHL3DPositionSave
(
    iu3dp_t position,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    const int verbose = 1;
    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, "x", &(position->x), 1, verbose);
    //sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, "y", &(position->y), 1, verbose);
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, "z", &(position->z), 1, verbose);
    //status |= H5Gclose(group_id );
    return status;
}

int iusHL2DPositionSave
(
    iu2dp_t position,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    const int verbose = 1;
    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, "x", &(position->x), 1, verbose);   //todo put x and z as defines in central place
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, "z", &(position->z), 1, verbose);
    //status |= H5Gclose(group_id );
    return status;
}