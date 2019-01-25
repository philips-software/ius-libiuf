//
// Created by nlv09165 on 05/10/2018.
//
#include <stdlib.h>

#include <ius.h>

// ADT
iuo_t iusOffsetCreate
(
    void
)
{
    iuo_t created = calloc(1,sizeof(IusOffset));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusOffset, IUO_INVALID);
    created->x = 0;
    created->y = 0;
    created->z = 0;
    created->t = 0;
    return created;
}

int iusOffsetDelete
(
    iuo_t offset
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(offset, IUS_ERR_VALUE);
    free(offset);
    return IUS_E_OK;}


// operations
int iusOffsetCompare
(
    iuo_t reference,
    iuo_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->x != actual->x) return IUS_FALSE;
    if( reference->y != actual->y) return IUS_FALSE;
    if( reference->z != actual->z) return IUS_FALSE;
    if( reference->t != actual->t) return IUS_FALSE;
    return IUS_TRUE;
}

