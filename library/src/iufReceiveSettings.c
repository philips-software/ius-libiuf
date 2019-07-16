#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <iuf.h>
#include <iufReceiveSettingsPrivate.h>
#include <iufTGCPrivate.h>

struct IufReceiveSettings
{
    iutgc_t  TGC;                   /**< TimeGainControl points (time,gain) */
    float    sampleFrequency;       /**< The sampling frequency that was used */
    int      numSamplesPerLine;     /**< length of an acquisition line */
} ;

// ADT
iurs_t iufReceiveSettingsCreateWithoutTGC
(
    float sampleFrequency,
    int numSamplesPerLine
)
{
    iurs_t created = calloc(1,sizeof(IufReceiveSettings));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufReceiveSettings, IURS_INVALID);
    created->sampleFrequency = sampleFrequency;
    created->numSamplesPerLine = numSamplesPerLine;
    return created;
}

iurs_t iufReceiveSettingsCreate
(
    float sampleFrequency,
    int numSamplesPerLine,
    int numTGCentries
)
{
    if( sampleFrequency <= 0.0f )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
        "sampleFrequency argument should be > 0.0, but was: '%f'", sampleFrequency);
        return IURS_INVALID;
    }

    if( numSamplesPerLine < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "numSamplesPerLine argument should be >= 0, but was: '%d'", numSamplesPerLine);
        return IURS_INVALID;
    }

    if( numTGCentries <= 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "numTGCentries argument should be > 0, but was: '%d'", numTGCentries);
        return IURS_INVALID;
    }

    iurs_t created = iufReceiveSettingsCreateWithoutTGC(sampleFrequency,numSamplesPerLine);
    created->TGC = iufTGCCreate(numTGCentries);
    return created;
}

int iufReceiveSettingsDelete
(
    iurs_t receiveSettings
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUF_ERR_VALUE);
    iufTGCDelete(receiveSettings->TGC);
    free(receiveSettings);
    return IUF_E_OK;
}


// operations
int iufReceiveSettingsCompare
(
    iurs_t reference,
    iurs_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( IUF_EQUAL_FLOAT(reference->sampleFrequency, actual->sampleFrequency ) == IUF_FALSE ) return IUF_FALSE;
    if( reference->numSamplesPerLine != actual->numSamplesPerLine ) return IUF_FALSE;
    return iufTGCCompare(reference->TGC,actual->TGC);
}

// Getters
iutgc_t iufReceiveSettingsGetTGC
(
    iurs_t receiveSettings
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUTGC_INVALID);
    return receiveSettings->TGC;
}

float iufReceiveSettingsGetSampleFrequency
(
    iurs_t receiveSettings
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, NAN);
    return receiveSettings->sampleFrequency;
}

int iufReceiveSettingsGetNumSamplesPerLine
(
    iurs_t receiveSettings
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUF_ERR_VALUE);
    return receiveSettings->numSamplesPerLine;
}

int iufReceiveSettingsGetNumTGCentries
(
    iurs_t receiveSettings
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUF_ERR_VALUE);
    return iufTGCGetNumValues(receiveSettings->TGC);
}



int iufReceiveSettingsSave
(
    iurs_t receiveSettings,
    hid_t handle
)
{
	int status = 0;
    IUF_ERR_CHECK_NULL_N_RETURN(receiveSettings, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufHdf5WriteFloat(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &(receiveSettings->sampleFrequency), 1);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &(receiveSettings->numSamplesPerLine), 1);
    if (status != 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "write failed for %s and/or %s",
                           IUF_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY,
                           IUF_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE);
        return IUF_ERR_VALUE;
    }

    hid_t tgc_id = H5Gcreate(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status |= iufTGCSave( receiveSettings->TGC, tgc_id);
	H5Gclose(tgc_id);
    return status;
}

iurs_t iufReceiveSettingsLoad
(
    hid_t handle
)
{
    float sampleFrequency;
    int numSamplesPerLine;
    int status=0;

    iutgc_t tgc;
    iurs_t iufReceiveSettings;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IURS_INVALID);
    status |= iufHdf5ReadFloat(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY, &sampleFrequency);
    status |= iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE, &numSamplesPerLine);
    if (status != 0)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "read failed for %s or %s",
        IUF_INPUTFILE_PATH_RECEIVESETTINGS_SAMPLEFREQUENCY,
        IUF_INPUTFILE_PATH_RECEIVESETTINGS_NUMSAMPLESPERLINE);
        return IURS_INVALID;
    }

	hid_t tgc_id = H5Gopen(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC, H5P_DEFAULT);
    tgc = iufTGCLoad(tgc_id);
	H5Gclose(tgc_id);
    if ( tgc == IUTGC_INVALID ) return IURS_INVALID;

    iufReceiveSettings = iufReceiveSettingsCreateWithoutTGC(sampleFrequency,numSamplesPerLine);
    iufReceiveSettings->TGC = tgc;

    return iufReceiveSettings;
}
