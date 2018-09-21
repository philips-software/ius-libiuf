//
// Created by nlv09165 on 25/04/2018.
//
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHLReceiveSettingsImp.h>
#include <iusHLTGCImp.h>
#include <include/iusHDF5.h>

struct IusReceiveSettings
{
    iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
    char    *pLabel;
    float    sampleFrequency;       /**< The sampling frequency that was used */
    int      numDelays;           // should be identical to the number of pulses
    float   *startDelay;            /**< The start delay of RFlines, array length is the number of pulses per frame, values are in seconds */
    int      numSamplesPerLine;     /**< length of an acquisition line */
} ;

#define TGCFMT "TGC"
#define LABELFMT "Label"
#define SAMPLEFREQUENCYFMT "sampleFrequency"
#define NUMDELAYSFMT "numDelays"
#define STARTDELAYFMT "startDelay"
#define NUMSAMPLESPERLINEFMT "numSamplesPerLine"

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
	created->pLabel = NULL;
	created->pLabel = strdup(pLabel);
	if (created->pLabel == NULL)
	{
		return NULL;
	}
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
    if(iusReceiveSettings == NULL) return IUS_ERR_VALUE;
    if(index > iusReceiveSettings->numDelays) return IUS_ERR_VALUE;
    iusReceiveSettings->startDelay[index] = delay;
    return IUS_E_OK;
}



int iusHLReceiveSettingsSave
(
    iurs_t iusReceiveSettings,
    hid_t handle
)
{
	hid_t receiveSettingsId;
	int status = H5Gget_objinfo(handle, "ReceiveSettings", 0, NULL); // todo centralize the path "Sources"
	if (status != IUS_E_OK) // the group does not exist yet
	{
		receiveSettingsId = H5Gcreate(handle, "ReceiveSettings", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	}
	else
	{
		receiveSettingsId = H5Gopen(handle, "ReceiveSettings", H5P_DEFAULT);
	}
	hid_t label_id = H5Gcreate(receiveSettingsId, iusReceiveSettings->pLabel, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //char path[IUS_MAX_HDF5_PATH];

    //sprintf(path, LABELFMT, parentPath);
    status |= iusHdf5WriteString(label_id, LABELFMT, iusReceiveSettings->pLabel, 1);
    //sprintf(path, SAMPLEFREQUENCYFMT, parentPath);
    status |= iusHdf5WriteFloat(label_id, SAMPLEFREQUENCYFMT, &(iusReceiveSettings->sampleFrequency), 1, 1);
    //sprintf(path, NUMDELAYSFMT, parentPath);
    status |= iusHdf5WriteInt(label_id, NUMDELAYSFMT, &(iusReceiveSettings->numDelays), 1);
    //sprintf(path, NUMSAMPLESPERLINE, parentPath);
    status |= iusHdf5WriteInt(label_id, NUMSAMPLESPERLINEFMT, &(iusReceiveSettings->numSamplesPerLine), 1);
    //sprintf(path, STARTDELAYFMT, parentPath);
    status |= iusHdf5WriteFloat(label_id, STARTDELAYFMT, iusReceiveSettings->startDelay, iusReceiveSettings->numDelays, 1);
    //sprintf(path, TGCFMT, parentPath);
    status |= iusHLTGCSave( iusReceiveSettings->TGC, TGCFMT, label_id );
    
	status |= H5Gclose(label_id );
	status |= H5Gclose(receiveSettingsId);

    return status;
}

iurs_t iusHLReceiveSettingsLoad
(
    hid_t handle,
    char *label
)
{
    float sampleFrequency;
    int numDelays;
    int numSamplesPerLine;
    int numTGCentries;
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
	
    iutgc_t tgc;
    iurs_t iusReceiveSettings;
	
	hid_t settings_id = H5Gopen(handle, "ReceiveSettings", H5P_DEFAULT);
	hid_t label_id = H5Gopen(settings_id, label, H5P_DEFAULT);

    //sprintf(path, SAMPLEFREQUENCYFMT, parentPath);
    status |= iusHdf5ReadFloat(label_id, SAMPLEFREQUENCYFMT, &sampleFrequency);
    //sprintf(path, NUMDELAYSFMT, parentPath);
    status |= iusHdf5ReadInt(label_id, NUMDELAYSFMT, &numDelays);
    //sprintf(path, NUMSAMPLESPERLINE, parentPath);
    status |= iusHdf5ReadInt(label_id, NUMSAMPLESPERLINEFMT, &numSamplesPerLine);
    if ( status != 0 ) return IURS_INVALID;

    //sprintf(path, TGCFMT, parentPath);
    tgc = iusHLTGCLoad(label_id, TGCFMT);
    if ( tgc == IUTGC_INVALID ) return IURS_INVALID;
    numTGCentries = iusHLTGCGetNumValues(tgc);
    iusReceiveSettings = iusHLReceiveSettingsCreate(label,sampleFrequency,numDelays,numSamplesPerLine,numTGCentries);

    //sprintf(path, STARTDELAYFMT, parentPath); 
    status |= iusHdf5ReadFloat(handle, STARTDELAYFMT, iusReceiveSettings->startDelay); //todo: understand why startDelay is not in iusHLReceiveSettingsCreate 
    if ( status != 0 )
    {
        iusHLReceiveSettingsDelete(iusReceiveSettings);
        iusReceiveSettings = IURS_INVALID;
    }
	H5Gclose(label_id);
	H5Gclose(settings_id);

    return iusReceiveSettings;
}
