
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
#include "include/iusHL3DNonParametricSource.h"
#include <include/iusHLPositionImp.h>

struct Ius3DNonParametricSource
{
    struct IusSource base;
    struct Ius3DPosition *pLocations;
} ;

// ADT
iu3dnps_t iusHL3DNonParametricSourceCreate
(
    char *pLabel,
    int numLocations
)
{
    if ( pLabel == NULL ) return NULL;
    if ( strcmp(pLabel,"") == 0 ) return NULL;
    if ( numLocations <= 0 ) return  NULL;
    iu3dnps_t created = calloc(1,sizeof(Ius3DNonParametricSource));
    if( created == NULL ) return NULL;

    created->pLocations = (struct Ius3DPosition *) calloc(numLocations, sizeof(Ius3DPosition));
    if( created->pLocations == NULL )
    {
        free(created);
        return NULL;
    }

    created->base.type = IUS_3D_NON_PARAMETRIC_SOURCE;
    created->base.label = strdup(pLabel);
    created->base.locationCount = numLocations;
    return created;
}

int iusHL3DNonParametricSourceDelete
(
    iu3dnps_t ius3DNonParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DNonParametricSource != NULL)
    {
        free(ius3DNonParametricSource);
        ius3DNonParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DNonParametricSourceCompare
(
    iu3dnps_t reference,
    iu3dnps_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    return IUS_FALSE;
}

// Getters


// Setters
