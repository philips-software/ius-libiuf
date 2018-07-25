
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusHL2DNonParametricSource.h"

struct Ius2DNonParametricSource
{
    int intParam;
    float floatParam;
} ;

// ADT

iu2dnps_t iusHL2DNonParametricSourceCreate
(
    int intParam,
    float floatParam
)
{
    if( intParam < 0 ) return IU2DNPS_INVALID;
    if( floatParam <= 0.0f ) return IU2DNPS_INVALID;
    iu2dnps_t created = calloc(1,sizeof(Ius2DNonParametricSource));
    created->intParam = intParam;
    created->floatParam = floatParam;
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
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHL2DNonParametricSourceGetIntParam
(
    iu2dnps_t ius2DNonParametricSource
)
{
    return ius2DNonParametricSource->intParam;
}

float iusHL2DNonParametricSourceGetFloatParam
(
    iu2dnps_t ius2DNonParametricSource
)
{
    return ius2DNonParametricSource->floatParam;
}

// Setters
int iusHL2DNonParametricSourceSetFloatParam
(
    iu2dnps_t ius2DNonParametricSource,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius2DNonParametricSource != NULL)
    {
        ius2DNonParametricSource->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHL2DNonParametricSourceSetIntParam
(
    iu2dnps_t ius2DNonParametricSource,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius2DNonParametricSource != NULL)
    {
        ius2DNonParametricSource->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
