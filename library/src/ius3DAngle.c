
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <ius.h>
#include <ius3DAnglePrivate.h>

// ADT
iu3da_t ius3DAngleCreate
(
    float theta,
    float phi
)
{
    IUS_ERR_EVAL_N_RETURN(theta == NAN, IU3DA_INVALID);
    IUS_ERR_EVAL_N_RETURN(phi == NAN, IU3DA_INVALID);
    iu3da_t created = calloc(1,sizeof(Ius3DAngle));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius3DAngle, IU3DA_INVALID);
    created->theta = theta;
    created->phi = phi;
    return created;
}

int ius3DAngleDelete
(
    iu3da_t angle
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(angle, IUS_ERR_VALUE);
    free(angle);
    return IUS_E_OK;
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

float ius3DAngleGetTheta
(
    iu3da_t angle
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(angle, NAN);
    return angle->theta;
}

float ius3DAngleGetPhi(
    iu3da_t angle
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(angle, NAN);
    return angle->phi;
}

int ius3DAngleSave
(
    iu3da_t angle,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(angle, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    hid_t group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_ANGLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_THETA, &(angle->theta), 1);
    status |= iusHdf5WriteFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_PHI, &(angle->phi), 1);
    status |= H5Gclose(group_id );
    return status;
}

iu3da_t ius3DAngleLoad
(
    hid_t handle
)
{
    int status=0;
    float theta,phi;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DA_INVALID);
	hid_t group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_ANGLE, H5P_DEFAULT);
    status = iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_THETA, &theta);
    status |= iusHdf5ReadFloat(group_id, IUS_INPUTFILE_PATH_ANGLE_PHI, &phi);
	H5Gclose(group_id);

    if (status<0)
        return IU3DA_INVALID;

    return ius3DAngleCreate(theta,phi);
}

