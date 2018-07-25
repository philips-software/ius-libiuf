
//
// Created by nlv09165 on 25/07/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusHL3DParametricSource.h"

struct Ius3DParametricSource
{
    int intParam;
    float floatParam;
} ;

// ADT

iu3dps_t iusHL3DParametricSourceCreate
(
    int intParam,
    float floatParam
)
{
    if( intParam < 0 ) return IU3DPS_INVALID;
    if( floatParam <= 0.0f ) return IU3DPS_INVALID;
    iu3dps_t created = calloc(1,sizeof(Ius3DParametricSource));
    created->intParam = intParam;
    created->floatParam = floatParam;
    return created;
}

int iusHL3DParametricSourceDelete
(
    iu3dps_t ius3DParametricSource
)
{
    int status = IUS_ERR_VALUE;
    if(ius3DParametricSource != NULL)
    {
        free(ius3DParametricSource);
        ius3DParametricSource = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHL3DParametricSourceCompare
(
    iu3dps_t reference,
    iu3dps_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHL3DParametricSourceGetIntParam
(
    iu3dps_t ius3DParametricSource
)
{
    return ius3DParametricSource->intParam;
}

float iusHL3DParametricSourceGetFloatParam
(
    iu3dps_t ius3DParametricSource
)
{
    return ius3DParametricSource->floatParam;
}

// Setters
int iusHL3DParametricSourceSetFloatParam
(
    iu3dps_t ius3DParametricSource,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius3DParametricSource != NULL)
    {
        ius3DParametricSource->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHL3DParametricSourceSetIntParam
(
    iu3dps_t ius3DParametricSource,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(ius3DParametricSource != NULL)
    {
        ius3DParametricSource->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
