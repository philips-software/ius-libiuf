
//
// Created by nlv09165 on 31/08/2018.
//
#include <ius.h>
#include <iusHistoryNodePrivate.h>
#include <iusInputFilePrivate.h>
#include <memory.h>
#include <string.h>

struct IusFile
{
    iuhn_t history;
    hid_t handle;                         /**< file handle */
} ;

// ADT
int iusFileDelete
(
    iuf_t iusFile
)
{
    int status = IUS_ERR_VALUE;
    if(iusFile != NULL)
    {
        if( strcmp( iusHistoryNodeGetType(iusFile->history), IUS_INPUT_TYPE ) == 0 )
        {
            iuifi_t instance = iusHistoryNodeGetInstanceData(iusFile->history);
            iusInputFileInstanceDelete(instance);
        }
        iusHistoryNodeDelete(iusFile->history);
        free(iusFile);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusFileCompare
(
    iuf_t reference,
    iuf_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    return IUS_TRUE;
}

iuf_t iusFileLoad
(
    char *pFilename
)
{
    if (pFilename == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_FILENAME, "pFilename argument is NULL");
        return IUFI_INVALID;
    }

    iuf_t file = calloc(1,sizeof(IusFile));

    if (file == IUFI_INVALID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "unable to allocate IusFile ADT");
        return IUFI_INVALID;
    }

    // open  Hdf5 file using default properties.
    file->handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (file->handle <=0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_FILENAME, "unable to open file '%s'\n", pFilename );
        return IUFI_INVALID;
    }

    file->history = iusHistoryNodeLoad(file->handle);
    if( strcmp( iusHistoryNodeGetType(file->history), IUS_INPUT_TYPE ) == 0 )
    {
        iusInputFileSetFilename((iuif_t)file->history,pFilename);
    }

    return file;
}


// Getters
iuhn_t iusFileGetHistoryTree
(
    iuf_t iusFile
)
{
    if (iusFile == NULL) return IUHN_INVALID;
    return iusFile->history;
}

const char *iusFileGetType
(
    iuf_t iusFile
)
{
    if (iusFile == NULL) return NULL;
    return iusHistoryNodeGetType(iusFile->history);
}

// Setters
int iusFileSetHistoryTree
(
    iuf_t iusFile,
    iuhn_t history
)
{
    if (iusFile == NULL) return IUS_ERR_VALUE;
    if (history == NULL) return IUS_ERR_VALUE;
    iusFile->history = history;
    return IUS_E_OK;
}
