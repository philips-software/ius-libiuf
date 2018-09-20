//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
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


#define FRAMELISTFMT "Frame[%d]"
#define FRAMELISTSIZE "Size"

iufl_t iusHLFrameListLoad
(
    hid_t handle
)
{
    char path[IUS_MAX_HDF5_PATH];
    int numPatterns,i;
    
    int status = iusHdf5ReadInt(handle, FRAMELISTSIZE, &(numPatterns));
    if(status!=0) return IUFL_INVALID;
	hid_t frameList_id = H5Gopen(handle, "/Frames", H5P_DEFAULT);
    iufl_t frameList = iusHLFrameListCreate(numPatterns);
    iuf_t sourceElement;

    // Load frames
    for (i=0;i < numPatterns;i++)
    {
        sprintf(path, FRAMELISTFMT, i);
		//
		hid_t frame_id = H5Gopen(frameList_id, path, H5P_DEFAULT);
        sourceElement = iusHLFrameLoad(frame_id);
        if(sourceElement == IUF_INVALID)
        {
            break;
        }
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

	status = H5Gget_objinfo(handle, "Frames", 0, NULL); // todo centralize the path
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, "Frames", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, "Frames", H5P_DEFAULT);
	}
	if (handle == H5I_INVALID_HID)
		return IUS_ERR_VALUE;
	
	iuf_t sourceElement;
    size = iusHLFrameListGetSize(list);
    //sprintf(path, FRAMELISTSIZE, handle);
    status |= iusHdf5WriteInt(group_id, FRAMELISTSIZE, &(size), 1);

    // iterate over source list elements and save'em
    for (i=0;i < size;i++)
    {
        sourceElement = iusHLFrameListGet(list,i);
        if(sourceElement == IUF_INVALID) continue;

        sprintf(path, FRAMELISTFMT, i);
		hid_t frame_id = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		//H5close(frame_id);
		
		//hid_t grp = H5Gopen(group_id, path, H5P_DEFAULT);
        status = iusHLFrameSave(sourceElement, frame_id);
		H5Gclose(frame_id);
		
		if(status != IUS_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
