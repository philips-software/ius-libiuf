//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusInputFileStructure.h>
#include <iusHLFrameImp.h>
#include <include/iusHDF5.h>
#include <math.h>

struct IusFrame
{
    int patternListIndex;
    int dataIndex;
    float time;
} ;

// ADT
iuf_t iusHLFrameCreate
(
    int patternListIndex,
    int dataIndex,
    float time
)
{
    if( patternListIndex < 0 || dataIndex < 0) return IUF_INVALID;
    iuf_t created = calloc(1,sizeof(IusFrame));
    created->patternListIndex = patternListIndex;
    created->dataIndex = dataIndex;
    created->time = time;
    return created;
}

int iusHLFrameDelete
(
    iuf_t iusFrame
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
int iusHLFrameCompare
(
    iuf_t reference,
    iuf_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->patternListIndex != actual->patternListIndex ) return IUS_FALSE;
    if( reference->dataIndex != actual->dataIndex ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->time, actual->time) == IUS_FALSE ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters

float iusHLFrameGetTime
(
    iuf_t iusFrame
)
{
    if( iusFrame == NULL ) return -1;
    return iusFrame->time;
}

int iusHLFrameGetPatternListIndex
(
    iuf_t iusFrame
)
{
    if( iusFrame == NULL ) return -1;
    return iusFrame->patternListIndex;
}

int iusHLFrameGetDataIndex
(
    iuf_t iusFrame
)
{
    if( iusFrame == NULL ) return -1;
    return iusFrame->dataIndex;
}


int iusHLFrameSave
(
    iuf_t frame,
    hid_t handle
)
{
    int  status=IUS_E_OK;
 
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTINDEX, &(frame->patternListIndex), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(frame->dataIndex), 1);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(frame->time), 1, 1);
    return status;
}

iuf_t iusHLFrameLoad
(
    hid_t handle
)
{
    int status=IUS_E_OK;

    int patternListIndex;
    int dataIndex;
    float time;

    status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_PATTERNLISTINDEX, &(patternListIndex));
    status |= iusHdf5ReadInt( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_DATAINDEX, &(dataIndex));
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_FRAMELIST_FRAME_TIME, &(time));
    if( status < 0 )
        return IUF_INVALID;

	return iusHLFrameCreate(patternListIndex,dataIndex,time);
}