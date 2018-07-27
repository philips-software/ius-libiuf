
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <include/iusHLSourceImp.h>
#include <include/iusHLPositionImp.h>
#include <memory.h>
#include "include/iusHL2DParametricSource.h"

struct Ius2DParametricSource
{
    struct IusSource base;
    struct Ius2DPosition *pLocations;

    float fNumber;          /**< distance in [m] of sources to transducer for POLAR */
    float angularDelta;     /**< angle in [rad] between sources */
    float startAngle;       /**< angle in [rad] between sources */
} ;

// ADT

iu2dps_t iusHL2DParametricSourceCreate
(
    char *pLabel,
    int numLocations,
    float fNumber,
    float angularDelta,
    float startAngle
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc(numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->base.locationCount = numLocations;
    created->angularDelta = angularDelta;
    created->startAngle = startAngle;
    created->fNumber = fNumber;
    return created;
}

int iusHL2DParametricSourceDelete
(
    iu2dps_t ius2DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DParametricSource != NULL)
    {
        free(ius2DParametricSource);
        ius2DParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DParametricSourceCompare
(
    iu2dps_t reference,
    iu2dps_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    return IUS_FALSE;
}

// Getters


// Setters
