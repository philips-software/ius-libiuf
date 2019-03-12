
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
    IUS_ERR_EVAL_N_RETURN(timeInFrame < 0.0f, IUIQPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pPulseLabel, IUIQPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pSourceLabel, IUIQPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pChannelMapLabel, IUIQPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pApodizationLabel, IUIQPA_INVALID);
    IUS_ERR_STRP_NULL_EMPTY(pDemodulationLabel, IUIQPA_INVALID);
    iuiqpa_t created = calloc(1, sizeof(IusIqPattern));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusIqPattern, IUIQPA_INVALID);
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
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, IUS_ERR_VALUE);
    free((void *)pattern->pApodizationLabel);
    free((void *)pattern->pChannelMapLabel);
    free((void *)pattern->pPulseLabel);
    free((void *)pattern->pSourceLabel);
    free((void *)pattern->pDemodulationLabel);
    free(pattern);
    return IUS_E_OK;
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
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
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

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUIQPA_INVALID);

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
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pPulseLabel;
}

const char * iusIqPatternGetSourceLabel
(
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pSourceLabel;
}

const char * iusIqPatternGetChannelMapLabel
(
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pChannelMapLabel;
}

const char * iusIqPatternGetApodizationLabel
(
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pApodizationLabel;
}

const char * iusIqPatternGetDemodulationLabel
(
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NULL);
    return pattern->pDemodulationLabel;
}

float iusIqPatternGetTimeInFrame
(
    iuiqpa_t pattern
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(pattern, NAN);
    return pattern->timeInFrame;
}

