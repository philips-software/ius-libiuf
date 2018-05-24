


//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <math.h>
#include <include/iusHL2DTransducerElementList.h>
#include <include/ius.h>
#include <include/iusError.h>
#include <include/iusUtil.h>


// ADT
struct Ius2DTransducerElementList
{
    int count;
    iu2dte_t *   p2DTransducerElements ;
} ;

// ADT

iu2dtel_t iusHL2DTransducerElementListCreate
(
    int num2DTransducerElements
)
{
    iu2dtel_t list = calloc(1, sizeof(Ius2DTransducerElementList));
    if(list!=NULL)
    {
        list->count = num2DTransducerElements;
        list->p2DTransducerElements = (iu2dte_t *) calloc((size_t)num2DTransducerElements, sizeof(iu2dte_t));
        if( list->p2DTransducerElements == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusHL2DTransducerElementListDelete
(
    iu2dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHL2DTransducerElementListCompare
(
    iu2dtel_t reference,
    iu2dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusHL2DTransducerElementCompare( reference->p2DTransducerElements[index], actual->p2DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHL2DTransducerElementListGetSize
(
    iu2dtel_t list
)
{
    return list->count;
}

iu2dte_t iusHL2DTransducerElementListGet
(
    iu2dtel_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->p2DTransducerElements[index];
}

int iusHL2DTransducerElementListSet
(
    iu2dtel_t list,
    iu2dte_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->p2DTransducerElements[index] = member;
    return IUS_E_OK;
}