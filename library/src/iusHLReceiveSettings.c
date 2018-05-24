
//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusHLReceiveSettings.h"

struct IusReceiveSettings
{
    int intParam;
    float floatParam;
} ;

// ADT

iurs_t iusHLReceiveSettingsCreate
(
int intParam,
float floatParam
)
{
    if( intParam < 0 ) return IURS_INVALID;
    if( floatParam <= 0.0f ) return IURS_INVALID;
    iurs_t created = calloc(1,sizeof(IusReceiveSettings));
    created->intParam = intParam;
    created->floatParam = floatParam;
    return created;
}

int iusHLReceiveSettingsDelete
(
iurs_t iusReceiveSettings
)
{
    int status = IUS_ERR_VALUE;
    if(iusReceiveSettings != NULL)
    {
        free(iusReceiveSettings);
        iusReceiveSettings = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLReceiveSettingsCompare
(
iurs_t reference,
iurs_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHLReceiveSettingsGetIntParam
(
    iurs_t iusReceiveSettings
)
{
    return iusReceiveSettings->intParam;
}

float iusHLReceiveSettingsGetFloatParam
(
    iurs_t iusReceiveSettings
)
{
    return iusReceiveSettings->floatParam;
}

// Setters
int iusHLReceiveSettingsSetFloatParam
(
    iurs_t iusReceiveSettings,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusReceiveSettings != NULL)
    {
        iusReceiveSettings->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHLReceiveSettingsSetIntParam
(
    iurs_t iusReceiveSettings,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusReceiveSettings != NULL)
    {
        iusReceiveSettings->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
