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
    int      numSamplesPerLine;     /**< length of an acquisition line */
} ;

// ADT
iurs_t iusReceiveSettingsCreateWithoutTGC
(
    float sampleFrequency,
    int numSamplesPerLine
)
{
    iurs_t created = calloc(1,sizeof(IusReceiveSettings));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusReceiveSettings, IURS_INVALID);
    created->sampleFrequency = sampleFrequency;
    created->numSamplesPerLine = numSamplesPerLine;
    return created;
}

iurs_t iusReceiveSettingsCreate
(
    float sampleFrequency,
    int numSamplesPerLine,
    int numTGCentries
)
{
    if( sampleFrequency <= 0.0f )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
        "sampleFrequency argument should be > 0.0, but was: '%f'", sampleFrequency);
        return IURS_INVALID;
    }

    if( numSamplesPerLine < 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numSamplesPerLine argument should be >= 0, but was: '%d'", numSamplesPerLine);
        return IURS_INVALID;
    }

    if( numTGCentries <= 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "numTGCentries argument should be > 0, but was: '%d'", numTGCentries);
        return IURS_INVALID;
    }

    iurs_t created = iusReceiveSettingsCreateWithoutTGC(sampleFrequency,numSamplesPerLine);
    created->TGC = iusTGCCreate(numTGCentries);
    return created;
}

int iusReceiveSettingsDelete
(
    iurs_t receiveSettings
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUS_ERR_VALUE);
    iusTGCDelete(receiveSettings->TGC);
    free(receiveSettings);
    return IUS_E_OK;
}


// operations
int iusReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( IUS_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency ) == IUS_FALSE ) return IUS_FALSE;
    if( reference->numSamplesPerLine != actual->numSamplesPerLine ) return IUS_FALSE;
    return iusTGCCompare(reference->TGC,actual->TGC);
}

// Getters
iutgc_t iusReceiveSettingsGetTGC
(
    iurs_t receiveSettings
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUTGC_INVALID);
    return receiveSettings->TGC;
}

float iusReceiveSettingsGetSampleFrequency
(
    iurs_t receiveSettings
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, NAN);
    return receiveSettings->sampleFrequency;
}

int iusReceiveSettingsGetNumSamplesPerLine
(
    iurs_t receiveSettings
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUS_ERR_VALUE);
    return receiveSettings->numSamplesPerLine;
}

int iusReceiveSettingsGetNumTGCentries
(
    iurs_t receiveSettings
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUS_ERR_VALUE);
    return iusTGCGetNumValues(receiveSettings->TGC);
}



int iusReceiveSettingsSave
(
    iurs_t receiveSettings,
    hid_t handle
)
{
	int status = 0;
    IUS_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUS_ERR_VALUE);
    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUS_ERR_VALUE);
    status |= iusHdf5WriteFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &(receiveSettings->sampleFrequency), 1);
    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &(receiveSettings->numSamplesPerLine), 1);
    if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "write failed for %s and/or %s",
                           IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY,
                           IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE);
        return IUS_ERR_VALUE;
    }

    hid_t tgc_id = H5Gcreate(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iusTGCSave( receiveSettings->TGC, tgc_id);
	H5Gclose(tgc_id);
    return status;
}

iurs_t iusReceiveSettingsLoad
(
    hid_t handle
)
{
    float sampleFrequency;
    int numSamplesPerLine;
    int status=0;

    iutgc_t tgc;
    iurs_t iusReceiveSettings;

    IUS_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IURS_INVALID);
    status |= iusHdf5ReadFloat(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &sampleFrequency);
    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &numSamplesPerLine);
    if (status != 0)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "read failed for %s or %s",
        IUS_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY,
        IUS_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE);
        return IURS_INVALID;
    }

	hid_t tgc_id = H5Gopen(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT);
    tgc = iusTGCLoad(tgc_id);
	H5Gclose(tgc_id);
    if ( tgc == IUTGC_INVALID ) return IURS_INVALID;

    iusReceiveSettings = iusReceiveSettingsCreateWithoutTGC(sampleFrequency,numSamplesPerLine);
    iusReceiveSettings->TGC = tgc;

    return iusReceiveSettings;
}
