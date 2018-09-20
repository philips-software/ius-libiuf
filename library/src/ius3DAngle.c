
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <ius3DAngleImp.h>
#include <include/iusHDF5.h>

// ADT
iu3da_t ius3DAngleCreate
(
    float theta,
    float phi
)
{
    if( theta == NAN || phi == NAN ) return IU3DA_INVALID;
    iu3da_t created = calloc(1,sizeof(Ius3DAngle));
    created->theta = theta;
    created->phi = phi;
    return created;
}

int ius3DAngleDelete
(
    iu3da_t ius3DAngle
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DAngle != NULL)
    {
        free(ius3DAngle);
        ius3DAngle = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int ius3DAngleCompare
(
    iu3da_t reference,
    iu3da_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->theta, actual->theta ) == IUS_FALSE ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->phi, actual->phi ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

#define ANGLETHETAFMT "%s/theta"
#define ANGLEPHIFMT "%s/phi"

int ius3DAngleSave
(
    iu3da_t angle,
    char *parentPath,
    hid_t handle
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];
    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    sprintf(path, ANGLETHETAFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &(angle->theta), 1);
    sprintf(path, ANGLEPHIFMT, parentPath);
    status |= iusHdf5WriteFloat(group_id, path, &(angle->phi), 1);
    status |= H5Gclose(group_id );
    return status;
}

iu3da_t ius3DAngleLoad
(
    hid_t handle,
    char *parentPath
)
{
    int status=0;
    char path[IUS_MAX_HDF5_PATH];
    float theta,phi;

    sprintf(path, ANGLETHETAFMT, parentPath);
    status = iusHdf5ReadFloat(handle,path,&theta);
    sprintf(path, ANGLEPHIFMT, parentPath);
    status |= iusHdf5ReadFloat(handle,path,&phi);
    if (status<0)
        return IU3DA_INVALID;
    return ius3DAngleCreate(theta,phi);
}

