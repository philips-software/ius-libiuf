
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <ius3DSize.h>
#include <include/iusHDF5.h>

// ADT
iu3ds_t ius3DSizeCreate
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

int ius3DSizeDelete
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

#define SIZEXFMT "%s/sx"
#define SIZEYFMT "%s/xy"
#define SIZEZFMT "%s/sz"

iu3ds_t ius3DSizeLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];
    float sx,sy,sz;

    sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, path, &(sx));
    sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, path, &(sy));
    sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5ReadFloat(handle, path, &(sz));
    if (status < 0)
        return IU3DS_INVALID;
    return ius3DSizeCreate(sx,sy,sz);
}

int ius3DSizeSave
(
    iu3ds_t size,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];
    const int verbose = 1;
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, SIZEXFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &(size->sx), 1, verbose);
    sprintf(path, SIZEYFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &(size->sy), 1, verbose);
    sprintf(path, SIZEZFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &(size->sz), 1, verbose);
    status |= H5Gclose(group_id );
    return status;
}
