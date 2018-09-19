//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusFrameListImp.h>
#include <include/iusHDF5.h>
#include <include/iusFrameImp.h>

// ADT
struct IusFrameList
{
    int count;
    iuf_t *   pFrames ;
} ;

// ADT

iufl_t iusFrameListCreate
(
    int numFrames
)
{
    iufl_t list = calloc(1, sizeof(IusFrameList));
    if(list!=NULL)
    {
        list->count = numFrames;
        list->pFrames = (iuf_t *) calloc((size_t)numFrames, sizeof(iuf_t));
        if( list->pFrames == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusFrameListDelete
(
    iufl_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusFrameListCompare
(
    iufl_t reference,
    iufl_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusFrameCompare( reference->pFrames[index], actual->pFrames[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusFrameListGetSize
(
    iufl_t list
)
{
    return list->count;
}

iuf_t iusFrameListGet
(
    iufl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pFrames[index];
}

int iusFrameListSet
(
    iufl_t list,
    iuf_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->pFrames[index] = member;
    return IUS_E_OK;
}


#define FRAMELISTFMT "%s/Frame[%d]"
#define FRAMELISTSIZEFMT "%s/Size"

iufl_t iusFrameListLoad
(
    hid_t handle,
    const char *parentPath
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numPatterns,i;
    sprintf(path, FRAMELISTSIZEFMT, parentPath);
    int status = iusHdf5ReadInt(handle, path, &(numPatterns));
    if(status!=0) return IUFL_INVALID;

    iufl_t frameList = iusFrameListCreate(numPatterns);
    iuf_t sourceElement;

    // Load patterns
    for (i=0;i < numPatterns;i++)
    {
        sprintf(path, FRAMELISTFMT, parentPath, i);
        sourceElement = iusFrameLoad(handle, path);
        if(sourceElement == IUF_INVALID)
        {
            break;
        }
        iusFrameListSet(frameList,sourceElement,i);
    }
    return frameList;
}

IUS_BOOL iusFrameListFull
(
    iufl_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->count;i++)
    {
        if(list->pFrames[i] == IUF_INVALID)
        {
            isFull = IUS_FALSE;
            break;
        }
    }
    return isFull;
}


int iusFrameListSave
(
    iufl_t list,
    const char *parentPath,
    hid_t handle
)
{
    int status=0;
    int i,size;
    char path[IUS_MAX_HDF5_PATH];

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(parentPath == NULL || handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(iusFrameListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;

    hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    iuf_t sourceElement;
    size = iusFrameListGetSize(list);
    sprintf(path, FRAMELISTSIZEFMT, parentPath);
    status |= iusHdf5WriteInt(handle, path, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = iusFrameListGet(list,i);
        if(sourceElement == IUF_INVALID) continue;

        sprintf(path, FRAMELISTFMT, parentPath, i);
        status = iusFrameSave(sourceElement,path,group_id);
        if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id );
    return status;
}