
//
// Created by nlv09165 on 13/07/2018.
//
#include <stdlib.h>
#include <hdf5.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <math.h>
#include <string.h>
#include <include/iusHDF5.h>

#include "include/iusHLPulse.h"
#include "include/iusHLPatternImp.h"


#define PULSELABEL "PulseLabel"
#define PATTERNLABEL "PatternLabel"
#define SOURCELABEL "SourceLabel"
#define CHANNELMAPLABEL "ChannelMapLabel"
#define APODIZATIONLABEL "ApodizationLabel"
#define RECEIVESETTNGSLABEL "ReceiveSettingsLabel"
#define TIMEINFRAME "TimeInFrame"
#define TOSTR(x)    #x

struct IusPattern
{
    const char* pLabel;
    float timeInFrame;
    const char* pPulseLabel;
    const char* pSourceLabel;
    const char* pChannelMapLabel;
    const char* pApodizationLabel;
    const char* pReceiveSettingsLabel;
} ;

// ADT

iupa_t iusHLPatternCreate
(
    const char *pLabel,
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
)
{
    if( pLabel == NULL ||
        pPulseLabel == NULL ||
        pSourceLabel == NULL ||
        pChannelMapLabel == NULL ||
        pApodizationLabel == NULL ||
        pReceiveSettingsLabel == NULL
    ) return IUPA_INVALID;

    if( strcmp(pLabel,"") == 0 ||
        strcmp(pPulseLabel,"") == 0 ||
        strcmp(pSourceLabel,"") == 0 ||
        strcmp(pChannelMapLabel,"") == 0 ||
        strcmp(pApodizationLabel,"") == 0 ||
        strcmp(pReceiveSettingsLabel,"") == 0
    ) return IUPA_INVALID;

    if( timeInFrame <= 0.0f ) return IUPA_INVALID;
    iupa_t created = calloc(1, sizeof(IusPattern));
    created->pLabel = pLabel;
    created->timeInFrame = timeInFrame;
    created->pPulseLabel = pPulseLabel;
    created->pSourceLabel = pSourceLabel;
    created->pChannelMapLabel = pChannelMapLabel;
    created->pApodizationLabel = pApodizationLabel;
    created->pReceiveSettingsLabel = pReceiveSettingsLabel;
    return created;
}

int iusHLPatternDelete
(
    iupa_t iusPattern
)
{
    int status = IUS_ERR_VALUE;
    if(iusPattern != IUPA_INVALID)
    {
        free(iusPattern);
        iusPattern = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLPatternCompare
(
    iupa_t reference,
    iupa_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == IUPA_INVALID || actual == IUPA_INVALID ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->timeInFrame, actual->timeInFrame ) == IUS_FALSE ) return IUS_FALSE;

    if( strcmp(reference->pLabel,actual->pLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pReceiveSettingsLabel,actual->pReceiveSettingsLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pApodizationLabel,actual->pApodizationLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pChannelMapLabel,actual->pChannelMapLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pSourceLabel,actual->pSourceLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pPulseLabel,actual->pPulseLabel) != 0 ) return IUS_FALSE;

    return IUS_TRUE;
}



int iusHLPatternSave
(
    iupa_t pattern,
    hid_t handle
)
{
  int status=IUS_E_OK;
  //char path[IUS_MAX_HDF5_PATH];

  //hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  //sprintf(path, PATTERNLABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, PATTERNLABEL, pattern->pLabel, 1);
  //sprintf(path, PULSELABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, PULSELABEL, pattern->pPulseLabel, 1);
 // sprintf(path, SOURCELABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, SOURCELABEL, pattern->pSourceLabel, 1);
  //sprintf(path, CHANNELMAPLABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, CHANNELMAPLABEL, pattern->pChannelMapLabel, 1);
  //sprintf(path, APODIZATIONLABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, APODIZATIONLABEL, pattern->pApodizationLabel, 1);
  //sprintf(path, RECEIVESETTNGSLABELFMT, parentPath);
  status |= iusHdf5WriteString(handle, RECEIVESETTNGSLABEL, pattern->pReceiveSettingsLabel, 1);
  //sprintf(path, TIMEINFRAMEFMT, parentPath);
  status |= iusHdf5WriteFloat(handle, TIMEINFRAME, &(pattern->timeInFrame), 1, 1);
  //status |= H5Gclose(group_id );
  return status;
}


iupa_t iusHLPatternLoad
(
  hid_t handle
)
{
  int status = 0;
  //IusPulseType type;
  const char *pLabel;
  float timeInFrame;
  const char *pPulseLabel;
  const char *pSourceLabel;
  const char *pChannelMapLabel;
  const char *pApodizationLabel;
  const char *pReceiveSettingsLabel;
  //char path[IUS_MAX_HDF5_PATH];

  //sprintf(path, PATTERNLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, PATTERNLABEL, &(pLabel));
  //sprintf(path, PULSELABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, PULSELABEL, &(pPulseLabel));
  //sprintf(path, SOURCELABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, SOURCELABEL, &(pSourceLabel));
  //sprintf(path, CHANNELMAPLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, CHANNELMAPLABEL, &(pChannelMapLabel));
  //sprintf(path, APODIZATIONLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, APODIZATIONLABEL, &(pApodizationLabel));
  //sprintf(path, RECEIVESETTNGSLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, RECEIVESETTNGSLABEL, &(pReceiveSettingsLabel));
  //sprintf(path, TIMEINFRAMEFMT, parentPath);
  status |= iusHdf5ReadFloat( handle, TIMEINFRAME, &(timeInFrame));

  if( status < 0 )
    return NULL;

  return iusHLPatternCreate(pLabel,timeInFrame,
                            pPulseLabel,
                            pSourceLabel,
                            pChannelMapLabel,
                            pApodizationLabel,
                            pReceiveSettingsLabel);
}

// Getters
const char * iusHLPatternGetLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pLabel;
}

const char * iusHLPatternGetPulseLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pPulseLabel;
}

const char * iusHLPatternGetSourceLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pSourceLabel;
}

const char * iusHLPatternGetChannelMapLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pChannelMapLabel;
}

const char * iusHLPatternGetApodizationLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pApodizationLabel;
}

const char * iusHLPatternGetReceivesettingsLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pReceiveSettingsLabel;
}

float iusHLPatternGetTimeInFrame
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NAN;
    return iusPattern->timeInFrame;
}

