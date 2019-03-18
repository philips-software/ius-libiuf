
//
// Created by nlv12901 on 14/01/2019.
//
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <iuf.h>
#include <iufIqPatternPrivate.h>

struct IufIqPattern
{
    float timeInFrame;
    const char* pPulseLabel;
    const char* pSourceLabel;
    const char* pChannelMapLabel;
    const char* pApodizationLabel;
    const char* pDemodulationLabel;
} ;

// ADT
iuiqpa_t iufIqPatternCreate
(
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pDemodulationLabel
)
{
	IUF_ERR_EVAL_N_RETURN(timeInFrame < 0.0f, IUIQPA_INVALID);
	IUF_ERR_STRP_NULL_EMPTY(pPulseLabel, IUIQPA_INVALID);
	IUF_ERR_STRP_NULL_EMPTY(pSourceLabel, IUIQPA_INVALID);
	IUF_ERR_STRP_NULL_EMPTY(pChannelMapLabel, IUIQPA_INVALID);
	IUF_ERR_STRP_NULL_EMPTY(pApodizationLabel, IUIQPA_INVALID);
	IUF_ERR_STRP_NULL_EMPTY(pDemodulationLabel, IUIQPA_INVALID);

    iuiqpa_t created = calloc(1, sizeof(IufIqPattern));
	IUF_ERR_ALLOC_NULL_N_RETURN(created, IufPattern, IUIQPA_INVALID);

    created->timeInFrame = timeInFrame;
    created->pPulseLabel = strdup(pPulseLabel);
    created->pSourceLabel = strdup(pSourceLabel);
    created->pChannelMapLabel = strdup(pChannelMapLabel);
    created->pApodizationLabel = strdup(pApodizationLabel);
    created->pDemodulationLabel = strdup(pDemodulationLabel);
    return created;
}

int iufIqPatternDelete
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, IUF_ERR_VALUE);
    free((void *)iqPattern->pApodizationLabel);
    free((void *)iqPattern->pChannelMapLabel);
    free((void *)iqPattern->pPulseLabel);
    free((void *)iqPattern->pSourceLabel);
    free((void *)iqPattern->pDemodulationLabel);
    free(iqPattern);
    return IUF_E_OK;
}


// operations
int iufIqPatternCompare
(
    iuiqpa_t reference,
    iuiqpa_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == IUIQPA_INVALID || actual == IUIQPA_INVALID ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->timeInFrame, actual->timeInFrame ) == IUF_FALSE ) return IUF_FALSE;

    if( strcmp(reference->pDemodulationLabel,actual->pDemodulationLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pApodizationLabel,actual->pApodizationLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pChannelMapLabel,actual->pChannelMapLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pSourceLabel,actual->pSourceLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pPulseLabel,actual->pPulseLabel) != 0 ) return IUF_FALSE;

    return IUF_TRUE;
}



int iufIqPatternSave
(
    iuiqpa_t pattern,
    hid_t handle
)
{
  int status=IUF_E_OK;
  IUF_ERR_CHECK_NULL_N_RETURN(pattern, IUF_ERR_VALUE);
  IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
  status |= iufHdf5WriteString(handle, IUF_PATH_PATTERN_PULSELABEL, pattern->pPulseLabel);
  status |= iufHdf5WriteString(handle, IUF_PATH_PATTERN_SOURCELABEL, pattern->pSourceLabel);
  status |= iufHdf5WriteString(handle, IUF_PATH_PATTERN_CHANNELMAPLABEL, pattern->pChannelMapLabel);
  status |= iufHdf5WriteString(handle, IUF_PATH_PATTERN_APODIZATIONLABEL, pattern->pApodizationLabel);
  status |= iufHdf5WriteString(handle, IUF_PATH_PATTERN_DEMODULATIONLABEL, pattern->pDemodulationLabel);
  status |= iufHdf5WriteFloat(handle,  IUF_PATH_PATTERN_TIMEINFRAME, &(pattern->timeInFrame), 1);
  return status;
}


iuiqpa_t iufIqPatternLoad
(
  hid_t handle
)
{
  int status = 0;
  IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIQPA_INVALID);

  float timeInFrame;
  char pPulseLabel[IUF_MAX_HDF5_PATH];
  char pSourceLabel[IUF_MAX_HDF5_PATH];
  char pChannelMapLabel[IUF_MAX_HDF5_PATH];
  char pApodizationLabel[IUF_MAX_HDF5_PATH];
  char pDemodulationLabel[IUF_MAX_HDF5_PATH];

  status |= iufHdf5ReadString(handle, IUF_PATH_PATTERN_PULSELABEL, pPulseLabel);
  status |= iufHdf5ReadString(handle, IUF_PATH_PATTERN_SOURCELABEL, pSourceLabel);
  status |= iufHdf5ReadString(handle, IUF_PATH_PATTERN_CHANNELMAPLABEL, pChannelMapLabel);
  status |= iufHdf5ReadString(handle, IUF_PATH_PATTERN_APODIZATIONLABEL, pApodizationLabel);
  status |= iufHdf5ReadString(handle, IUF_PATH_PATTERN_DEMODULATIONLABEL, pDemodulationLabel);
  status |= iufHdf5ReadFloat( handle, IUF_PATH_PATTERN_TIMEINFRAME, &(timeInFrame));

  if( status < 0 )
    return NULL;

  return iufIqPatternCreate(timeInFrame,
                          pPulseLabel,
                          pSourceLabel,
                          pChannelMapLabel,
                          pApodizationLabel,
                          pDemodulationLabel);
}

const char * iufIqPatternGetPulseLabel
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NULL);
    return iqPattern->pPulseLabel;
}

const char * iufIqPatternGetSourceLabel
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NULL);
    return iqPattern->pSourceLabel;
}

const char * iufIqPatternGetChannelMapLabel
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NULL);
    return iqPattern->pChannelMapLabel;
}

const char * iufIqPatternGetApodizationLabel
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NULL);
    return iqPattern->pApodizationLabel;
}

const char * iufIqPatternGetDemodulationLabel
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NULL);
    return iqPattern->pDemodulationLabel;
}

float iufIqPatternGetTimeInFrame
(
    iuiqpa_t iqPattern
)
{
	IUF_ERR_CHECK_NULL_N_RETURN(iqPattern, NAN);
    return iqPattern->timeInFrame;
}

