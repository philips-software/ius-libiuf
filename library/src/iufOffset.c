//
// Created by nlv09165 on 05/10/2018.
//
#include <stdlib.h>

#include <iuf.h>

// ADT
iuo_t iufOffsetCreate
(
    void
)
{
    iuo_t created = calloc(1,sizeof(IufOffset));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufOffset, IUO_INVALID);
    created->x = 0;
    created->y = 0;
    created->z = 0;
    created->t = 0;
    return created;
}

int iufOffsetDelete
(
    iuo_t offset
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(offset, IUF_ERR_VALUE);
    free(offset);
    return IUF_E_OK;}


// operations
int iufOffsetCompare
(
    iuo_t reference,
    iuo_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->x != actual->x) return IUF_FALSE;
    if( reference->y != actual->y) return IUF_FALSE;
    if( reference->z != actual->z) return IUF_FALSE;
    if( reference->t != actual->t) return IUF_FALSE;
    return IUF_TRUE;
}

