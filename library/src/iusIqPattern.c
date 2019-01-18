
//
// Created by nlv12901 on 14/01/2019.
//
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <ius.h>
#include <iusIqPatternPrivate.h>

struct IusIqPattern
{
    float timeInFrame;
    const char* pPulseLabel;
    const char* pSourceLabel;
    const char* pChannelMapLabel;
    const char* pApodizationLabel;
    const char* pDemodulationLabel;
} ;

// ADT
iuiqpa_t iusIqPatternCreate
(
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pDemodulationLabel
)
{
    if( pPulseLabel == NULL ||
        pSourceLabel == NULL ||
        pChannelMapLabel == NULL ||
        pApodizationLabel == NULL ||
        pDemodulationLabel == NULL
    ) return IUIQPA_INVALID;

    if( strcmp(pPulseLabel,"") == 0 ||
        strcmp(pSourceLabel,"") == 0 ||
        strcmp(pChannelMapLabel,"") == 0 ||
        strcmp(pApodizationLabel,"") == 0 ||
        strcmp(pDemodulationLabel,"") == 0
    ) return IUIQPA_INVALID;

    if( timeInFrame < 0.0f ) return IUIQPA_INVALID;
    iuiqpa_t created = calloc(1, sizeof(IusIqPattern));
    created->timeInFrame = timeInFrame;
    created->pPulseLabel = strdup(pPulseLabel);
    created->pSourceLabel = strdup(pSourceLabel);
    created->pChannelMapLabel = strdup(pChannelMapLabel);
    created->pApodizationLabel = strdup(pApodizationLabel);
    created->pDemodulationLabel = strdup(pDemodulationLabel);
    return created;
}

int iusIqPatternDelete
(
    iuiqpa_t iusIqPattern
)
{
    int status = IUS_ERR_VALUE;
    if(iusIqPattern != IUIQPA_INVALID)
    {
        free((void *)iusIqPattern->pApodizationLabel);
        free((void *)iusIqPattern->pChannelMapLabel);
        free((void *)iusIqPattern->pPulseLabel);
        free((void *)iusIqPattern->pSourceLabel);
        free((void *)iusIqPattern->pDemodulationLabel);
        free(iusIqPattern);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusIqPatternCompare
(
    iuiqpa_t reference,
    iuiqpa_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == IUIQPA_INVALID || actual == IUIQPA_INVALID ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->timeInFrame, actual->timeInFrame ) == IUS_FALSE ) return IUS_FALSE;

    if( strcmp(reference->pDemodulationLabel,actual->pDemodulationLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pApodizationLabel,actual->pApodizationLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pChannelMapLabel,actual->pChannelMapLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pSourceLabel,actual->pSourceLabel) != 0 ) return IUS_FALSE;
    if( strcmp(reference->pPulseLabel,actual->pPulseLabel) != 0 ) return IUS_FALSE;

    return IUS_TRUE;
}



int iusIqPatternSave
(
    iuiqpa_t pattern,
    hid_t handle
)
{
  int status=IUS_E_OK;
  //char path[IUS_MAX_HDF5_PATH];

  status |= iusHdf5WriteString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pattern->pPulseLabel);
  status |= iusHdf5WriteString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pattern->pSourceLabel);
  status |= iusHdf5WriteString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pattern->pChannelMapLabel);
  status |= iusHdf5WriteString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pattern->pApodizationLabel);
  status |= iusHdf5WriteString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_DEMODULATIONLABEL, pattern->pDemodulationLabel);
  status |= iusHdf5WriteFloat(handle,  IUS_IQFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(pattern->timeInFrame), 1);
  return status;
}


iuiqpa_t iusIqPatternLoad
(
  hid_t handle
)
{
  int status = 0;

  float timeInFrame;
  char pPulseLabel[IUS_MAX_HDF5_PATH];
  char pSourceLabel[IUS_MAX_HDF5_PATH];
  char pChannelMapLabel[IUS_MAX_HDF5_PATH];
  char pApodizationLabel[IUS_MAX_HDF5_PATH];
  char pDemodulationLabel[IUS_MAX_HDF5_PATH];

  status |= iusHdf5ReadString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pPulseLabel);
  status |= iusHdf5ReadString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pSourceLabel);
  status |= iusHdf5ReadString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pChannelMapLabel);
  status |= iusHdf5ReadString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pApodizationLabel);
  status |= iusHdf5ReadString(handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_DEMODULATIONLABEL, pDemodulationLabel);
  status |= iusHdf5ReadFloat( handle, IUS_IQFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(timeInFrame));

  if( status < 0 )
    return NULL;

  return iusIqPatternCreate(timeInFrame,
                          pPulseLabel,
                          pSourceLabel,
                          pChannelMapLabel,
                          pApodizationLabel,
                          pDemodulationLabel);
}

const char * iusIqPatternGetPulseLabel
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NULL;
    return iusIqPattern->pPulseLabel;
}

const char * iusIqPatternGetSourceLabel
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NULL;
    return iusIqPattern->pSourceLabel;
}

const char * iusIqPatternGetChannelMapLabel
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NULL;
    return iusIqPattern->pChannelMapLabel;
}

const char * iusIqPatternGetApodizationLabel
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NULL;
    return iusIqPattern->pApodizationLabel;
}

const char * iusIqPatternGetDemodulationLabel
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NULL;
    return iusIqPattern->pDemodulationLabel;
}

float iusIqPatternGetTimeInFrame
(
    iuiqpa_t iusIqPattern
)
{
    if(iusIqPattern == IUIQPA_INVALID) return NAN;
    return iusIqPattern->timeInFrame;
}

