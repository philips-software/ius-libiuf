
//
// Created by nlv09165 on 31/08/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusFile.h>

struct IusFile
{
    int intParam;
    float floatParam;
    iuhn_t historyTree;
} ;

// ADT
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
    return IUS_TRUE;
}

iufi_t iusFileOpen
(
    char *pFilename
)
{
    if( pFilename == NULL ) return IUFI_INVALID;
    return IUFI_INVALID;
}


// Getters
iuhn_t iusFileGetHistoryTree
(
    iufi_t iusFile
)
{
    if (iusFile == NULL) return IUHN_INVALID;
    return iusFile->historyTree;
}

const char *iusFileGetType
(
    iufi_t iusFile
)
{
    if (iusFile == NULL) return NULL;
    return iusHistoryNodeGetType(iusFile->historyTree);
}
