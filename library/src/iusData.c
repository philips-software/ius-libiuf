
//
// Created by nlv09165 on 05/10/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <memory.h>

#include "include/iusData.h"

struct IusData
{
    int size;
    float *pData;
} ;

// ADT
iud_t iusDataCreate
(
    int size
)
{
    if( size < 0 ) return IUD_INVALID;
    iud_t created = calloc(1,sizeof(IusData));
    created->size = size;
    created->pData = calloc(size,sizeof(float));
    return created;
}

int iusDataDelete
(
    iud_t iusData
)
{
    int status = IUS_ERR_VALUE;
    if(iusData != NULL)
    {
        free(iusData->pData);
        free(iusData);
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusDataCompare
(
    iud_t reference,
    iud_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->size != actual->size ) return IUS_FALSE;
    if( memcmp(actual->pData, reference->pData, actual->size* sizeof(float)) != 0  ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
int iusDataGetSize
(
    iud_t iusData
)
{
    if( iusData == NULL ) return -1;
    return iusData->size;
}

float *iusDataGetPointer
(
    iud_t iusData
)
{
    if( iusData == NULL ) return NULL;
    return iusData->pData;
}
