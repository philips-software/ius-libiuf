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
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
} ;


// ADT
iu2dps_t ius2DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle
)
{
    if ( numLocations <= 0 ) return  NULL;
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc((size_t)numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->fNumber = fNumber;
    return created;
}

int ius2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DParametricSource != NULL)
    {
        free(ius2DParametricSource->pLocations);
        free(ius2DParametricSource);
        status = IUS_E_OK;
    }
    return status;
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
    if (IUS_EQUAL_FLOAT(reference->startAngle, actual->startAngle) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->angularDelta, actual->angularDelta) == IUS_FALSE ) return IUS_FALSE;
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
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->fNumber;
}

float ius2DParametricSourceGetAngularDelta
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->angularDelta;
}

float ius2DParametricSourceGetStartAngle
(
    iu2dps_t ius2DParametricSource
)
{
    if( ius2DParametricSource == NULL  ) return NAN;
    return ius2DParametricSource->startAngle;
}

int ius2DParametricSourceSetFNumber
(
    iu2dps_t ius2DParametricSource,
    float FNumber
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->fNumber = FNumber;
    return IUS_E_OK;
}

int ius2DParametricSourceSetAngularDelta
(
    iu2dps_t ius2DParametricSource,
    float angularDelta
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->angularDelta = angularDelta;
    return IUS_E_OK;
}

int ius2DParametricSourceSetStartAngle
(
    iu2dps_t ius2DParametricSource,
    float startAngle
)
{
    if (ius2DParametricSource == NULL) return IUS_ERR_VALUE;
    ius2DParametricSource->startAngle = startAngle;
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

    // Base
    status = iusBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(source->fNumber), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(source->angularDelta), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(source->startAngle), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(source->numLocations),1);
    return status;
}


iu2dps_t ius2DParametricSourceLoad
(
    hid_t handle
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    int numLocations;
    iu2dps_t  source;

    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(angularDelta));
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(startAngle));
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_SOURCE_LISTSIZE, &(numLocations));
    if (status < 0)
        return NULL;

    source = ius2DParametricSourceCreate(numLocations,fNumber,angularDelta,startAngle);
    return source;
}
