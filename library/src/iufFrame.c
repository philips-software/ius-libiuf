#include <string.h>

#include <iuf.h>
#include <iufFramePrivate.h>

struct IufFrame
{
    char *patternListLabel;
    int dataIndex;
    float time;
} ;

// ADT
iufr_t iufFrameCreate
(
    char *label,
    int dataIndex,
    float time
)
{
    IUF_ERR_EVAL_N_RETURN(dataIndex <0, IUF_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(label, IUF_INVALID);
    iufr_t created = calloc(1,sizeof(IufFrame));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufFrame, IUF_INVALID);
    created->patternListLabel = strdup(label);
    created->dataIndex = dataIndex;
    created->time = time;
    return created;
}

int iufFrameDelete
(
    iufr_t frame
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(frame, IUF_ERR_VALUE);
    free(frame->patternListLabel);
    free(frame);
    return IUF_E_OK;
}


// operations
int iufFrameCompare
(
    iufr_t reference,
    iufr_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( strcmp(reference->patternListLabel, actual->patternListLabel) != 0 ) return IUF_FALSE;
    if( reference->dataIndex != actual->dataIndex ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->time, actual->time) == IUF_FALSE ) return IUF_FALSE;
    return IUF_TRUE;
}

// Getters
float iufFrameGetTime
(
    iufr_t frame
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(frame, -1);
    return frame->time;
}

char *iufFrameGetPatternListLabel
(
    iufr_t frame
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(frame, NULL);
    return frame->patternListLabel;
}

int iufFrameGetDataIndex
(
    iufr_t frame
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(frame, -1);
    return frame->dataIndex;
}


int iufFrameSave
(
    iufr_t frame,
    hid_t handle
)
{
    int  status=IUF_E_OK;
    IUF_ERR_CHECK_NULL_N_RETURN(frame, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL, frame->patternListLabel);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(frame->dataIndex), 1);
    status |= iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(frame->time), 1);
    return status;
}

iufr_t iufFrameLoad
(
    hid_t handle
)
{
    int status=IUF_E_OK;

    char patternListLabel[IUF_MAX_HDF5_PATH];
    int dataIndex;
    float time;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_INVALID);
    status |= iufHdf5ReadString( handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL, patternListLabel);
    status |= iufHdf5ReadInt( handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(dataIndex));
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(time));
    if( status < 0 )
        return IUF_INVALID;

    return iufFrameCreate(patternListLabel,dataIndex,time);
}
