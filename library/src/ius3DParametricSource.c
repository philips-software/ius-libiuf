
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

// ADT
ius_t ius3DParametricSourceCreate
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
    IUS_ERR_EVAL_N_RETURN(numThetaLocations <= 0, IUS_INVALID);
    IUS_ERR_EVAL_N_RETURN(numPhiLocations <= 0, IUS_INVALID);
	int numLocations = numThetaLocations * numPhiLocations;
	ius_t created = calloc(1,sizeof(IusSource));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusSource, IUS_INVALID);

    created->locations3D = (struct Ius3DPosition *) calloc((size_t)numLocations, sizeof(Ius3DPosition));
    if( created->locations3D == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return IUS_INVALID;
    }

    created->type = IUS_3D_PARAMETRIC_SOURCE;
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
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    free(source->locations3D);
    free(source);
    return IUS_E_OK;
}


// operations
int ius3DParametricSourceCompare
(
    ius_t reference,
    ius_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=reference->type,IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=actual->type,IUS_ERR_VALUE);
    if (reference->numThetaLocations != actual->numThetaLocations) return IUS_FALSE;
	if (reference->numPhiLocations != actual->numPhiLocations) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numThetaLocations * reference->numPhiLocations; i++ )
    {
        if( ius3DPositionCompare(&reference->locations3D[i],&actual->locations3D[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius3DParametricSourceGetFNumber
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type, NAN);
    return source->fNumber;
}

float ius3DParametricSourceGetDeltaTheta
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->deltaTheta;
}

float ius3DParametricSourceGetStartTheta
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->startTheta;
}

float ius3DParametricSourceGetDeltaPhi
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->deltaPhi;
}

float ius3DParametricSourceGetStartPhi
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->startPhi;
}

int ius3DParametricSourceGetNumThetaLocations
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,-1);
    return source->numThetaLocations;
}

int ius3DParametricSourceGetNumPhiLocations
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type, -1);
    return source->numPhiLocations;
}

int ius3DParametricSourceSetFNumber
(
    ius_t source,
    float FNumber
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->fNumber = FNumber;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaTheta
(
    ius_t source,
    float deltaTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartTheta
(
    ius_t source,
    float startTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->startTheta = startTheta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaPhi
(
    ius_t source,
    float deltaPhi
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->deltaPhi = deltaPhi;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartPhi
(
    ius_t source,
    float startPhi
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->startPhi = startPhi;
    return IUS_E_OK;
}



// serialization

int ius3DParametricSourceSave
(
    ius_t source,
    hid_t handle
)
{
    int status=0;

    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_3D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
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


ius_t ius3DParametricSourceLoad
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
    ius_t  source;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_INVALID);

    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI, &(deltaPhi));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI, &(startPhi));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(numThetaLocations));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS, &(numPhiLocations));
    if (status < 0)
        return IUS_INVALID;

    source = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, fNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    return source;
}
