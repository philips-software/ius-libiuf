#include <iuf.h>
#include <iufHistoryNodePrivate.h>
#include <iufInputFilePrivate.h>
#include <memory.h>
#include <string.h>

struct IufFile
{
    iuhn_t history;
    hid_t handle;                         /**< file handle */
} ;

// ADT
int iufFileDelete
(
    iuf_t file
)
{
    if (file == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_FILENAME, "iufFile argument is NULL");
        return IUF_ERR_VALUE;
    }

    if( strcmp( iufHistoryNodeGetType(file->history), IUF_INPUT_TYPE ) == 0 )
    {
        iuifi_t instance = iufHistoryNodeGetInstanceData(file->history);
        iufInputFileInstanceDelete(instance);
    }
    iufHistoryNodeDelete(file->history);
    free(file);
    return IUF_E_OK;
}


// operations
int iufFileCompare
(
    iuf_t reference,
    iuf_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    return IUF_TRUE;
}

iuf_t iufFileLoad
(
    char *pFilename
)
{
    if (pFilename == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_FILENAME, "pFilename argument is NULL");
        return IUFI_INVALID;
    }

    iuf_t file = calloc(1,sizeof(IufFile));
    if (file == IUFI_INVALID)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "unable to allocate IufFile ADT");
        return IUFI_INVALID;
    }

    // open  Hdf5 file using default properties.
    file->handle = H5Fopen( pFilename, H5F_ACC_RDONLY, H5P_DEFAULT );
    if (file->handle <=0)
    {
        free(file);
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_FILENAME, "unable to open file '%s'", pFilename );
        return IUFI_INVALID;
    }

    file->history = iufHistoryNodeLoad(file->handle);
    if( strcmp( iufHistoryNodeGetType(file->history), IUF_INPUT_TYPE ) == 0 )
    {
        iufInputFileSetFilename((iuif_t)file->history,pFilename);
    }

    return file;
}


// Getters
iuhn_t iufFileGetHistoryTree
(
    iuf_t file
)
{
    if (file == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "iufFile");
        return IUHN_INVALID;
    }
    return file->history;
}

const char *iufFileGetType
(
    iuf_t file
)
{
    if (file == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "iufFile");
    }
    return iufHistoryNodeGetType(file->history);
}

// Setters
int iufFileSetHistoryTree
(
    iuf_t file,
    iuhn_t history
)
{
    if (file == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "iufFile");
        return IUF_ERR_VALUE;
    }
    if (history == NULL)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_NULL_VALUE, "history");
        return IUF_ERR_VALUE;
    }

    file->history = history;
    return IUF_E_OK;
}
