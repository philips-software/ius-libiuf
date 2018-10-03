
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
#include <ius2DSizeImp.h>
#include <include/iusHDF5.h>

// ADT
iu2ds_t ius2DSizeCreate
(
    float sx,
    float sz
)
{
    if( sx == NAN || sz == NAN ) return IU2DS_INVALID;
    iu2ds_t created = calloc(1,sizeof(Ius2DSize));
    created->sx = sx;
    created->sz = sz;
    return created;
}

int ius2DSizeDelete
(
    iu2ds_t ius2DSize
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DSize != NULL)
    {
        free(ius2DSize);
        ius2DSize = NULL;
        status = IUS_E_OK;
    }
    return status;
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
    const int verbose = 1;
    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_SIZE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_X, &(size->sx), 1, verbose);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_SIZE_Z, &(size->sz), 1, verbose);
    status |= H5Gclose(group_id );
    return status;
}

