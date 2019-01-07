
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
	int numLocations = numThetaLocations * numPhiLocations;
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
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->fNumber;
}

float ius3DParametricSourceGetDeltaTheta
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->deltaTheta;
}

float ius3DParametricSourceGetStartTheta
(
    iu3dps_t ius3DParametricSource
)
{
    if( ius3DParametricSource == NULL  ) return NAN;
    return ius3DParametricSource->startTheta;
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

int ius3DParametricSourceSetDeltaTheta
(
    iu3dps_t ius3DParametricSource,
    float deltaTheta
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->deltaTheta = deltaTheta;
    return IUS_E_OK;
}

int ius3DParametricSourceSetStartTheta
(
    iu3dps_t ius3DParametricSource,
    float startTheta
)
{
    if (ius3DParametricSource == NULL) return IUS_ERR_VALUE;
    ius3DParametricSource->startTheta = startTheta;
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
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER,        &(source->fNumber), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA,     &(source->deltaTheta), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA,     &(source->startTheta), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI,       &(source->deltaPhi), 1);
    status |= iusHdf5WriteFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI,       &(source->startPhi), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS,   &(source->numPhiLocations), 1);
	status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(source->numThetaLocations), 1);

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

    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_FNUMBER, &(fNumber));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTATHETA, &(deltaTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTTHETA, &(startTheta));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_DELTAPHI, &(deltaPhi));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_SOURCE_STARTPHI, &(startPhi));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMTHETALOCATIONS, &(numThetaLocations));
	status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_SOURCE_NUMPHILOCATIONS, &(numPhiLocations));
    if (status < 0)
        return NULL;

    source = ius3DParametricSourceCreate(numThetaLocations, numPhiLocations, fNumber, deltaTheta, startTheta, deltaPhi, startPhi);
    return source;
}
