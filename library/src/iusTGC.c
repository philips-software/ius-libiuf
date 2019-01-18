
//
// Created by nlv09165 on 08/08/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusTGCPrivate.h>

struct IusTGC
{
    int     numTGCValues;
    float * pGains;
    float * pTimes;
} ;

// ADT
iutgc_t iusTGCCreate
(
    int numTGCValues
)
{
    IusTGC *tgc;
    if( numTGCValues < 0 )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
        "argument numTGCValues should be >= 0, but was '%d'", numTGCValues);
        return IUTGC_INVALID;
    }

    tgc = (IusTGC *) calloc (1,sizeof(IusTGC));
    if(tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for IusTGC");
        return IUTGC_INVALID;
    }

    tgc->pTimes = (float *)calloc(numTGCValues, sizeof(float));
    tgc->pGains = (float *)calloc(numTGCValues, sizeof(float));
    if(tgc->pTimes == NULL || tgc->pGains == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for IusTGC members pTimes/pGains");
        return IUTGC_INVALID;
    }

    tgc->numTGCValues = numTGCValues;
    return tgc;
}

int iusTGCDelete
(
    iutgc_t tgc
)
{
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return IUS_ERR_VALUE;
    }

    free(tgc->pGains);
    free(tgc->pTimes);
    free(tgc);
    return IUS_E_OK;
}


// operations
int iusTGCCompare
(
    iutgc_t reference,
    iutgc_t actual
)
{
    int i;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->numTGCValues != actual->numTGCValues ) return IUS_FALSE;
    for(i=0; i< reference->numTGCValues ; i++)
    {
        if( IUS_EQUAL_FLOAT(reference->pTimes[i], actual->pTimes[i] ) == IUS_FALSE ) return IUS_FALSE;
        if( IUS_EQUAL_FLOAT(reference->pGains[i], actual->pGains[i] ) == IUS_FALSE ) return IUS_FALSE;
    }
    return IUS_TRUE;
}

// Getters
int iusTGCGetNumValues
(
    iutgc_t tgc
)
{
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return -1;
    }

    return tgc->numTGCValues;
}


float iusTGCGetTime
(
    iutgc_t tgc,
    int index
)
{
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return NAN;
    }

    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
        "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return NAN;
    }
    return tgc->pTimes[index];
}

float iusTGCGetGain
(
    iutgc_t tgc,
    int index
)
{
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return NAN;
    }

    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return NAN;
    }

    return tgc->pGains[index];
}

// Setters
int iusTGCSet
(
    iutgc_t tgc,
    int index,
    float time,
    float gain
)
{
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return IUS_ERR_VALUE;
    }

    if( index < 0 || index >= tgc->numTGCValues )
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE,
                           "index >= 0 && index < %d but was '%d'", tgc->numTGCValues, index);
        return IUS_ERR_VALUE;
    }

    tgc->pTimes[index] = time;
    tgc->pGains[index] = gain;
    return IUS_E_OK;
}

int iusTGCSave
(
    iutgc_t tgc,
    hid_t handle
)
{
    int status=IUS_E_OK;
    if (tgc == NULL)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_NULL_VALUE, "argument tgc was NULL");
        return IUS_ERR_VALUE;
    }

    if (handle == H5I_INVALID_HID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument handle was invalid");
        return IUS_ERR_VALUE;
    }

    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(tgc->numTGCValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = tgc->numTGCValues;
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, 1, dims, tgc->pGains );
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, 1, dims, tgc->pTimes );

    if (status != IUS_E_OK)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during write of tgc gains, times or count");
    }
	return status;
}

iutgc_t iusTGCLoad
(
    hid_t handle
)
{
    int status = IUS_E_OK;
    int  numTGCValues;
    iutgc_t  tgc;

    if (handle == H5I_INVALID_HID)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_VALUE, IUS_ERR_MIN_ARG_VALUE, "argument handle was invalid");
        return IUTGC_INVALID;
    }

    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(numTGCValues));
    if (status != IUS_E_OK)
    {
        IUS_ERROR_FMT_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during read of %s",
        IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES);
        return IUTGC_INVALID;
    }

    tgc = iusTGCCreate(numTGCValues);
    status |= H5LTread_dataset_float( handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, tgc->pTimes );
    status |= H5LTread_dataset_float( handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, tgc->pGains );
    if (status != IUS_E_OK)
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_HDF5, IUS_ERR_MIN_HDF5, "during read of tgc gains, times or count");
        return IUTGC_INVALID;
    }
    return tgc;
}


