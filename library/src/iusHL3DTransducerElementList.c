//
// Created by nlv09165 on 23/05/2018.
//
#include <stdlib.h>
#include <math.h>

#include <ius.h>
#include <iusError.h>
#include <iusUtil.h>
#include <iusHL3DTransducerElementList.h>


// ADT
struct Ius3DTransducerElementList
{
    int count;
    iu3dte_t *   p3DTransducerElements ;
} ;

// ADT

iu3dtel_t iusHL3DTransducerElementListCreate
(
    int num3DTransducerElements
)
{
    iu3dtel_t list = calloc(1, sizeof(Ius3DTransducerElementList));
    if(list!=NULL)
    {
        list->count = num3DTransducerElements;
        list->p3DTransducerElements = (iu3dte_t *) calloc((size_t)num3DTransducerElements, sizeof(iu3dte_t));
        if( list->p3DTransducerElements == NULL )
        {
            free(list);
            list = NULL;
        }
    }
    return list;
}

int iusHL3DTransducerElementListDelete
(
    iu3dtel_t list
)
{
    if(list == NULL) return IUS_ERR_VALUE;
    free(list);
    return IUS_E_OK;
}


// operations
int iusHL3DTransducerElementListCompare
(
    iu3dtel_t reference,
    iu3dtel_t actual
)
{
    int index;
    if( reference == actual ) return IUS_TRUE;
    if( reference == NULL || actual == NULL ) return IUS_FALSE;
    if( reference->count != actual->count ) return IUS_FALSE;
    for(index = 0 ; index < actual->count ; index++ )
    {
        if( iusHL3DTransducerElementCompare( reference->p3DTransducerElements[index], actual->p3DTransducerElements[index] )
            == IUS_FALSE )
            return IUS_FALSE;
    }
    return IUS_TRUE;
}


int iusHL3DTransducerElementListGetSize
(
    iu3dtel_t list
)
{
    return list->count;
}

iu3dte_t iusHL3DTransducerElementListGet
(
    iu3dtel_t list,
    int index
)
{
    if( index < 0 ) return NULL;
    if( list == NULL || index >= list->count ) return NULL;
    return list->p3DTransducerElements[index];
}

int iusHL3DTransducerElementListSet
(
    iu3dtel_t list,
    iu3dte_t member,
    int index
)
{
    if( index < 0 ) return IUS_ERR_VALUE;
    if( list == NULL   || index >= list->count ) return IUS_ERR_VALUE;
    list->p3DTransducerElements[index] = member;
    return IUS_E_OK;
}