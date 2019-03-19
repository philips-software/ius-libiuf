//
// Created by nlv09165 on 13/07/2018.
//
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <iuf.h>
#include <iufPatternPrivate.h>

struct IufPattern
{
    float timeInFrame;
    const char* pPulseLabel;
    const char* pSourceLabel;
    const char* pChannelMapLabel;
    const char* pApodizationLabel;
    const char* pReceiveSettingsLabel;
} ;


// ADT
iupa_t iufPatternCreate
(
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
)
{
    IUF_ERR_EVAL_N_RETURN(timeInFrame < 0.0f, IUPA_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(pPulseLabel, IUPA_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(pSourceLabel, IUPA_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(pChannelMapLabel, IUPA_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(pApodizationLabel, IUPA_INVALID);
    IUF_ERR_STRP_NULL_EMPTY(pReceiveSettingsLabel, IUPA_INVALID);

    iupa_t created = calloc(1, sizeof(IufPattern));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufPattern, IUPA_INVALID);

    created->timeInFrame = timeInFrame;
    created->pPulseLabel = strdup(pPulseLabel);
    created->pSourceLabel = strdup(pSourceLabel);
    created->pChannelMapLabel = strdup(pChannelMapLabel);
    created->pApodizationLabel = strdup(pApodizationLabel);
    created->pReceiveSettingsLabel = strdup(pReceiveSettingsLabel);
    return created;
}

int iufPatternDelete
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, IUF_ERR_VALUE);
    free((void *)pattern->pApodizationLabel);
    free((void *)pattern->pChannelMapLabel);
    free((void *)pattern->pPulseLabel);
    free((void *)pattern->pSourceLabel);
    free((void *)pattern->pReceiveSettingsLabel);
    free(pattern);
    return IUF_E_OK;
}


// operations
int iufPatternCompare
(
    iupa_t reference,
    iupa_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == IUPA_INVALID || actual == IUPA_INVALID ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->timeInFrame, actual->timeInFrame ) == IUF_FALSE ) return IUF_FALSE;

    if( strcmp(reference->pReceiveSettingsLabel,actual->pReceiveSettingsLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pApodizationLabel,actual->pApodizationLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pChannelMapLabel,actual->pChannelMapLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pSourceLabel,actual->pSourceLabel) != 0 ) return IUF_FALSE;
    if( strcmp(reference->pPulseLabel,actual->pPulseLabel) != 0 ) return IUF_FALSE;

    return IUF_TRUE;
}



int iufPatternSave
(
    iupa_t pattern,
    hid_t handle
)
{
    int status=IUF_E_OK;
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pattern->pPulseLabel);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pattern->pSourceLabel);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pattern->pChannelMapLabel);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pattern->pApodizationLabel);
    status |= iufHdf5WriteString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL, pattern->pReceiveSettingsLabel);
    status |= iufHdf5WriteFloat(handle,  IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(pattern->timeInFrame), 1);
    return status;
}


iupa_t iufPatternLoad
(
  hid_t handle
)
{
    int status = 0;
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPA_INVALID);

    float timeInFrame;
    char pPulseLabel[IUF_MAX_HDF5_PATH];
    char pSourceLabel[IUF_MAX_HDF5_PATH];
    char pChannelMapLabel[IUF_MAX_HDF5_PATH];
    char pApodizationLabel[IUF_MAX_HDF5_PATH];
    char pReceiveSettingsLabel[IUF_MAX_HDF5_PATH];

    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pPulseLabel);
    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pSourceLabel);
    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pChannelMapLabel);
    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pApodizationLabel);
    status |= iufHdf5ReadString(handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL, pReceiveSettingsLabel);
    status |= iufHdf5ReadFloat( handle, IUF_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(timeInFrame));

    if( status < 0 )
        return IUPA_INVALID;

    return iufPatternCreate(timeInFrame,
                          pPulseLabel,
                          pSourceLabel,
                          pChannelMapLabel,
                          pApodizationLabel,
                          pReceiveSettingsLabel);
}

const char * iufPatternGetPulseLabel
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pPulseLabel;
}

const char * iufPatternGetSourceLabel
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pSourceLabel;
}

const char * iufPatternGetChannelMapLabel
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pChannelMapLabel;
}

const char * iufPatternGetApodizationLabel
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pApodizationLabel;
}

const char * iufPatternGetReceivesettingsLabel
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pReceiveSettingsLabel;
}

float iufPatternGetTimeInFrame
(
    iupa_t pattern
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(pattern, NAN);
    return pattern->timeInFrame;
}

