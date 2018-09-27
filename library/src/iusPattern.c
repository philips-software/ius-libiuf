
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

#include "include/iusPulse.h"
#include "include/iusPatternImp.h"


#define PULSELABELFMT "%s/PulseLabel"
#define PATTERNLABELFMT "%s/PatternLabel"
#define SOURCELABELFMT "%s/SourceLabel"
#define CHANNELMAPLABELFMT "%s/ChannelMapLabel"
#define APODIZATIONLABELFMT "%s/ApodizationLabel"
#define RECEIVESETTNGSLABELFMT "%s/ReceiveSettingsLabel"
#define TIMEINFRAMEFMT "%s/TimeInFrame"
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

iupa_t iusPatternCreate
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

int iusPatternDelete
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
int iusPatternCompare
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



int iusPatternSave
(
    iupa_t pattern,
    char *parentPath,
    hid_t handle
)
{
  int status=IUS_E_OK;
  char path[IUS_MAX_HDF5_PATH];

  hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  sprintf(path, PATTERNLABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pLabel);
  sprintf(path, PULSELABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pPulseLabel);
  sprintf(path, SOURCELABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pSourceLabel);
  sprintf(path, CHANNELMAPLABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pChannelMapLabel);
  sprintf(path, APODIZATIONLABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pApodizationLabel);
  sprintf(path, RECEIVESETTNGSLABELFMT, parentPath);
  status |= iusHdf5WriteString(group_id, path, pattern->pReceiveSettingsLabel);
  sprintf(path, TIMEINFRAMEFMT, parentPath);
  status |= iusHdf5WriteFloat(handle, path, &(pattern->timeInFrame), 1);
  status |= H5Gclose(group_id );
  return status;
}


iupa_t iusPatternLoad
(
  hid_t handle,
  char *parentPath
)
{
  int status = 0;
  char pLabel[256];
  float timeInFrame;
  char pPulseLabel[256];
  char pSourceLabel[256];
  char pChannelMapLabel[256];
  char pApodizationLabel[256];
  char pReceiveSettingsLabel[256];
  char path[IUS_MAX_HDF5_PATH];

  sprintf(path, PATTERNLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pLabel);
  sprintf(path, PULSELABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pPulseLabel);
  sprintf(path, SOURCELABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pSourceLabel);
  sprintf(path, CHANNELMAPLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pChannelMapLabel);
  sprintf(path, APODIZATIONLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pApodizationLabel);
  sprintf(path, RECEIVESETTNGSLABELFMT, parentPath);
  status |= iusHdf5ReadString(handle, path, pReceiveSettingsLabel);
  sprintf(path, TIMEINFRAMEFMT, parentPath);
  status |= iusHdf5ReadFloat( handle, path, &(timeInFrame));

  if( status < 0 )
    return NULL;

  return iusPatternCreate(pLabel,timeInFrame,
                            pPulseLabel,
                            pSourceLabel,
                            pChannelMapLabel,
                            pApodizationLabel,
                            pReceiveSettingsLabel);
}



// Getters
const char * iusPatternGetLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pLabel;
}

const char * iusPatternGetPulseLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pPulseLabel;
}

const char * iusPatternGetSourceLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pSourceLabel;
}

const char * iusPatternGetChannelMapLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pChannelMapLabel;
}

const char * iusPatternGetApodizationLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pApodizationLabel;
}

const char * iusPatternGetReceivesettingsLabel
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NULL;
    return iusPattern->pReceiveSettingsLabel;
}

float iusPatternGetTimeInFrame
(
    iupa_t iusPattern
)
{
    if(iusPattern == IUPA_INVALID) return NAN;
    return iusPattern->timeInFrame;
}

