
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <iusInputFileStructure.h>
#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHL3DAngleImp.h>
#include <include/iusHDF5.h>

// ADT
iu3da_t iusHL3DAngleCreate
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

int iusHL3DAngleDelete
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
int iusHL3DAngleCompare
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

int iusHL3DAngleSave
(
    iu3da_t angle,
    hid_t handle
)
{
    int status=0;
    const int verbose = 1;
    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_ANGLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_THETA, &(angle->theta), 1, verbose);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_PHI, &(angle->phi), 1, verbose);
    status |= H5Gclose(group_id );
    return status;
}

iu3da_t iusHL3DAngleLoad
(
    hid_t handle
)
{
    int status=0;
    float theta,phi;

	hid_t group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_ANGLE, H5P_DEFAULT);
    status = iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_THETA, &theta);
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_PHI, &phi);
	H5Gclose(group_id);

    if (status<0)
        return IU3DA_INVALID;

    return iusHL3DAngleCreate(theta,phi);
}

