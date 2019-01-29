
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusSourcePrivate.h>
#include <iusPositionPrivate.h>


struct Ius3DParametricSource
{
    struct IusSource base;
    int numThetaLocations;
	int numPhiLocations;
    struct Ius3DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;       /**< angle in [rad] between sources */
    float startTheta;       /**< offset angle in [rad] */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< offset angle in [rad] */
} ;

// ADT
iu3dps_t ius3DParametricSourceCreate
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
    IUS_ERR_EVAL_N_RETURN(numThetaLocations <= 0, IU3DPS_INVALID);
    IUS_ERR_EVAL_N_RETURN(numPhiLocations <= 0, IU3DPS_INVALID);
	int numLocations = numThetaLocations * numPhiLocations;
	iu3dps_t created = calloc(1,sizeof(Ius3DParametricSource));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius3DParametricSource, IU3DPS_INVALID);

    created->pLocations = (struct Ius3DPosition *) calloc((size_t)numLocations, sizeof(Ius3DPosition));
    if( created->pLocations == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IU3DPS_INVALID;
    }

    created->base.type = IUS_3D_PARAMETRIC_SOURCE;
    created->numThetaLocations = numThetaLocations;
	created->numPhiLocations = numPhiLocations;
    created->deltaTheta = deltaTheta;
	created->deltaPhi   = deltaPhi;
	created->startTheta = startTheta;
    created->startPhi = startPhi;
    created->fNumber = fNumber;
    return created;
}

int ius3DParametricSourceDelete
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUS_E_OK;
}


// operations
int ius3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->numThetaLocations != actual->numThetaLocations) return IUS_FALSE;
	if (reference->numPhiLocations != actual->numPhiLocations) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numThetaLocations * reference->numPhiLocations; i++ )
    {
        if( ius3DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius3DParametricSourceGetFNumber
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->fNumber;
}

float ius3DParametricSourceGetDeltaTheta
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaTheta;
}

float ius3DParametricSourceGetStartTheta
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startTheta;
}

float ius3DParametricSourceGetDeltaPhi
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaPhi;
}

float ius3DParametricSourceGetStartPhi
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startPhi;
}

int ius3DParametricSourceGetNumThetaLocations
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numThetaLocations;
}

int ius3DParametricSourceGetNumPhiLocations
(
    iu3dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numPhiLocations;
}

int ius3DParametricSourceSetFNumber
(
    iu3dps_t source,
    float FNumber
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->fNumber = FNumber;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaTheta
(
    iu3dps_t source,
    float deltaTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartTheta
(
    iu3dps_t source,
    float startTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->startTheta = startTheta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaPhi
(
    iu3dps_t source,
    float deltaPhi
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->deltaPhi = deltaPhi;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartPhi
(
    iu3dps_t source,
    float startPhi
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->startPhi = startPhi;
    return IUS_E_OK;
}



// serialization

int ius3DParametricSourceSave
(
    iu3dps_t source,
    hid_t handle
)
{
    int status=0;

    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    // Base
    status = iusBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER,           &(source->fNumber),           1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA,        &(source->deltaTheta),        1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA,        &(source->startTheta),        1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI,          &(source->deltaPhi),          1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI,          &(source->startPhi),          1);
    status |= iusHdf5WriteInt(   handle, IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS,   &(source->numPhiLocations),   1);
	status |= iusHdf5WriteInt(   handle, IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(source->numThetaLocations), 1);

    return status;
}


iu3dps_t ius3DParametricSourceLoad
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

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU3DPS_INVALID);

    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI, &(deltaPhi));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI, &(startPhi));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(numThetaLocations));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS, &(numPhiLocations));
    if (status < 0)
        return IU3DPS_INVALID;

    source = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, fNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    return source;
}
