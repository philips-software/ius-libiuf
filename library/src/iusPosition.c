//
// Created by Ruijzendaal on 28/03/2018.
//
#include <stdlib.h>

#include <iusInputFileStructure.h>
#include <iusPositionImp.h>
#include <iusUtil.h>
#include <include/iusHDF5.h>

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

Ius3DPosition *ius3DPositionCreate
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


Ius2DPosition *ius2DPositionCreate
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

void ius3DPositionDelete
(
    Ius3DPosition *iusPos
)
{
    free(iusPos);
}

void ius2DPositionDelete
(
    Ius2DPosition *iusPos
)
{
    free(iusPos);
}

// TODO: position should serialize to a "position" with x,y,z properties instead of 
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
    const int verbose = 1;

	hid_t position_id;
	int status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_POSITION, 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		position_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_POSITION, 
										H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
	}
    status = iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(position->x), 1, verbose);
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Y, &(position->y), 1, verbose);
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(position->z), 1, verbose);
	H5Gclose(position_id);
    return status;
}

int ius2DPositionSave
(
    iu2dp_t position,
    hid_t handle
)
{
    const int verbose = 1;
	hid_t position_id;
	int status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_POSITION, 0, NULL); // todo centralize the path "Sources"
	if (status != 0) // the group does not exist yet
	{
		position_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_POSITION, 
									H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		position_id = H5Gopen(handle, IUS_INPUTFILE_PATH_POSITION, H5P_DEFAULT);
	}
    status = iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_X, &(position->x), 1, verbose);   //todo put x and z as defines in central place
    status |= iusHdf5WriteFloat(position_id, IUS_INPUTFILE_PATH_POSITION_Z, &(position->z), 1, verbose);
	H5Gclose(position_id);

    return status;
}