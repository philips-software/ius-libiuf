//
// Created by nlv09165 on 13/07/2018.
//
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <ius.h>
#include <iusPatternPrivate.h>

struct IusPattern
{
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
    float timeInFrame,
    const char *pPulseLabel,
    const char *pSourceLabel,
    const char *pChannelMapLabel,
    const char *pApodizationLabel,
    const char *pReceiveSettingsLabel
)
{
    IUS_ERR_EVAL_N_RETURN(timeInFrame < 0.0f, IUPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pPulseLabel, IUPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pSourceLabel, IUPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pChannelMapLabel, IUPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pApodizationLabel, IUPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pReceiveSettingsLabel, IUPA_INVALID);

    iupa_t created = calloc(1, sizeof(IusPattern));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusPattern, IUPA_INVALID);

    created->timeInFrame = timeInFrame;
    created->pPulseLabel = strdup(pPulseLabel);
    created->pSourceLabel = strdup(pSourceLabel);
    created->pChannelMapLabel = strdup(pChannelMapLabel);
    created->pApodizationLabel = strdup(pApodizationLabel);
    created->pReceiveSettingsLabel = strdup(pReceiveSettingsLabel);
    return created;
}

int iusPatternDelete
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, IUS_ERR_VALUE);
    free((void *)pattern->pApodizationLabel);
    free((void *)pattern->pChannelMapLabel);
    free((void *)pattern->pPulseLabel);
    free((void *)pattern->pSourceLabel);
    free((void *)pattern->pReceiveSettingsLabel);
    free(pattern);
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
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
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
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUPA_INVALID);

    float timeInFrame;
    char pPulseLabel[IUS_MAX_HDF5_PATH];
    char pSourceLabel[IUS_MAX_HDF5_PATH];
    char pChannelMapLabel[IUS_MAX_HDF5_PATH];
    char pApodizationLabel[IUS_MAX_HDF5_PATH];
    char pReceiveSettingsLabel[IUS_MAX_HDF5_PATH];

    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_PULSELABEL, pPulseLabel);
    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_SOURCELABEL, pSourceLabel);
    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_CHANNELMAPLABEL, pChannelMapLabel);
    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_APODIZATIONLABEL, pApodizationLabel);
    status |= iusHdf5ReadString(handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_RECEIVESETTINGSLABEL, pReceiveSettingsLabel);
    status |= iusHdf5ReadFloat( handle, IUS_INPUTFILE_PATH_PATTERNLIST_PATTERN_TIMEINFRAME, &(timeInFrame));

    if( status < 0 )
        return IUPA_INVALID;

    return iusPatternCreate(timeInFrame,
                          pPulseLabel,
                          pSourceLabel,
                          pChannelMapLabel,
                          pApodizationLabel,
                          pReceiveSettingsLabel);
}

const char * iusPatternGetPulseLabel
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pPulseLabel;
}

const char * iusPatternGetSourceLabel
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pSourceLabel;
}

const char * iusPatternGetChannelMapLabel
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pChannelMapLabel;
}

const char * iusPatternGetApodizationLabel
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pApodizationLabel;
}

const char * iusPatternGetReceivesettingsLabel
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pReceiveSettingsLabel;
}

float iusPatternGetTimeInFrame
(
    iupa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NAN);
    return pattern->timeInFrame;
}

