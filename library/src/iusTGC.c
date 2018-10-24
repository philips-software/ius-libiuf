
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

    if( numTGCValues < 0 ) return NULL;

    tgc = (IusTGC *) calloc (1,sizeof(IusTGC));
    if(tgc == NULL) return NULL;

    tgc->pTimes = (float *)calloc(numTGCValues, sizeof(float));
    tgc->pGains = (float *)calloc(numTGCValues, sizeof(float));
    if(tgc->pTimes == NULL || tgc->pGains == NULL ) return NULL;

    tgc->numTGCValues = numTGCValues;
    return tgc;
}

int iusTGCDelete
(
    iutgc_t iusTGC
)
{
    int status = IUS_ERR_VALUE;
    if(iusTGC != NULL)
    {
        free(iusTGC->pGains);
        free(iusTGC->pTimes);
        free(iusTGC);
        status = IUS_E_OK;
    }
    return status;
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
    iutgc_t iusTGC
)
{
    if ( iusTGC == NULL )
        return -1;
    return iusTGC->numTGCValues;
}


float iusTGCGetTime
(
    iutgc_t iusTGC,
    int index
)
{
    if( iusTGC == NULL ) return NAN;
    if( index < 0 || index >= iusTGC->numTGCValues )
        return NAN;
    return iusTGC->pTimes[index];
}

float iusTGCGetGain
(
    iutgc_t iusTGC,
    int index
)
{
    if( iusTGC == NULL ) return NAN;
    if( index < 0 || index >= iusTGC->numTGCValues )
        return NAN;
    return iusTGC->pGains[index];
}

// Setters
int iusTGCSet
(
    iutgc_t iusTGC,
    int index,
    float time,
    float gain
)
{
    if( iusTGC == NULL ) return IUS_ERR_VALUE;
    if( index < 0 || index >= iusTGC->numTGCValues )
        return IUS_ERR_VALUE;
    iusTGC->pTimes[index] = time;
    iusTGC->pGains[index] = gain;
    return IUS_E_OK;
}

int iusTGCSave
(
    iutgc_t iusTGC,
    hid_t handle
)
{
    int status=0;
    if( iusTGC == NULL )
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;

    status |= iusHdf5WriteInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(iusTGC->numTGCValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = iusTGC->numTGCValues;
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, 1, dims, iusTGC->pGains );
    status |= H5LTmake_dataset_float(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, 1, dims, iusTGC->pTimes );

	return status;
}

iutgc_t iusTGCLoad
(
    hid_t handle
)
{
    int status = 0;
    int  numTGCValues;
    iutgc_t  tgc;

    if(handle == H5I_INVALID_HID)
        return NULL;

    status |= iusHdf5ReadInt(handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_NUMTGCVALUES, &(numTGCValues));
    if( status < 0 )
        return NULL;

    tgc = iusTGCCreate(numTGCValues);
    status |= H5LTread_dataset_float( handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_TIMES, tgc->pTimes );
    status |= H5LTread_dataset_float( handle, IUS_INPUTFILE_PATH_RECEIVESETTINGS_TGC_GAINS, tgc->pGains );
    if( status < 0 )
        return NULL;
    return tgc;
}


