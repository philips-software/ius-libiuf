
//
// Created by nlv09165 on 13/07/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <math.h>
#include <string.h>

#include "include/iusHLPattern.h"

struct IusPattern
{
    char* pLabel;
    float timeInFrame;
    char* pPulseLabel;
    char* pSourceLabel;
    char* pChannelMapLabel;
    char* pApodizationLabel;
    char* pReceiveSettingsLabel;
} ;

// ADT

iup_t iusHLPatternCreate
(
    char* pLabel,
    float timeInFrame
)
{
    if( pLabel == NULL ) return IUP_INVALID;
    if( strcmp(pLabel,"") == 0 ) return IUP_INVALID;
    if( timeInFrame <= 0.0f ) return IUP_INVALID;
    iup_t created = calloc(1, sizeof(IusPattern));
    created->pLabel = pLabel;
    created->timeInFrame = timeInFrame;
    return created;
}

int iusHLPatternDelete
(
    iup_t iusPattern
)
{
    int status = IUS_ERR_VALUE;
    if(iusPattern != IUP_INVALID)
    {
        free(iusPattern);
        iusPattern = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLPatternCompare
(
    iup_t reference,
    iup_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == IUP_INVALID || actual == IUP_INVALID ) return IUS_FALSE;
    if( reference->pLabel != actual->pLabel ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->timeInFrame, actual->timeInFrame ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusHLPatternGetIntParam
(
    iup_t iusPattern
)
{
    if(iusPattern == IUP_INVALID) return -1;
    return iusPattern->pLabel;
}

float iusHLPatternGetFloatParam
(
    iup_t iusPattern
)
{
    if(iusPattern == IUP_INVALID) return NAN;
    return iusPattern->timeInFrame;
}

// Setters
int iusHLPatternSetFloatParam
(
    iup_t iusPattern,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusPattern != IUP_INVALID)
    {
        iusPattern->timeInFrame = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusHLPatternSetIntParam
(
    iup_t iusPattern,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusPattern != IUP_INVALID)
    {
        iusPattern->pLabel = intParam;
        status = IUS_E_OK;
    }
    return status;
}
