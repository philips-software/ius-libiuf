
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <iuf.h>
#include <iufSourcePrivate.h>
#include <iufPositionPrivate.h>


struct Iuf3DParametricSource
{
    struct IufSource base;
    int numThetaLocations;
	int numPhiLocations;
    struct Iuf3DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;       /**< angle in [rad] between sources */
    float startTheta;       /**< offset angle in [rad] */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< offset angle in [rad] */
} ;

// ADT
iu3dps_t iuf3DParametricSourceCreate
(
	int numThetaLocations,
	int numPhiLocations,
	float fNumber,
	float deltaTheta,
	float startTheta,
	float deltaPhi,
	float startPhi
)
{
    IUF_ERR_EVAL_N_RETURN(numThetaLocations <= 0, IU3DPS_INVALID);
    IUF_ERR_EVAL_N_RETURN(numPhiLocations <= 0, IU3DPS_INVALID);
	int numLocations = numThetaLocations * numPhiLocations;
	iu3dps_t created = calloc(1,sizeof(Iuf3DParametricSource));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf3DParametricSource, IU3DPS_INVALID);

    created->pLocations = (struct Iuf3DPosition *) calloc((size_t)numLocations, sizeof(Iuf3DPosition));
    if( created->pLocations == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IU3DPS_INVALID;
    }

    created->base.type = IUF_3D_PARAMETRIC_SOURCE;
    created->numThetaLocations = numThetaLocations;
	created->numPhiLocations = numPhiLocations;
    created->deltaTheta = deltaTheta;
	created->deltaPhi   = deltaPhi;
	created->startTheta = startTheta;
    created->startPhi = startPhi;
    created->fNumber = fNumber;
    return created;
}

int iuf3DParametricSourceDelete
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUF_E_OK;
}


// operations
int iuf3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
)
{
    if (reference == actual ) return IUF_TRUE;
    if (reference == NULL || actual == NULL ) return IUF_FALSE;
    if (reference->numThetaLocations != actual->numThetaLocations) return IUF_FALSE;
	if (reference->numPhiLocations != actual->numPhiLocations) return IUF_FALSE;
    if (iufBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUF_FALSE ) return IUF_FALSE;
    int i;
    for( i = 0; i < reference->numThetaLocations * reference->numPhiLocations; i++ )
    {
        if( iuf3DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUF_FALSE )
        {
            return IUF_FALSE;
        }
    }
    return IUF_TRUE;
}

// Getters
float iuf3DParametricSourceGetFNumber
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->fNumber;
}

float iuf3DParametricSourceGetDeltaTheta
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaTheta;
}

float iuf3DParametricSourceGetStartTheta
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startTheta;
}

float iuf3DParametricSourceGetDeltaPhi
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaPhi;
}

float iuf3DParametricSourceGetStartPhi
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startPhi;
}

int iuf3DParametricSourceGetNumThetaLocations
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numThetaLocations;
}

int iuf3DParametricSourceGetNumPhiLocations
(
    iu3dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numPhiLocations;
}

int iuf3DParametricSourceSetFNumber
(
    iu3dps_t source,
    float FNumber
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->fNumber = FNumber;
    return IUF_E_OK;
}

int iuf3DParametricSourceSetDeltaTheta
(
    iu3dps_t source,
    float deltaTheta
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUF_E_OK;
}

int iuf3DParametricSourceSetStartTheta
(
    iu3dps_t source,
    float startTheta
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->startTheta = startTheta;
    return IUF_E_OK;
}

int iuf3DParametricSourceSetDeltaPhi
(
    iu3dps_t source,
    float deltaPhi
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->deltaPhi = deltaPhi;
    return IUF_E_OK;
}

int iuf3DParametricSourceSetStartPhi
(
    iu3dps_t source,
    float startPhi
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->startPhi = startPhi;
    return IUF_E_OK;
}



// serialization

int iuf3DParametricSourceSave
(
    iu3dps_t source,
    hid_t handle
)
{
    int status=0;

    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    // Base
    status = iufBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_FNUMBER,           &(source->fNumber),           1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_DELTATHETA,        &(source->deltaTheta),        1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_STARTTHETA,        &(source->startTheta),        1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_DELTAPHI,          &(source->deltaPhi),          1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_STARTPHI,          &(source->startPhi),          1);
    status |= iufHdf5WriteInt(   handle, IUF_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS,   &(source->numPhiLocations),   1);
	status |= iufHdf5WriteInt(   handle, IUF_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(source->numThetaLocations), 1);

    return status;
}


iu3dps_t iuf3DParametricSourceLoad
(
    hid_t handle
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;       /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< angle in [rad] between sources */
    int numThetaLocations;
	int numPhiLocations;
    iu3dps_t  source;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DPS_INVALID);

    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_SOURCE_DELTAPHI, &(deltaPhi));
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_SOURCE_STARTPHI, &(startPhi));
	status |= iufHdf5ReadInt( handle, IUF_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(numThetaLocations));
	status |= iufHdf5ReadInt( handle, IUF_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS, &(numPhiLocations));
    if (status < 0)
        return IU3DPS_INVALID;

    source = iuf3DParametricSourceCreate(numThetaLocations, numPhiLocations, fNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    return source;
}
