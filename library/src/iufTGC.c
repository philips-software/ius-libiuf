
//
// Created by nlv09165 on 08/08/2018.
//
#include <stdlib.h>
#include <math.h>

#include <iuf.h>
#include <iufTGCPrivate.h>

struct IufTGC
{
    int     numTGCValues;
    float * pGains;
    float * pTimes;
} ;

// ADT
iutgc_t iufTGCCreate
(
    int numTGCValues
)
{
    IufTGC *tgc;
    if( numTGCValues < 0 )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
        "argument numTGCValues should be >= 0, but was '%d'", numTGCValues);
        return IUTGC_INVALID;
    }

    tgc = (IufTGC *) calloc (1,sizeof(IufTGC));
    IUF_ERR_ALLOC_NULL_N_RETURN(tgc, IufReceiveSettingsDict, IUTGC_INVALID);

    tgc->pTimes = (float *)calloc(numTGCValues, sizeof(float));
    tgc->pGains = (float *)calloc(numTGCValues, sizeof(float));
    if(tgc->pTimes == NULL || tgc->pGains == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for IufTGC members pTimes/pGains");
        return IUTGC_INVALID;
    }

    tgc->numTGCValues = numTGCValues;
    return tgc;
}

int iufTGCDelete
(
    iutgc_t tgc
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, IUF_ERR_VALUE);
    free(tgc->pGains);
    free(tgc->pTimes);
    free(tgc);
    return IUF_E_OK;
}


// operations
int iufTGCCompare
(
    iutgc_t reference,
    iutgc_t actual
)
{
    int i;
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->numTGCValues != actual->numTGCValues ) return IUF_FALSE;
    for(i=0; i< reference->numTGCValues ; i++)
    {
        if( IUF_EQUAL_FLOAT(reference->pTimes[i], actual->pTimes[i] ) == IUF_FALSE ) return IUF_FALSE;
        if( IUF_EQUAL_FLOAT(reference->pGains[i], actual->pGains[i] ) == IUF_FALSE ) return IUF_FALSE;
    }
    return IUF_TRUE;
}

// Getters
int iufTGCGetNumValues
(
    iutgc_t tgc
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, -1);
    return tgc->numTGCValues;
}


float iufTGCGetTime
(
    iutgc_t tgc,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, NAN);
    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
        "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return NAN;
    }
    return tgc->pTimes[index];
}

float iufTGCGetGain
(
    iutgc_t tgc,
    int index
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, NAN);
    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return NAN;
    }

    return tgc->pGains[index];
}

// Setters
int iufTGCSet
(
    iutgc_t tgc,
    int index,
    float time,
    float gain
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, IUF_ERR_VALUE);
    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_VALUE, IUF_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return IUF_ERR_VALUE;
    }

    tgc->pTimes[index] = time;
    tgc->pGains[index] = gain;
    return IUF_E_OK;
}

int iufTGCSave
(
    iutgc_t tgc,
    hid_t handle
)
{
    int status=IUF_E_OK;
    IUF_ERR_CHECK_NULL_N_RETURN(tgc, IUF_ERR_VALUE);
    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUF_ERR_VALUE);
    status |= iufHdf5WriteInt(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(tgc->numTGCValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = tgc->numTGCValues;
    status |= H5LTmake_dataset_float(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, 1, dims, tgc->pGains );
    status |= H5LTmake_dataset_float(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, 1, dims, tgc->pTimes );

    if (status != IUF_E_OK)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during write of tgc gains, times or count");
    }
	return status;
}

iutgc_t iufTGCLoad
(
    hid_t handle
)
{
    int status = IUF_E_OK;
    int  numTGCValues;
    iutgc_t  tgc;

    IUF_ERR_EVAL_N_RETURN(handle == H5I_INVALID_HID, IUTGC_INVALID);
    status |= iufHdf5ReadInt(handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(numTGCValues));
    if (status != IUF_E_OK)
    {
        IUF_ERROR_FMT_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during read of %s",
        IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES);
        return IUTGC_INVALID;
    }

    tgc = iufTGCCreate(numTGCValues);
    status |= H5LTread_dataset_float( handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, tgc->pTimes );
    status |= H5LTread_dataset_float( handle, IUF_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, tgc->pGains );
    if (status != IUF_E_OK)
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_HDF5, IUF_ERR_MIN_HDF5, "during read of tgc gains, times or count");
        return IUTGC_INVALID;
    }
    return tgc;
}


