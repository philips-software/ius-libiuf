//
// Created by nlv09165 on 31/08/2018.
//
#include <ius.h>
#include <iusHistoryNodePrivate.h>
#include <iusInputFilePrivate.h>
#include <memory.h>

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
    int status = IUS_ERR_VALUE;
    if(file != NULL)
    {
        if( strcmp( iusHistoryNodeGetType(file->history), IUS_INPUT_TYPE ) == 0 )
        {
            iuifi_t instance = iusHistoryNodeGetInstanceData(file->history);
            iusInputFileInstanceDelete(instance);
        }
        iusHistoryNodeDelete(file->history);
        free(file);
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
        fprintf(stderr, "iusFileLoad: Input arguments can not be NULL \n");
        return IUFI_INVALID;
    }

    iuf_t file = calloc(1,sizeof(IusFile));

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
    if (file == NULL) return IUHN_INVALID;
    return file->history;
}

const char *iusFileGetType
(
    iuf_t file
)
{
    if (file == NULL) return NULL;
    return iusHistoryNodeGetType(file->history);
}

// Setters
int iusFileSetHistoryTree
(
    iuf_t file,
    iuhn_t history
)
{
    if (file == NULL) return IUS_ERR_VALUE;
    if (history == NULL) return IUS_ERR_VALUE;
    file->history = history;
    return IUS_E_OK;
}

