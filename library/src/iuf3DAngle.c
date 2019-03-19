
//
// Created by nlv09165 on 24/05/2018.
//
#include <math.h>
#include <stdlib.h>

#include <iuf.h>
#include <iuf3DAnglePrivate.h>

// ADT
iu3da_t iuf3DAngleCreate
(
    float theta,
    float phi
)
{
    IUF_ERR_EVAL_N_RETURN(theta == NAN, IU3DA_INVALID);
    IUF_ERR_EVAL_N_RETURN(phi == NAN, IU3DA_INVALID);
    iu3da_t created = calloc(1,sizeof(Iuf3DAngle));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DAngle, IU3DA_INVALID);
    created->theta = theta;
    created->phi = phi;
    return created;
}

int iuf3DAngleDelete
(
    iu3da_t angle
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(angle, IUF_ERR_VALUE);
    free(angle);
    return IUF_E_OK;
}

// operations
int iuf3DAngleCompare
(
    iu3da_t reference,
    iu3da_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->theta, actual->theta ) == IUF_FALSE ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->phi, actual->phi ) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}

int iuf3DAngleSave
(
    iu3da_t angle,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(angle, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    hid_t group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_ANGLE, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_ANGLE_THETA, &(angle->theta), 1);
    status |= iufHdf5WriteFloat(group_id, IUF_INPUTFILE_PATH_ANGLE_PHI, &(angle->phi), 1);
    status |= H5Gclose(group_id );
    return status;
}

iu3da_t iuf3DAngleLoad
(
    hid_t handle
)
{
    int status=0;
    float theta,phi;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DA_INVALID);
	hid_t group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_ANGLE, H5P_DEFAULT);
    status = iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_ANGLE_THETA, &theta);
    status |= iufHdf5ReadFloat(group_id, IUF_INPUTFILE_PATH_ANGLE_PHI, &phi);
	H5Gclose(group_id);

    if (status<0)
        return IU3DA_INVALID;

    return iuf3DAngleCreate(theta,phi);
}

