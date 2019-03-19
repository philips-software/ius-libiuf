//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <iuf.h>
#include <iufTransducerElementPrivate.h>

// ADT
iute_t iufTransducerElementCreate
(
    IufShape shape
)
{
    if( shape ==  IUF_INVALID_SHAPE ) return IUTE_INVALID;
    iute_t created = calloc(1,sizeof(IufTransducerElement));
    IUF_ERR_ALLOC_NULL_N_RETURN(created, IufTransducerElement, IUTE_INVALID);
    created->shape = shape;
    return created;
}

int iufTransducerElementDelete
(
    iute_t transducerElement
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducerElement, IUF_ERR_VALUE);
    free(transducerElement);
    return IUF_E_OK;
}


// operations
int iufTransducerElementCompare
(
    iute_t reference,
    iute_t actual
)
{
    if( reference == actual ) return IUF_TRUE;
    if( reference == NULL || actual == NULL ) return IUF_FALSE;
    if( reference->shape != actual->shape ) return IUF_FALSE;
    return IUF_TRUE;
}

// Getters
IufShape iufTransducerElementGetShape
(
    iute_t transducerElement
)
{
    IUF_ERR_CHECK_NULL_N_RETURN(transducerElement, IUF_INVALID_SHAPE);
    return transducerElement->shape;
}
