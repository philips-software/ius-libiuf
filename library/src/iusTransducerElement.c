//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusTransducerElementPrivate.h>

// ADT
iute_t iusTransducerElementCreate
(
    IusShape shape
)
{
    if( shape ==  IUS_INVALID_SHAPE ) return IUTE_INVALID;
    iute_t created = calloc(1,sizeof(IusTransducerElement));
    IUS_ERR_ALLOC_NULL_N_RETURN(created, IusTransducerElement, IUTE_INVALID);
    created->shape = shape;
    return created;
}

int iusTransducerElementDelete
(
    iute_t transducerElement
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducerElement, IUS_ERR_VALUE);
    free(transducerElement);
    return IUS_E_OK;
}


// operations
int iusTransducerElementCompare
(
    iute_t reference,
    iute_t actual
)
{
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->shape != actual->shape ) return IUS_FALSE;
    return IUS_TRUE;
}

// Getters
IusShape iusTransducerElementGetShape
(
    iute_t transducerElement
)
{
    IUS_ERR_CHECK_NULL_N_RETURN(transducerElement, IUS_INVALID_SHAPE);
    return transducerElement->shape;
}
