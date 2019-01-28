//
// Created by nlv09165 on 30/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusSourcePrivate.h>
#include <iusPositionPrivate.h>

struct Ius2DParametricSource
{
    struct IusSource base;
    int numLocations;
    struct Ius2DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
} ;


// ADT
iu2dps_t ius2DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta
)
{
    IUS_ERR_EVAL_N_RETURN(numLocations <= 0, IU2DPS_INVALID);
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, Ius2DParametricSource, IU2DPS_INVALID);
    created->pLocations = (struct Ius2DPosition *) calloc((size_t)numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    created->deltaTheta = deltaTheta;
    created->startTheta = startTheta;
    created->fNumber = fNumber;
    return created;
}

int ius2DParametricSourceDelete
(
    iu2dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    free(source->pLocations);
    free(source);
    return IUS_E_OK;
}


// operations
int ius2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    if (reference->numLocations != actual->numLocations) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
    {
        if( ius2DPositionCompare(&reference->pLocations[i],&actual->pLocations[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius2DParametricSourceGetFNumber
(
    iu2dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->fNumber;
}

float ius2DParametricSourceGetDeltaTheta
(
    iu2dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->deltaTheta;
}

float ius2DParametricSourceGetStartTheta
(
    iu2dps_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    return source->startTheta;
}

int ius2DParametricSourceSetFNumber
(
    iu2dps_t source,
    float FNumber
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->fNumber = FNumber;
    return IUS_E_OK;
}

int ius2DParametricSourceSetDeltaTheta
(
    iu2dps_t source,
    float deltaTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUS_E_OK;
}

int ius2DParametricSourceSetStartTheta
(
    iu2dps_t source,
    float startTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    source->startTheta = startTheta;
    return IUS_E_OK;
}


// serialization
int ius2DParametricSourceSave
(
    iu2dps_t source,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);

    // Base
    status = iusBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(source->fNumber), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(source->deltaTheta), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(source->startTheta), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(source->numLocations),1);
    return status;
}


iu2dps_t ius2DParametricSourceLoad
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

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IU2DPS_INVALID);

    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(numLocations));
    if (status < 0)
        return IU2DPS_INVALID;

    source = ius2DParametricSourceCreate(numLocations,fNumber,deltaTheta,startTheta);
    return source;
}

