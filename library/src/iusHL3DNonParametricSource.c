
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusHL3DNonParametricSource.h"

struct Ius3DNonParametricSource
{
    int intParam;
    float floatParam;
} ;

// ADT

iu3dnps_t iusHL3DNonParametricSourceCreate
(
    int intParam,
    float floatParam
)
{
    if( intParam < 0 ) return IU3DNPS_INVALID;
    if( floatParam <= 0.0f ) return IU3DNPS_INVALID;
    iu3dnps_t created = calloc(1,sizeof(Ius3DNonParametricSource));
    created->intParam = intParam;
    created->floatParam = floatParam;
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
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHL3DNonParametricSourceGetIntParam
(
    iu3dnps_t ius3DNonParametricSource
)
{
    return ius3DNonParametricSource->intParam;
}

float iusHL3DNonParametricSourceGetFloatParam
(
    iu3dnps_t ius3DNonParametricSource
)
{
    return ius3DNonParametricSource->floatParam;
}

// Setters
int iusHL3DNonParametricSourceSetFloatParam
(
    iu3dnps_t ius3DNonParametricSource,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius3DNonParametricSource != NULL)
    {
        ius3DNonParametricSource->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHL3DNonParametricSourceSetIntParam
(
    iu3dnps_t ius3DNonParametricSource,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius3DNonParametricSource != NULL)
    {
        ius3DNonParametricSource->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
