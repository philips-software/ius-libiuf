
//
// Created by nlv09165 on 31/08/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusFile.h>
#include <hdf5.h>
#include <include/iusHistoryNodePrivate.h>
#include <memory.h>
#include <include/iusInputFile.h>
#include <include/iusInputFilePrivate.h>

struct IusFile
{
    iuhn_t history;
    hid_t handle;                         /**< file handle */
    void *instance_data;
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

iufi_t iusFileLoad
(
    char *pFilename
)
{
    if (pFilename == NULL)
    {
        fprintf(stderr, "iusFileLoad: Input arguments can not be NULL \n");
        return IUFI_INVALID;
    }

    iufi_t file = calloc(1,sizeof(IusFile));

    // check calloc
    if (file == IUFI_INVALID)
    {
        fprintf(stderr, "iusFileLoad: calloc of instance failed\n");
        return IUFI_INVALID;
    }

    // open  Hdf5 file using default properties.
    file->handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (file->handle <=0)
    {
        fprintf( stderr, "iusFileLoad: could not open file: %s\n", pFilename );
        return IUFI_INVALID;
    }

    file->history = iusHistoryNodeLoad(file->handle);

    return file;
}


// Getters
iuhn_t iusFileGetHistoryTree
(
    iufi_t iusFile
)
{
    if (iusFile == NULL) return IUHN_INVALID;
    return iusFile->history;
}

const char *iusFileGetType
(
    iufi_t iusFile
)
{
    if (iusFile == NULL) return NULL;
    return iusHistoryNodeGetType(iusFile->history);
}

// Setters
int iusFileSetHistoryTree
(
    iufi_t iusFile,
    iuhn_t history
)
{
    if (iusFile == NULL) return IUS_ERR_VALUE;
    if (history == NULL) return IUS_ERR_VALUE;
    iusFile->history = history;
    return IUS_E_OK;
}
