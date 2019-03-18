
//
// Created by nlv09165 on 05/10/2018.
//
#include <stdlib.h>
#include <memory.h>

#include <iuf.h>

struct IufData
{
    int size;
    float *pData;
} ;

// ADT
iud_t iufDataCreate
(
    int size
)
{
    IUF_ERR_EVAL_N_RETURN(size <= 0, IUD_INVALID);
    iud_t created = calloc(1,sizeof(IufData));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufData, IUD_INVALID);
    created->size = size;
    created->pData = calloc((size_t)size,sizeof(float));
    if( created->pData == NULL )
    {
        IUF_ERROR_PUSH(IUF_ERR_MAJ_MEMORY, IUF_ERR_MIN_ALLOC, "calloc failed for pData member");
        free(created);
        created = IUD_INVALID;
    }
    return created;
}

int iufDataDelete
(
    iud_t data
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(data, IUF_ERR_VALUE);
    free(data->pData);
    free(data);
    return IUF_E_OK;
}


// operations
int iufDataCompare
(
    iud_t reference,
    iud_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->size != actual->size ) return IUF_FALSE;
    if( memcmp(actual->pData, reference->pData, actual->size* sizeof(float)) != 0  ) return IUF_FALSE;
    return IUF_TRUE;
}

// Getters
int iufDataGetSize
(
    iud_t data
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(data, -1);
    return data->size;
}

float *iufDataGetPointer
(
    iud_t data
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(data, NULL);
    return data->pData;
}

int iufDataFill
(
    iud_t data,
    double *input,
    int length
)
{
    IUF_ERR_EVAL_N_RETURN(length != data->size, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(data, IUF_ERR_VALUE);
    IUF_ERR_CHECK_NULL_N_RETURN(input, IUF_ERR_VALUE);
    int j;
    for (j=0;j<length;j++)
    {
        data->pData[j] = (float) input[j];
    }
    return IUF_E_OK;
}
