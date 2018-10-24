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
    char *patternListLabel,
    int dataIndex,
    float time
)
{
    if( patternListLabel == NULL || dataIndex < 0) return IUF_INVALID;
	if (strlen(patternListLabel) ==  0) return IUF_INVALID;
    iufr_t created = calloc(1,sizeof(IusFrame));
    created->patternListLabel = strdup(patternListLabel);
    created->dataIndex = dataIndex;
    created->time = time;
    return created;
}

int iusFrameDelete
(
    iufr_t iusFrame
)
{
    int status = IUS_ERR_VALUE;
    if(iusFrame != NULL)
    {
        free(iusFrame);
        status = IUS_E_OK;
    }
    return status;
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
    iufr_t iusFrame
)
{
    if( iusFrame == NULL ) return -1;
    return iusFrame->time;
}

char *iusFrameGetPatternListLabel
(
    iufr_t iusFrame
)
{
    if( iusFrame == NULL ) return NULL;
    return iusFrame->patternListLabel;
}

int iusFrameGetDataIndex
(
    iufr_t iusFrame
)
{
    if( iusFrame == NULL ) return -1;
    return iusFrame->dataIndex;
}


int iusFrameSave
(
    iufr_t frame,
    hid_t handle
)
{
    int  status=IUS_E_OK;
 
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

    status |= iusHdf5ReadString( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTLABEL, patternListLabel);
    status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(dataIndex));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(time));
    if( status < 0 )
        return IUF_INVALID;

	return iusFrameCreate(patternListLabel,dataIndex,time);
}
