
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
    int numLocations;
    struct Ius3DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< angle in [rad] between sources */
} ;

// ADT
iu3dps_t ius3DParametricSourceCreate
(
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle,
    float deltaPhi,
    float startPhi
)
{
    if ( numLocations <= 0 ) return  NULL;
    iu3dps_t created = calloc(1,sizeof(Ius3DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius3DPosition *) calloc((size_t)numLocations, sizeof(Ius3DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_3D_PARAMETRIC_SOURCE;
    created->numLocations = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->deltaPhi = deltaPhi;
    created->startPhi = startPhi;
    created->fNumber = fNumber;
    return created;
}

int ius3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DParametricSource != NULL)
    {
        free(ius3DParametricSource->pLocations);
        free(ius3DParametricSource);
        status = IUS_E_OK;
    }
    return status;
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
    if (reference->numLocations != actual->numLocations) return IUS_FALSE;
    if (iusBaseSourceCompare((ius_t)reference, (ius_t)actual) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->fNumber, actual->fNumber) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startPhi, actual->startPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->deltaPhi, actual->deltaPhi) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->startAngle, actual->startAngle) == IUS_FALSE ) return IUS_FALSE;
    if (IUS_EQUAL_FLOAT(reference->angularDelta, actual->angularDelta) == IUS_FALSE ) return IUS_FALSE;
    int i;
    for( i = 0; i < reference->numLocations; i++ )
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
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->fNumber;
}

float ius3DParametricSourceGetAngularDelta
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->angularDelta;
}

float ius3DParametricSourceGetStartAngle
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->startAngle;
}

float ius3DParametricSourceGetDeltaPhi
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->deltaPhi;
}

float ius3DParametricSourceGetStartPhi
(
    iu3dps_t ius3DParametricSource
)
{
    if ( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->startPhi;
}

int ius3DParametricSourceSetFNumber
(
    iu3dps_t ius3DParametricSource,
    float FNumber
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->fNumber = FNumber;
    return IUS_E_OK;
}

int ius3DParametricSourceSetAngularDelta
(
    iu3dps_t ius3DParametricSource,
    float angularDelta
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->angularDelta = angularDelta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartAngle
(
    iu3dps_t ius3DParametricSource,
    float startAngle
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->startAngle = startAngle;
    return IUS_E_OK;
}

int ius3DParametricSourceSetDeltaPhi
(
    iu3dps_t ius3DParametricSource,
    float deltaPhi
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->deltaPhi = deltaPhi;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartPhi
(
    iu3dps_t ius3DParametricSource,
    float startPhi
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->startPhi = startPhi;
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

    // Base
    status = iusBaseSourceSave((ius_t)source, handle);

    // Parametric stuff
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(source->fNumber), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(source->angularDelta), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(source->startAngle), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI, &(source->deltaPhi), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI, &(source->startPhi), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(source->numLocations), 1);


    return status;
}


iu3dps_t ius3DParametricSourceLoad
(
    hid_t handle
)
{
    int status = 0;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
    float deltaPhi;         /**< angle in [rad] between sources */
    float startPhi;         /**< angle in [rad] between sources */
    int numLocations;
    iu3dps_t  source;

    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_ANGULARDELTA, &(angularDelta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTANGLE, &(startAngle));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI, &(deltaPhi));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI, &(startPhi));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMSOURCES, &(numLocations));
    if (status < 0)
        return NULL;

    source = ius3DParametricSourceCreate(numLocations,fNumber,angularDelta,startAngle,deltaPhi,startPhi);
    return source;
}
