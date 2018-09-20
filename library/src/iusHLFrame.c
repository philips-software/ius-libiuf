//
// Created by nlv09165 on 15/08/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>

#include <iusHLFrameImp.h>
#include <include/iusHDF5.h>
#include <math.h>

struct IusFrame
{
    int patternListIndex;
    int dataIndex;
    float time;
} ;

#define IUS_FRAME_PATTERNLISTINDEX "PatternListIndex"
#define IUS_FRAME_DATAINDEX "DataIndex"
#define IUS_FRAME_TIME "Time"

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
    //char path[IUS_MAX_HDF5_PATH];

    //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, IUSPATTERNLISTINDEXFMT, parentPath);
    status |= iusHdf5WriteInt(handle, IUS_FRAME_PATTERNLISTINDEX, &(frame->patternListIndex), 1);
    //sprintf(path, IUSDATAINDEXFMT, parentPath);
    status |= iusHdf5WriteInt(handle, IUS_FRAME_DATAINDEX, &(frame->dataIndex), 1);
    //sprintf(path, IUSTIMEFMT, parentPath);
    status |= iusHdf5WriteFloat(handle, IUS_FRAME_TIME, &(frame->time), 1, 1);
    //status |= H5Gclose(group_id );
    return status;
}

iuf_t iusHLFrameLoad
(
    hid_t handle
)
{
    int status=IUS_E_OK;
    //char path[IUS_MAX_HDF5_PATH];
    int patternListIndex;
    int dataIndex;
    float time;

    //sprintf(path, IUS_FRAME_PATTERNLISTINDEX, parentPath);
    status |= iusHdf5ReadInt( handle, IUS_FRAME_PATTERNLISTINDEX, &(patternListIndex));
    //sprintf(path, DATAINDEXFMT, parentPath);
    status |= iusHdf5ReadInt( handle, IUS_FRAME_DATAINDEX, &(dataIndex));
    //sprintf(path, TIMEFMT, parentPath);
    status |= iusHdf5ReadFloat( handle, IUS_FRAME_TIME, &(time));
    if( status < 0 )
        return IUF_INVALID;
    return iusHLFrameCreate(patternListIndex,dataIndex,time);
}