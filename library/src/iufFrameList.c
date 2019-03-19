//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>

#include <iuf.h>
#include <iufFramePrivate.h>
#include <iufFrameListPrivate.h>


// ADT
struct IufFrameList
{
    int numFrames;
    iufr_t *  pFrames ;
    IUF_BOOL deepDelete;
} ;

// ADT

iufl_t iufFrameListCreate
(
    int numFrames
)
{
    IUF_ERR_EVAL_N_RETURN(numFrames<=0, IUFL_INVALID);
    iufl_t list = calloc(1, sizeof(IufFrameList));
    IUF_ERR_ALLOC_NULL_N_RETURN(list, IufFrameList, IUFL_INVALID);
    list->deepDelete = IUF_FALSE;
    list->numFrames = numFrames;
    list->pFrames = (iufr_t *) calloc((size_t)numFrames, sizeof(iufr_t));
    if( list->pFrames == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pFrames member");
        free(list);
        list = IUFL_INVALID;
    }
    return list;
}

int iufFrameListDeleteFrames
(
    iufl_t list
)
{
    int index;
    int status = 0;
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    for(index = 0 ; index < list->numFrames ; index++ )
    {
        status |= iufFrameDelete(list->pFrames[index]);
    }
    return status;
}

int iufFrameListDeepDelete
(
    iufl_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    list->deepDelete = IUF_TRUE;
    return iufFrameListDelete(list);
}

int iufFrameListDelete
(
    iufl_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    if(list->deepDelete == IUF_TRUE)
        iufFrameListDeleteFrames(list);
    free(list->pFrames);
    free(list);
    return IUF_E_OK;
}


// operations
int iufFrameListCompare
(
    iufl_t reference,
    iufl_t actual
)
{
    int index;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numFrames != actual->numFrames ) return IUF_FALSE;
    for(index = 0 ; index < actual->numFrames ; index++ )
    {
        if( iufFrameCompare( reference->pFrames[index], actual->pFrames[index] )
            == IUF_FALSE )
            return IUF_FALSE;
    }
    return IUF_TRUE;
}


int iufFrameListGetSize
(
    iufl_t list
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, -1);
    return list->numFrames;
}

iufr_t iufFrameListGet
(
    iufl_t list,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_INVALID);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numFrames, IUF_INVALID);
    return list->pFrames[index];
}

int iufFrameListSet
(
    iufl_t list,
    iufr_t member,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(index < 0  || index >= list->numFrames, IUF_ERR_VALUE);
    list->pFrames[index] = member;
    return IUF_E_OK;
}

iufl_t iufFrameListLoad
(
    hid_t handle
)
{
    char path[IUF_MAX_HDF5_PATH];
    int numFrames,i;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUFL_INVALID);
	hid_t frameList_id = H5Gopen(handle, IUF_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT);
    int status = iufHdf5ReadInt(frameList_id, IUF_INPUTFILE_PATH_FRAMELIST_SIZE, &(numFrames));
    if(status!=0) return IUFL_INVALID;
	
    iufl_t frameList = iufFrameListCreate(numFrames);
    iufr_t sourceElement;

    // Load frames
    for (i=0;i < numFrames; i++)
    {
        sprintf(path, IUF_INPUTFILE_PATH_FRAMELIST_FRAME, i);
	
		hid_t frame_id = H5Gopen(frameList_id, path, H5P_DEFAULT);
        sourceElement = iufFrameLoad(frame_id);
        if(sourceElement == IUF_INVALID)
        {
            break;
        }
		H5Gclose(frame_id);
        iufFrameListSet(frameList,sourceElement,i);
    }
	H5Gclose(frameList_id);
    frameList->deepDelete = IUF_TRUE;
    return frameList;
}

IUF_BOOL iufFrameListFull
(
    iufl_t list
)
{
    IUF_BOOL isFull = IUF_TRUE;
    int i;
    for (i=0;i < list->numFrames;i++)
    {
        if(list->pFrames[i] == IUF_INVALID)
        {
            isFull = IUF_FALSE;
            break;
        }
    }
    return isFull;
}


int iufFrameListSave
(
    iufl_t list,
    hid_t handle
)
{
    int status=0;
    int i,size;
    char path[IUF_MAX_HDF5_PATH];
	hid_t group_id;

    IUF_ERR_CHECK_NULL_N_RETURN(list, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    if(iufFrameListFull(list) == IUF_FALSE)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE, "argument list (frame list) was not full");
        return IUF_ERR_VALUE;
    }


	status = H5Gget_objinfo(handle, IUF_INPUTFILE_PATH_FRAMELIST, 0, NULL);
	if (status != 0) // the group does not exist yet
	{
		group_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		group_id = H5Gopen(handle, IUF_INPUTFILE_PATH_FRAMELIST, H5P_DEFAULT);
	}

	if (group_id == H5I_INVALID_HID)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "Error getting handle for path: %s", IUF_INPUTFILE_PATH_FRAMELIST);
        return IUF_ERR_VALUE;
    }

	iufr_t sourceElement;
    size = iufFrameListGetSize(list);
    status = iufHdf5WriteInt(group_id, IUF_INPUTFILE_PATH_FRAMELIST_SIZE, &(size), 1);
	if (status != IUF_E_OK) return IUF_ERR_VALUE;

    // iterate over source list elements and save'em
    for (i=0; i < size ;i++)
    {
        sourceElement = iufFrameListGet(list,i);
        if(sourceElement == IUF_INVALID) continue;

        sprintf(path, IUF_INPUTFILE_PATH_FRAMELIST_FRAME, i);
		hid_t frame_id = H5Gcreate(group_id, path, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
		
        status = iufFrameSave(sourceElement, frame_id);
		H5Gclose(frame_id);
		
		if(status != IUF_E_OK) break;
    }

    status |= H5Gclose(group_id);
    return status;
}
