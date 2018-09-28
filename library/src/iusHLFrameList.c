//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
#include <iusHLFrameListImp.h>
#include <include/iusHDF5.h>
#include <include/iusHLFrameImp.h>


// ADT
struct IusFrameList
{
    int count;
    iuf_t *   pFrames ;
} ;

// ADT

iufl_t iusHLFrameListCreate
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

int iusHLFrameListDelete
(
    iufl_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHLFrameListCompare
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
        if( iusHLFrameCompare( reference->pFrames[index], actual->pFrames[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHLFrameListGetSize
(
    iufl_t list
)
{
    return list->count;
}

iuf_t iusHLFrameListGet
(
    iufl_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->pFrames[index];
}

int iusHLFrameListSet
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

//TODO: do we need "numframes" if we have framelist size?

iufl_t iusHLFrameListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numFrames,i;

	hid_t frameList_id = H5Gopen(handle, IUS_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT);
    int status = iusHdf5ReadInt(frameList_id, IUS_INPUTFILE_PATH_FRAMELIST_SIZE, &(numFrames));
    if(status!=0) return IUFL_INVALID;
	
    iufl_t frameList = iusHLFrameListCreate(numFrames);
    iuf_t sourceElement;

    // Load frames
    for (i=0;i < numFrames; i++)
    {
        sprintf(path, IUS_INPUTFILE_PATH_FRAMELIST_FRAME, i);
	
		hid_t frame_id = H5Gopen(frameList_id, path, H5P_DEFAULT);
        sourceElement = iusHLFrameLoad(frame_id);
        if(sourceElement == IUF_INVALID)
        {
            break;
        }
		H5Gclose(frame_id);
        iusHLFrameListSet(frameList,sourceElement,i);
    }
	H5Gclose(frameList_id);
    return frameList;
}

IUS_BOOL iusHLFrameListFull
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


int iusHLFrameListSave
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
    if(iusHLFrameListFull(list) == IUS_FALSE)
        return IUS_ERR_VALUE;


	status = H5Gget_objinfo(handle, IUS_INPUTFILE_PATH_FRAMELIST, 0, NULL); // todo centralize the path
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

	status = 0;
	iuf_t sourceElement;
    size = iusHLFrameListGetSize(list);
    status = iusHdf5WriteInt(group_id, IUS_INPUTFILE_PATH_FRAMELIST_SIZE, &(size), 1);
	if (status != IUS_E_OK) return IUS_ERR_VALUE;

    // iterate over source list elements and save'em
    for (i=0; i < size ;i++)
    {
        sourceElement = iusHLFrameListGet(list,i);
        if(sourceElement == IUF_INVALID) continue;

        sprintf(path, IUS_INPUTFILE_PATH_FRAMELIST_FRAME, i);
		hid_t frame_id = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		
        status = iusHLFrameSave(sourceElement, frame_id);
		H5Gclose(frame_id);
		
		if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
