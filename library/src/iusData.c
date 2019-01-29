
//
// Created by nlv09165 on 05/10/2018.
//
#include <stdlib.h>
#include <memory.h>

#include <ius.h>

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
    IUS_ERR_EVAL_N_RETURN(size <= 0, IUD_INVALID);
    iud_t created = calloc(1,sizeof(IusData));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusData, IUD_INVALID);
    created->size = size;
    created->pData = calloc((size_t)size,sizeof(float));
    if( created->pData == NULL )
    {
        IUS_ERROR_PUSH(IUS_ERR_MAJ_MEMORY, IUS_ERR_MIN_ALLOC, "calloc failed for pData member");
        free(created);
        created = IUD_INVALID;
    }
    return created;
}

int iusDataDelete
(
    iud_t data
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(data, IUS_ERR_VALUE);
    free(data->pData);
    free(data);
    return IUS_E_OK;
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
    iud_t data
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(data, -1);
    return data->size;
}

float *iusDataGetPointer
(
    iud_t data
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(data, NULL);
    return data->pData;
}
