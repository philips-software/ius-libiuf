//
// Created by nlv09165 on 30/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <iuf.h>
#include <iufSourcePrivate.h>
#include <iufPositionPrivate.h>

struct Iuf2DParametricSource
{
    struct IufSource base;
    int numLocations;
    struct Iuf2DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
} ;


// ADT
iu2dps_t iuf2DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta
)
{
    IUF_ERR_EVAL_N_RETURN(numLocations <= 0, IU2DPS_INVALID);
    iu2dps_t created = calloc(1,sizeof(Iuf2DParametricSource));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, Iuf2DParametricSource, IU2DPS_INVALID);
    created->pLocations = (struct Iuf2DPosition *) calloc((size_t)numLocations, sizeof(Iuf2DPosition));
    if( created->pLocations == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return NULL;
    }

    created->base.type = IUF_2D_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    created->deltaTheta = deltaTheta;
    created->startTheta = startTheta;
    created->fNumber = fNumber;
    return created;
}

int iuf2DParametricSourceDelete
(
    iu2dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUF_E_OK;
}


// operations
int iuf2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
)
{
    if (reference == actual ) return IUF_TRUE;
    if (reference == NULL || actual == NULL ) return IUF_FALSE;
    if (reference->numLocations != actual->numLocations) return IUF_FALSE;
    if (iufBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUF_FALSE ) return IUF_FALSE;
    if (IUF_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUF_FALSE ) return IUF_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
    {
        if( iuf2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUF_FALSE )
        {
            return IUF_FALSE;
        }
    }
    return IUF_TRUE;
}

// Getters
float iuf2DParametricSourceGetFNumber
(
    iu2dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->fNumber;
}

float iuf2DParametricSourceGetDeltaTheta
(
    iu2dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaTheta;
}

float iuf2DParametricSourceGetStartTheta
(
    iu2dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startTheta;
}

int iuf2DParametricSourceGetNumLocations
(
        iu2dps_t source
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, -1);
    return source->numLocations;
}

int iuf2DParametricSourceSetFNumber
(
    iu2dps_t source,
    float FNumber
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->fNumber = FNumber;
    return IUF_E_OK;
}

int iuf2DParametricSourceSetDeltaTheta
(
    iu2dps_t source,
    float deltaTheta
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUF_E_OK;
}

int iuf2DParametricSourceSetStartTheta
(
    iu2dps_t source,
    float startTheta
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    source->startTheta = startTheta;
    return IUF_E_OK;
}


// serialization
int iuf2DParametricSourceSave
(
    iu2dps_t source,
    hid_t handle
)
{
    int status=0;
    IUF_ERR_CHECK_NULL_N_RETURN(source, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);

    // Base
    status = iufBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_FNUMBER, &(source->fNumber), 1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_DELTATHETA, &(source->deltaTheta), 1);
    status |= iufHdf5WriteFloat( handle, IUF_INPUTFILE_PATH_SOURCE_STARTTHETA, &(source->startTheta), 1);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(source->numLocations),1);
    return status;
}


iu2dps_t iuf2DParametricSourceLoad
(
    hid_t handle
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
    int numLocations;
    iu2dps_t  source;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DPS_INVALID);

    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(numLocations));
    if (status < 0)
        return IU2DPS_INVALID;

    source = iuf2DParametricSourceCreate(numLocations,fNumber,deltaTheta,startTheta);
    return source;
}

