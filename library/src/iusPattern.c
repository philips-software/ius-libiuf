
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
#include <iusInputFileStructure.h>
#include "include/iusPulse.h"
#include "include/iusPatternPrivate.h"

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
    created->pLabel = strdup(pLabel);
    created->pPulseLabel = strdup(pPulseLabel);
    created->pSourceLabel = strdup(pSourceLabel);
    created->pChannelMapLabel = strdup(pChannelMapLabel);
    created->pApodizationLabel = strdup(pApodizationLabel);
    created->pReceiveSettingsLabel = strdup(pReceiveSettingsLabel);
    return created;
}

int iusPatternDelete
(
    iupa_t iusPattern
)
{
    if (iusPattern == IUPA_INVALID) return IUS_ERR_VALUE;
    free((void *)iusPattern->pLabel);
    free((void *)iusPattern->pPulseLabel);
    free((void *)iusPattern->pSourceLabel);
    free((void *)iusPattern->pChannelMapLabel);
    free((void *)iusPattern->pApodizationLabel);
    free((void *)iusPattern->pReceiveSettingsLabel);
    free(iusPattern);
    return IUS_E_OK;
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
    hid_t handle
)
{
  int status=IUS_E_OK;
  //char path[IUS_MAX_HDF5_PATH];

  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PATTERNLABEL, pattern->pLabel);
  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pattern->pPulseLabel);
  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pattern->pSourceLabel);
  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pattern->pChannelMapLabel);
  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pattern->pApodizationLabel);
  status |= iusHdf5WriteString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL, pattern->pReceiveSettingsLabel);
  status |= iusHdf5WriteFloat(handle,  IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(pattern->timeInFrame), 1);
  return status;
}


iupa_t iusPatternLoad
(
  hid_t handle
)
{
  int status = 0;

  float timeInFrame;
  char label[IUS_MAX_HDF5_PATH];
  char pulseLabel[IUS_MAX_HDF5_PATH];
  char sourceLabel[IUS_MAX_HDF5_PATH];
  char channelMapLabel[IUS_MAX_HDF5_PATH];
  char apodizationLabel[IUS_MAX_HDF5_PATH];
  char receiveSettingsLabel[IUS_MAX_HDF5_PATH];

  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PATTERNLABEL, label);
  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pulseLabel);
  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, sourceLabel);
  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, channelMapLabel);
  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, apodizationLabel);
  status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL, receiveSettingsLabel);
  status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(timeInFrame));

  if( status < 0 )
    return NULL;

  return iusPatternCreate(label,timeInFrame,
                            pulseLabel,
                            sourceLabel,
                            channelMapLabel,
                            apodizationLabel,
                            receiveSettingsLabel);
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

