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
    created->x = 0;
    created->y = 0;
    created->z = 0;
    created->t = 0;
    return created;
}

int iusOffsetDelete
(
    iuo_t iusOffset
)
{
    int status = IUS_ERR_VALUE;
    if(iusOffset != NULL)
    {
        free(iusOffset);
        status = IUS_E_OK;
    }
    return status;
}


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

