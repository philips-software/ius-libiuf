
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHL3DSize.h>
#include <include/iusHDF5.h>

// ADT
iu3ds_t iusHL3DSizeCreate
(
    float sx,
    float sy,
    float sz
)
{
    if( sx == NAN || sy == NAN || sz == NAN  ) return IU3DS_INVALID;
    iu3ds_t created = calloc(1,sizeof(Ius3DSize));
    created->sx = sx;
    created->sy = sy;
    created->sz = sz;
    return created;
}

int iusHL3DSizeDelete
(
    iu3ds_t ius3DSize
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DSize != NULL)
    {
        free(ius3DSize);
        ius3DSize = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DSizeCompare
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

//#define SIZEXFMT "%s/sx"
//#define SIZEYFMT "%s/xy"
//#define SIZEZFMT "%s/sz"

iu3ds_t iusHL3DSizeLoad
(
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    float sx,sy,sz;

	hid_t group_id = H5Gopen(handle, "Size", H5P_DEFAULT);
    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5ReadFloat(group_id, "sx", &(sx));
    //sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5ReadFloat(group_id, "sy", &(sy));
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5ReadFloat(group_id, "sz", &(sz));
    if (status < 0)
        return IU3DS_INVALID;
    return iusHL3DSizeCreate(sx,sy,sz);
}

int iusHL3DSizeSave
(
    iu3ds_t size,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    const int verbose = 1;
    hid_t group_id = H5Gcreate(handle, "Size", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, "sx", &(size->sx), 1, verbose);
    //sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, "sy", &(size->sy), 1, verbose);
    //sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, "sz", &(size->sz), 1, verbose);
    status |= H5Gclose(group_id );
    return status;
}

