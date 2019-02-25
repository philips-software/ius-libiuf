//
// Created by nlv09165 on 30/07/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusSourcePrivate.h>
#include <iusPositionPrivate.h>

// ADT
ius_t ius2DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float deltaTheta,
    float startTheta
)
{
    IUS_ERR_EVAL_N_RETURN(numLocations <= 0, IUS_INVALID);
    ius_t created = calloc(1,sizeof(IusSource));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusSource, IUS_INVALID);
    created->locations2D = (struct Ius2DPosition *) calloc((size_t)numLocations, sizeof(Ius2DPosition));
    if( created->locations2D == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pLocations member");
        free(created);
        return NULL;
    }

    created->type = IUS_2D_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    created->deltaTheta = deltaTheta;
    created->startTheta = startTheta;
    created->fNumber = fNumber;
    return created;
}

int ius2DParametricSourceDelete
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    free(source->locations2D);
    free(source);
    return IUS_E_OK;
}


// operations
int ius2DParametricSourceCompare
(
    ius_t reference,
    ius_t actual
)
{
    if (reference == actual ) return IUS_TRUE;
    if (reference == NULL || actual == NULL ) return IUS_FALSE;
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=reference->type,IUS_FALSE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=actual->type,IUS_FALSE);
    if (reference->numLocations != actual->numLocations) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startTheta, actual->startTheta) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaTheta, actual->deltaTheta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
    {
        if( ius2DPositionCompare(&reference->locations2D[i],&actual->locations2D[i]) == IUS_FALSE )
        {
            return IUS_FALSE;
        }
    }
    return IUS_TRUE;
}

// Getters
float ius2DParametricSourceGetFNumber
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->fNumber;
}

float ius2DParametricSourceGetDeltaTheta
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->deltaTheta;
}

float ius2DParametricSourceGetStartTheta
(
    ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, NAN);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,NAN);
    return source->startTheta;
}

int ius2DParametricSourceGetNumLocations
(
        ius_t source
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, -1);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,-1);

    return source->numLocations;
}

int ius2DParametricSourceSetFNumber
(
    ius_t source,
    float FNumber
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->fNumber = FNumber;
    return IUS_E_OK;
}

int ius2DParametricSourceSetDeltaTheta
(
    ius_t source,
    float deltaTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->deltaTheta = deltaTheta;
    return IUS_E_OK;
}

int ius2DParametricSourceSetStartTheta
(
    ius_t source,
    float startTheta
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
    source->startTheta = startTheta;
    return IUS_E_OK;
}


// serialization
int ius2DParametricSourceSave
(
    ius_t source,
    hid_t handle
)
{
    int status=0;
    IUS_ERR_CHECK_NULL_N_RETURN(source, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(IUS_2D_PARAMETRIC_SOURCE!=source->type,IUS_ERR_VALUE);
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


ius_t ius2DParametricSourceLoad
(
    hid_t handle
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float deltaTheta;     /**< angle in [rad] between sources */
    float startTheta;       /**< angle in [rad] between sources */
    int numLocations;
    ius_t  source;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_INVALID);

    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(numLocations));
    if (status < 0)
        return IUS_INVALID;

    source = ius2DParametricSourceCreate(numLocations,fNumber,deltaTheta,startTheta);
    return source;
}

