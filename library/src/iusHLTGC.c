
//
// Created by nlv09165 on 08/08/2018.
//
#include <stdlib.h>
#include <math.h>
#include <memory.h>

//#include <hdf5_hl.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHDF5.h>
#include <iusHLTGCImp.h>
#include <hdf5_hl.h>

struct IusTGC
{
    int     numTGCValues;
    float * pGains;
    float * pTimes;
} ;

// ADT
iutgc_t iusHLTGCCreate
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

int iusHLTGCDelete
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
int iusHLTGCCompare
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
int iusHLTGCGetNumValues
(
    iutgc_t iusTGC
)
{
    if ( iusTGC == NULL )
        return -1;
    return iusTGC->numTGCValues;
}


float iusHLTGCGetTime
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

float iusHLTGCGetGain
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

int iusHLTGCSet
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


#define NUMTGCVALUES  "numTGCValues"
#define TGCTIMES      "Times"
#define TGCGAINS      "Gains"


int iusHLTGCSave
(
    iutgc_t iusTGC,
    hid_t handle
)
{
    int status=0;
    //char path[IUS_MAX_HDF5_PATH];
    if( iusTGC == NULL )
        return IUS_ERR_VALUE;
    if(handle == H5I_INVALID_HID)
        return IUS_ERR_VALUE;

	//hid_t group_id = H5Gcreate(handle, parentPath, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //sprintf(path, NUMTGCVALUESFMT, parentPath);
    status |= iusHdf5WriteInt(handle, NUMTGCVALUES, &(iusTGC->numTGCValues), 1);

    hsize_t dims[1] = { 1 };
    dims[0] = iusTGC->numTGCValues;
    //sprintf(path, TGCGAINSFMT, parentPath);
    status |= H5LTmake_dataset_float(handle, TGCGAINS, 1, dims, iusTGC->pGains );
    //sprintf(path, TGCTIMESFMT, parentPath);
    status |= H5LTmake_dataset_float(handle, TGCTIMES, 1, dims, iusTGC->pTimes );
    //status |= H5Gclose(group_id );
    return status;
}

iutgc_t iusHLTGCLoad
(
    hid_t handle
)
{
    int status = 0;
    //char path[IUS_MAX_HDF5_PATH];
    int  numTGCValues;
    iutgc_t  tgc;

    if(handle == H5I_INVALID_HID)
        return NULL;

    //sprintf(path, NUMTGCVALUESFMT, parentPath);
    status |= iusHdf5ReadInt(handle, NUMTGCVALUES, &(numTGCValues));
    if( status < 0 )
        return NULL;

    tgc = iusHLTGCCreate(numTGCValues);
    //sprintf(path, TGCTIMESFMT, parentPath);
    status |= H5LTread_dataset_float( handle, TGCTIMES, tgc->pTimes );
    //sprintf(path, TGCGAINSFMT, parentPath);
    status |= H5LTread_dataset_float( handle, TGCGAINS, tgc->pGains );
    if( status < 0 )
        return NULL;
    return tgc;
}


