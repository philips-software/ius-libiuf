//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHLReceiveSettings.h>
#include <iusHLTGC.h>
#include <memory.h>
#include <math.h>
///** \brief Ultrasound recording settings (TGC and Fs) */
//typedef struct
//{
//    /**< Describes the mapping between transducer elemenents and receive pulses */
//    int   numChannels;                              /**< The number of simultaneous active elements per pulse */
//    // for indeces, see transmit apodization
//    // size should be addressed like pChannelMap[numDelays][numChannels]
//    int * pChannelMap;                              /**< A table of size numTransmitPulses x numChannels  at each index the transducer element that is mapped to a (channel,transmitPulse) */
//    int                  numSamplesPerLine;         /**< length of an acquisition line */
//    float                sampleFrequency;           /**< The sampling frequency that was used */
//    int                  numDelays ;                // should be identical to drivingscheme.numtransmitpulses
//    float *              pStartDelay;               /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
//    float *              pEndDelay;                 /**< The end delay of RFLines, this data can be considered redundant as it is startDepth+(sampleFrequency*numSamplesPerLine)  */
//    int                  numTimeGainControlValues;  /**< number of control points that describe the TGC */
//    IusTimeGainControl * pTimeGainControl;          /**< TimeGainControl points (time,gain) */
//} IusReceiveSettings;

struct IusReceiveSettings
{
    iutgc_t TGC;             /**< TimeGainControl points (time,gain) */
    char *pLabel;
    float sampleFrequency;   /**< The sampling frequency that was used */
    int  numDelays;           // should be identical to the number of pulses
    float *startDelay;       /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
    int numSamplesPerLine;   /**< length of an acquisition line */
} ;

// ADT
iurs_t iusHLReceiveSettingsCreate
(
    char *pLabel,
    float sampleFrequency,
    int numDelays,
    int numSamplesPerLine,
    int numTGCentries
)
{
    if( pLabel == NULL ) return NULL;
    if( strcmp(pLabel,"") == 0 ) return NULL;
    if( sampleFrequency <= 0.0f ) return IURS_INVALID;
    if( numDelays <= 0 ) return IURS_INVALID;
    if( numSamplesPerLine < 0 ) return IURS_INVALID;
    if( numTGCentries <= 0 ) return IURS_INVALID;

    iurs_t created = calloc(1,sizeof(IusReceiveSettings));
    created->pLabel = strdup(pLabel);
    created->sampleFrequency = sampleFrequency;
    created->startDelay = (float *) calloc(numDelays, sizeof(float));
    created->numSamplesPerLine = numSamplesPerLine;
    created->numDelays = numDelays;
    created->TGC = iusHLTGCCreate(numTGCentries);
    return created;
}

int iusHLReceiveSettingsDelete
(
    iurs_t iusReceiveSettings
)
{
    int status = IUS_ERR_VALUE;
    if(iusReceiveSettings != NULL)
    {
        free(iusReceiveSettings->pLabel);
        free(iusReceiveSettings->startDelay);
        free(iusReceiveSettings);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
)
{
    int i;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( strcmp(reference->pLabel,actual->pLabel) != 0 ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency ) == IUS_FALSE ) return IUS_FALSE;
    if( reference->numSamplesPerLine != actual->numSamplesPerLine ) return IUS_FALSE;
    for (i=0;i<reference->numDelays;i++)
    {
        if( IUS_EQUAL_FLOAT(reference->startDelay[i], actual->startDelay[i] ) == IUS_FALSE ) return IUS_FALSE;
    }
    return iusHLTGCCompare(reference->TGC,actual->TGC);
}

// Getters
iutgc_t iusHLReceiveSettingsGetTGC
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return IUTGC_INVALID;
    return iusReceiveSettings->TGC;
}

char *iusHLReceiveSettingsGetLabel
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return NULL;
    return iusReceiveSettings->pLabel;
}

float iusHLReceiveSettingsGetSampleFrequency
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return NAN;
    return iusReceiveSettings->sampleFrequency;
}

int iusHLReceiveSettingsGetNumSamplesPerLine
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusReceiveSettings->numSamplesPerLine;
}

int iusHLReceiveSettingsGetNumDelays
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusReceiveSettings->numDelays;
}

int iusHLReceiveSettingsGetNumTGCentries
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusHLTGCGetNumValues(iusReceiveSettings->TGC);
}

float iusHLReceiveSettingsGetStartDelay
(
    iurs_t iusReceiveSettings,
    int index
)
{
    if(iusReceiveSettings == NULL) return -1;
    if(index > iusReceiveSettings->numDelays) return NAN;
    return iusReceiveSettings->startDelay[index];
}


// Setters
int iusHLReceiveSettingsSetStartDelay
(
    iurs_t iusReceiveSettings,
    int index,
    float delay
)
{
    int status = IUS_ERR_VALUE;
    if(iusReceiveSettings == NULL) return IUS_ERR_VALUE;
    if(index > iusReceiveSettings->numDelays) return IUS_ERR_VALUE;

    iusReceiveSettings->startDelay[index] = delay;
    return IUS_E_OK;
}

