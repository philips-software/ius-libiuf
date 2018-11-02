//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusFramePrivate.h>
#include <iusFrameListPrivate.h>


// ADT
struct IusFrameList
{
    int numFrames;
    iufr_t *   pFrames ;
    IUS_BOOL loadedFromFile;
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
        list->loadedFromFile = IUS_FALSE;
        list->numFrames = numFrames;
        list->pFrames = (iufr_t *) calloc((size_t)numFrames, sizeof(iufr_t));
        if( list->pFrames == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusFrameListDeleteFrames
(
    iufl_t list
)
{
    int index;
    int status = 0;
    if(list == NULL) return IUS_ERR_VALUE;
    for(index = 0 ; index < list->numFrames ; index++ )
    {
        status |= iusFrameDelete(list->pFrames[index]);
    }
    return status;
}

int iusFrameListDeepDelete
(
    iufl_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    list->loadedFromFile = IUS_TRUE;
    return iusFrameListDelete(list);
}

int iusFrameListDelete
(
    iufl_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    if(list->loadedFromFile == IUS_TRUE)
        iusFrameListDeleteFrames(list);
    free(list->pFrames);
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
    if( reference->numFrames != actual->numFrames ) return IUS_FALSE;
    for(index = 0 ; index < actual->numFrames ; index++ )
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
    return list->numFrames;
}

iufr_t iusFrameListGet
(
    iufl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->numFrames ) return NULL;
    return list->pFrames[index];
}

int iusFrameListSet
(
    iufl_t list,
    iufr_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->numFrames ) return IUS_ERR_VALUE;
    list->pFrames[index] = member;
    return IUS_E_OK;
}

iufl_t iusFrameListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numFrames,i;

	hid_t frameList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT);
    int status = iusHdf5ReadInt(frameList_id, IUS_INPUTFILE_PATH_FRAMELIST_SIZE, &(numFrames));
    if(status!=0) return IUFL_INVALID;
	
    iufl_t frameList = iusFrameListCreate(numFrames);
    iufr_t sourceElement;

    // Load frames
    for (i=0;i < numFrames; i++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_FRAMELIST_FRAME, i);
	
		hid_t frame_id = H5Gopen(frameList_id, path, H5P_DEFAULT);
        sourceElement = iusFrameLoad(frame_id);
        if(sourceElement == IUF_INVALID)
        {
            break;
        }
		H5Gclose(frame_id);
        iusFrameListSet(frameList,sourceElement,i);
    }
	H5Gclose(frameList_id);
    frameList->loadedFromFile = IUS_TRUE;
    return frameList;
}

IUS_BOOL iusFrameListFull
(
    iufl_t list
)
{
    IUS_BOOL isFull = IUS_TRUE;
    int i;
    for (i=0;i < list->numFrames;i++)
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
    hid_t handle
)
{
    int status=0;
    int i,size;
    char path[IUS_MAX_HDF5_PATH];
	hid_t group_id;

    if(list == NULL)
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;
    if(iusFrameListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;


	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_FRAMELIST, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUS_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT);
	}
	if (group_id == H5I_INVALID_HID)
		return IUS_ERR_VALUE;

	iufr_t sourceElement;
    size = iusFrameListGetSize(list);
    status = iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_FRAMELIST_SIZE, &(size), 1);
	if (status != IUS_E_OK) return IUS_ERR_VALUE;

    // iterate over source list elements and save'em
    for (i=0; i < size ;i++)
    {
        sourceElement = iusFrameListGet(list,i);
        if(sourceElement == IUF_INVALID) continue;

        sprintf(path, IUS_INPUTFILE_PATH_FRAMELIST_FRAME, i);
		hid_t frame_id = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		
        status = iusFrameSave(sourceElement, frame_id);
		H5Gclose(frame_id);
		
		if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
