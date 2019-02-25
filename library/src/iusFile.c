
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
    iuf_t file
)
{
    if (file == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_FILENAME, "iusFile argument is NULL");
        return IUS_ERR_VALUE;
    }

    if( strcmp( iusHistoryNodeGetType(file->history), IUS_INPUT_TYPE ) == 0 )
    {
        iuifi_t instance = iusHistoryNodeGetInstanceData(file->history);
        iusInputFileInstanceDelete(instance);
    }
    iusHistoryNodeDelete(file->history);
    free(file);
    return IUS_E_OK;
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
        free(file);
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_FILENAME, "unable to open file '%s'", pFilename );
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
    iuf_t file
)
{
    if (file == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "iusFile");
        return IUHN_INVALID;
    }
    return file->history;
}

const char *iusFileGetType
(
    iuf_t file
)
{
    if (file == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "iusFile");
    }
    return iusHistoryNodeGetType(file->history);
}

// Setters
int iusFileSetHistoryTree
(
    iuf_t file,
    iuhn_t history
)
{
    if (file == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "iusFile");
        return IUS_ERR_VALUE;
    }
    if (history == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "history");
        return IUS_ERR_VALUE;
    }

    file->history = history;
    return IUS_E_OK;
}
