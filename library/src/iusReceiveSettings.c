//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <ius.h>
#include <iusReceiveSettingsPrivate.h>
#include <iusTGCPrivate.h>

struct IusReceiveSettings
{
    iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
    float    sampleFrequency;       /**< The sampling frequency that was used */
    int      numDelays;           // should be identical to the number of pulses
    float   *startDelay;            /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
    int      numSamplesPerLine;     /**< length of an acquisition line */
} ;

// ADT
iurs_t iusReceiveSettingsCreateWithoutTGC
(
    float sampleFrequency,
    int numDelays,
    int numSamplesPerLine
)
{
    iurs_t created = calloc(1,sizeof(IusReceiveSettings));
    created->sampleFrequency = sampleFrequency;
    created->startDelay = (float *) calloc(numDelays, sizeof(float));
    created->numSamplesPerLine = numSamplesPerLine;
    created->numDelays = numDelays;
    return created;
}

iurs_t iusReceiveSettingsCreate
(
    float sampleFrequency,
    int numDelays,
    int numSamplesPerLine,
    int numTGCentries
)
{
    if( sampleFrequency <= 0.0f ) return IURS_INVALID;
    if( numDelays <= 0 ) return IURS_INVALID;
    if( numSamplesPerLine < 0 ) return IURS_INVALID;
    if( numTGCentries <= 0 ) return IURS_INVALID;
    iurs_t created = iusReceiveSettingsCreateWithoutTGC(sampleFrequency,numDelays,numSamplesPerLine);
    created->TGC = iusTGCCreate(numTGCentries);
    return created;
}

int iusReceiveSettingsDelete
(
    iurs_t iusReceiveSettings
)
{
    int status = IUS_ERR_VALUE;
    if(iusReceiveSettings != NULL)
    {
        iusTGCDelete(iusReceiveSettings->TGC);
        free(iusReceiveSettings->startDelay);
        free(iusReceiveSettings);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
)
{
    int i;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency ) == IUS_FALSE ) return IUS_FALSE;
    if( reference->numSamplesPerLine != actual->numSamplesPerLine ) return IUS_FALSE;
    for (i=0;i<reference->numDelays;i++)
    {
        if( IUS_EQUAL_FLOAT(reference->startDelay[i], actual->startDelay[i] ) == IUS_FALSE ) return IUS_FALSE;
    }
    return iusTGCCompare(reference->TGC,actual->TGC);
}

// Getters
iutgc_t iusReceiveSettingsGetTGC
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return IUTGC_INVALID;
    return iusReceiveSettings->TGC;
}

float iusReceiveSettingsGetSampleFrequency
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return NAN;
    return iusReceiveSettings->sampleFrequency;
}

int iusReceiveSettingsGetNumSamplesPerLine
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusReceiveSettings->numSamplesPerLine;
}

int iusReceiveSettingsGetNumDelays
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusReceiveSettings->numDelays;
}

int iusReceiveSettingsGetNumTGCentries
(
    iurs_t iusReceiveSettings
)
{
    if(iusReceiveSettings == NULL) return -1;
    return iusTGCGetNumValues(iusReceiveSettings->TGC);
}

float iusReceiveSettingsGetStartDelay
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
int iusReceiveSettingsSetStartDelay
(
    iurs_t iusReceiveSettings,
    int index,
    float delay
)
{
    if(iusReceiveSettings == NULL) return IUS_ERR_VALUE;
    if(index > iusReceiveSettings->numDelays) return IUS_ERR_VALUE;
    iusReceiveSettings->startDelay[index] = delay;
    return IUS_E_OK;
}



int iusReceiveSettingsSave
(
    iurs_t iusReceiveSettings,
    hid_t handle
)
{
	int status = 0;

    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &(iusReceiveSettings->sampleFrequency), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMDELAYS, &(iusReceiveSettings->numDelays), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &(iusReceiveSettings->numSamplesPerLine), 1);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, iusReceiveSettings->startDelay, iusReceiveSettings->numDelays);
	
	hid_t tgc_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusTGCSave( iusReceiveSettings->TGC, tgc_id);
	H5Gclose(tgc_id);
    
    return status;
}

iurs_t iusReceiveSettingsLoad
(
    hid_t handle
)
{
    float sampleFrequency;
    int numDelays;
    int numSamplesPerLine;
    int status=0;

    iutgc_t tgc;
    iurs_t iusReceiveSettings;
	
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &sampleFrequency);
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMDELAYS, &numDelays);
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &numSamplesPerLine);
    if ( status != 0 ) return IURS_INVALID;

	hid_t tgc_id = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT);
    tgc = iusTGCLoad(tgc_id);
	H5Gclose(tgc_id);
    if ( tgc == IUTGC_INVALID ) return IURS_INVALID;

    iusReceiveSettings = iusReceiveSettingsCreateWithoutTGC(sampleFrequency,numDelays,numSamplesPerLine);
    iusReceiveSettings->TGC = tgc;

    // Memory allocation for startDelays has been done in iusReceiveSettingsCreate
    // now, the delays can be read into iusReceiveSettings->startDelay.
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_STATRDELAY, iusReceiveSettings->startDelay);
    if ( status != 0 )
    {
        iusReceiveSettingsDelete(iusReceiveSettings);
        iusReceiveSettings = IURS_INVALID;
    }

    return iusReceiveSettings;
}
