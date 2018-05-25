//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>

#include <ius.h>
#include <iusError.h>
#include <iusTypes.h>
#include <iusUtil.h>
#include <iusHLTransducerElement.h>
#include <iusHLTransducerElementImp.h>

// ADT
iute_t iusHLTransducerElementCreate
(
    IusShape shape
)
{
    if( shape ==  IUS_INVALID_SHAPE ) return IUTE_INVALID;
    iute_t created = calloc(1,sizeof(IusTransducerElement));
    created->shape = shape;
    return created;
}

int iusHLTransducerElementDelete
(
    iute_t iusTransducerElement
)
{
    int status = IUS_ERR_VALUE;
    if(iusTransducerElement != NULL)
    {
        free(iusTransducerElement);
        iusTransducerElement = NULL;
        status = IUS_E_OK;
    }
    return status;
}


// operations
int iusHLTransducerElementCompare
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
IusShape iusHLTransducerElementGetShape
(
    iute_t element
)
{
    return element->shape;
}
