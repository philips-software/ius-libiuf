
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION


#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <memory.h>

#include <include/iusHLSourceImp.h>
#include <include/iusHLPositionImp.h>
#include "include/iusHL2DNonParametricSource.h"

struct Ius2DNonParametricSource
{
    struct IusSource base;
    int locationCount;

    struct Ius2DPosition *pLocations;
} ;

// ADT

iu2dnps_t iusHL2DNonParametricSourceCreate
(
    char *pLabel,
    int numLocations
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu2dnps_t created = calloc(1,sizeof(Ius2DNonParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius2DPosition *) calloc(numLocations, sizeof(Ius2DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_2D_NON_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->locationCount = numLocations;
    return created;
}

int iusHL2DNonParametricSourceDelete
(
    iu2dnps_t ius2DNonParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius2DNonParametricSource != NULL)
    {
        free(ius2DNonParametricSource);
        ius2DNonParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL2DNonParametricSourceCompare
(
    iu2dnps_t reference,
    iu2dnps_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    return IUS_FALSE;
}

// Getters


// Setters
