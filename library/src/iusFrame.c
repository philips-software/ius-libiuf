//
// Created by nlv09165 on 15/08/2018.
//
#include <string.h>

#include <ius.h>
#include <iusFramePrivate.h>

struct IusFrame
{
    char *patternListLabel;
    int dataIndex;
    float time;
} ;

// ADT
iufr_t iusFrameCreate
(
    char *label,
    int dataIndex,
    float time
)
{
    IUS_ERR_EVAL_N_RETURN(dataIndex <0, IUF_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(label, IUF_INVALID);
    iufr_t created = calloc(1,sizeof(IusFrame));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusFrame, IUF_INVALID);
    created->patternListLabel = strdup(label);
    created->dataIndex = dataIndex;
    created->time = time;
    return created;
}

int iusFrameDelete
(
    iufr_t frame
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(frame, IUS_ERR_VALUE);
    free(frame->patternListLabel);
    free(frame);
    return IUS_E_OK;
}


// operations
int iusFrameCompare
(
    iufr_t reference,
    iufr_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( strcmp(reference->patternListLabel, actual->patternListLabel) != 0 ) return IUS_FALSE;
    if( reference->dataIndex != actual->dataIndex ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->time, actual->time) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
float iusFrameGetTime
(
    iufr_t frame
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(frame, -1);
    return frame->time;
}

char *iusFrameGetPatternListLabel
(
    iufr_t frame
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(frame, NULL);
    return frame->patternListLabel;
}

int iusFrameGetDataIndex
(
    iufr_t frame
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(frame, -1);
    return frame->dataIndex;
}


int iusFrameSave
(
    iufr_t frame,
    hid_t handle
)
{
    int  status=IUS_E_OK;
    IUS_ERR_CHECK_NULL_N_RETURN(frame, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL, frame->patternListLabel);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(frame->dataIndex), 1);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(frame->time), 1);
    return status;
}

iufr_t iusFrameLoad
(
    hid_t handle
)
{
    int status=IUS_E_OK;

    char patternListLabel[IUS_MAX_HDF5_PATH];
    int dataIndex;
    float time;
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_INVALID);
    status |= iusHdf5ReadString( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL, patternListLabel);
    status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(dataIndex));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(time));
    if( status < 0 )
        return IUF_INVALID;

	return iusFrameCreate(patternListLabel,dataIndex,time);
}
