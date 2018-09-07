
//
// Created by nlv09165 on 31/08/2018.
//
#include <stdlib.h>
#define IUSLIBRARY_IMPLEMENTATION

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include "include/iusFile.h"

struct IusFile
{
    int intParam;
    float floatParam;
} ;

// ADT

iufi_t iusFileCreate
(
    int intParam,
    float floatParam
)
{
    if( intParam < 0 ) return IUFI_INVALID;
    if( floatParam <= 0.0f ) return IUFI_INVALID;
    iufi_t created = calloc(1,sizeof(IusFile));
    created->intParam = intParam;
    created->floatParam = floatParam;
    return created;
}

int iusFileDelete
(
    iufi_t iusFile
)
{
    int status = IUS_ERR_VALUE;
    if(iusFile != NULL)
    {
        free(iusFile);
        iusFile = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusFileCompare
(
    iufi_t reference,
    iufi_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->intParam != actual->intParam ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->floatParam, actual->floatParam ) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusFileGetIntParam
(
    iufi_t iusFile
)
{
    return iusFile->intParam;
}

float iusFileGetFloatParam
(
    iufi_t iusFile
)
{
    return iusFile->floatParam;
}

// Setters
int iusFileSetFloatParam
(
    iufi_t iusFile,
    float floatParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusFile != NULL)
    {
        iusFile->floatParam = floatParam;
        status = IUS_E_OK;
    }
    return status;
}


int iusFileSetIntParam
(
    iufi_t iusFile,
    int intParam
)
{
    int status = IUS_ERR_VALUE;

    if(iusFile != NULL)
    {
        iusFile->intParam = intParam;
        status = IUS_E_OK;
    }
    return status;
}
