
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusHL2DParametricSource.h"

struct Ius2DParametricSource
{
    int intParam;
    float floatParam;
} ;

// ADT

iu2dps_t iusHL2DParametricSourceCreate
(
    int intParam,
    float floatParam
)
{
    if( intParam < 0 ) return IU2DPS_INVALID;
    if( floatParam <= 0.0f ) return IU2DPS_INVALID;
    iu2dps_t created = calloc(1,sizeof(Ius2DParametricSource));
    created->intParam = intParam;
    created->floatParam = floatParam;
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
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHL2DParametricSourceGetIntParam
(
    iu2dps_t ius2DParametricSource
)
{
    return ius2DParametricSource->intParam;
}

float iusHL2DParametricSourceGetFloatParam
(
    iu2dps_t ius2DParametricSource
)
{
    return ius2DParametricSource->floatParam;
}

// Setters
int iusHL2DParametricSourceSetFloatParam
(
    iu2dps_t ius2DParametricSource,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius2DParametricSource != NULL)
    {
        ius2DParametricSource->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHL2DParametricSourceSetIntParam
(
    iu2dps_t ius2DParametricSource,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius2DParametricSource != NULL)
    {
        ius2DParametricSource->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
